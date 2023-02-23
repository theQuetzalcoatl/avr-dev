#include "Aztec/kernel/kernel.h"
#include <stdlib.h>
#include <stdbool.h>

/***** Applications *****/

#include "./Applications/heartbeat/heartbeat.h"
#include "./Applications/lcd_backlight_monitor/lcd_backlight_monitor.h"
#include "./Applications/songs/songs.h"

/************************/


register_t kv_stack[CONFIG_MIN_STACK_SIZE + 20];
void display_kernel_version(void)
{
    lease(DEV_LCD);
    lease(DEV_LCD_LIGHT);
    lcd_turn_backligh_on();
    lcd_move_cursor(4,1);
    lcd_print("Aztec "KERNEL_VERSION" ");
    wait_ms(2000);
    lcd_send_command(LCD_CLEAR);
    load(main_heartbeat, heartbeat_stack, sizeof(heartbeat_stack));
    exit_();  // exiting without releasing the LCD -> kernel takes care of it
}

/***********************************************************************/

#define PARENT_MENU  (15u)
#define PRESENT_MENU (16u)
#define ACTION_MENU  (17u)

#define MENU_MARKER_UP   (0u)
#define MENU_MARKER_DOWN (1u)

#define DOWN      KEYPAD_0
#define UP        KEYPAD_5
#define ENTER     KEYPAD_9
#define EXIT      KEYPAD_7
#define ACTIVATE  KEYPAD_8

#define MENU_MARKER "*"

typedef struct menu_point_s
{
    char *name;
    struct menu_point_s *submenu;
    uint8_t type;
    uint8_t is_end;
    uint16_t (*action)(void);
}menu_point_s;


static uint16_t get_systick(void)
{
    return CONFIG_SYSTEM_TICK_IN_US;
}

static uint16_t get_sysclk(void)
{
    return F_CPU/1000000;
}

static uint16_t display_time(void)
{
    char c = 0;
    uint64_t uptime_s = 0;
    uint16_t hours = 0;
    uint8_t minutes = 0;
    uint8_t seconds = 0;
    uint8_t prev_seconds_val = seconds;
    char buffer[4] = {0};

    wait_ms(200); // Needed because otherwise we immideatly escape from the view.

    do{
        uptime_s = get_uptime();;
    
        hours = uptime_s/3600;
        uptime_s -= hours*3600;
    
        minutes = uptime_s/60;
        uptime_s -= minutes*60;

        seconds = uptime_s;

        if(seconds != prev_seconds_val){
            lcd_send_command(LCD_CLEAR);
            lcd_move_cursor(1,1);

            utoa(hours, buffer, 10);
            lcd_print(buffer);
            lcd_print(":");
            utoa(minutes, buffer, 10);
            lcd_print(buffer);
            lcd_print(":");
            utoa(seconds, buffer, 10);
            lcd_print(buffer);

            prev_seconds_val = seconds;
        }
        c = keypad_get_pressed_key_nonblocking();
    }while(c == KEYPAD_NO_NUM);

    return 0;
}

static uint16_t get_threads(void)
{
    return get_num_of_threads();
}

static bool buzzer_is_on = false;
static uint16_t toggle_buzzer_sound(void)
{
    buzzer_is_on = !buzzer_is_on;
    lease(DEV_BUZZER);
    return 0;
}


static uint16_t play_imp_march(void)
{
    release(DEV_BUZZER);
    load(imperial_march, song_stack, sizeof(song_stack));
    return 0;
}

static uint16_t play_tetris(void)
{
    release(DEV_BUZZER);
    load(tetris, song_stack, sizeof(song_stack));
    return 0;
}

static uint16_t play_gta(void)
{
    release(DEV_BUZZER);
    load(gta_sa_theme, song_stack, sizeof(song_stack));
    return 0;
}


register_t menu_stack[CONFIG_MIN_STACK_SIZE + 200];
void menu(void)
{
    wait_ms(1000);
    while(lease(DEV_BUZZER) != NO_ERROR){;}
    while(lease(DEV_LCD) != NO_ERROR){;}

    uint8_t menu_marker_pos = MENU_MARKER_UP;
    uint8_t menu_index = 0;
    char buffer[10] = {0};

    menu_point_s music_submenu[] = {
                                        [0] = {.name = "Imperial March", .type = ACTION_MENU, .action = &play_imp_march, .submenu = 0, .is_end = false},
                                        [1] = {.name = "Tetris", .type = ACTION_MENU, .action = &play_tetris, .submenu = 0, .is_end = false},
                                        [2] = {.name = "GTA SA", .type = ACTION_MENU, .action = &play_gta, .submenu = 0, .is_end = true}
                                    };

    menu_point_s sys_info_submenu[] = {
                                        [0] = {.name = "Threads  - ", .type = PRESENT_MENU, .action = &get_threads, .submenu = 0, .is_end = false},
                                        [1] = {.name = "Systicks - ", .type = PRESENT_MENU, .action = &get_systick, .submenu = 0, .is_end = false},
                                        [2] = {.name = "Clock[MHz] - ", .type = PRESENT_MENU, .action = &get_sysclk, .submenu = 0, .is_end = false},
                                        [3] = {.name = "Uptime        >", .type = ACTION_MENU, .action = &display_time, .submenu = 0, .is_end = true},
                                      };

    menu_point_s keypad_submenu[] = {
                                        [0] = { .name = "On/Off", .type = ACTION_MENU, .action = &toggle_buzzer_sound, .submenu = 0, .is_end = false},
                                        [1] = { .name = " ", .type = ACTION_MENU, .action = 0, .submenu = 0, .is_end = true},
                                    };

    menu_point_s main_menu_points[] = {
                                        [0] = {.name = "Music         >", .type = PARENT_MENU, .action = 0, .submenu = music_submenu, .is_end = false},
                                        [1] = {.name = "System info   >", .type = PARENT_MENU, .action = 0, .submenu = sys_info_submenu, .is_end = false},
                                        [2] = {.name = "Keypad sound  >", .type = PARENT_MENU, .action = 0, .submenu = keypad_submenu, .is_end = false},
                                        [3] = {.name = "Games         >", .type = PARENT_MENU, .action = 0, .submenu = 0, .is_end = true},
                                    };

    menu_point_s *current_menu_point = main_menu_points;

    do{
        lease(DEV_BUZZER); // try to get back buzzer
         /* refresh lcd */
        lcd_send_command(LCD_CLEAR);
        wait_ms(2); /* needed because we can be too fast and print out garbage */
        if(menu_marker_pos == MENU_MARKER_UP){
            lcd_print(MENU_MARKER);
            lcd_print(current_menu_point->name);
            if(current_menu_point->action != 0){
                if(current_menu_point->type == PRESENT_MENU){
                    utoa(current_menu_point->action(), buffer, 10);
                    lcd_print(buffer);
                }
            }
            lcd_move_cursor(2,2);
            lcd_print((current_menu_point+1)->name);
            if((current_menu_point + 1)->action != 0){
                if((current_menu_point + 1)->type == PRESENT_MENU){
                    utoa((current_menu_point + 1)->action(), buffer, 10);
                    lcd_print(buffer);
                }
            }
        }
        else{
            lcd_move_cursor(2,1);
            lcd_print((current_menu_point - 1)->name);
            if((current_menu_point - 1)->action != 0){
                if((current_menu_point - 1)->type == PRESENT_MENU){
                    utoa((current_menu_point - 1)->action(), buffer, 10);
                    lcd_print(buffer);
                }
            }
            lcd_move_cursor(1,2);
            lcd_print(MENU_MARKER);
            lcd_print((current_menu_point)->name);
            if(current_menu_point->action != 0){
                if(current_menu_point->type == PRESENT_MENU){
                    utoa(current_menu_point->action(), buffer, 10);
                    lcd_print(buffer);
                }
            }
        }

        char key = keypad_get_pressed_key();
        switch(key)
        {
            case DOWN:
                if(current_menu_point->is_end == false) {
                    ++menu_index;
                    ++current_menu_point;
                    if(menu_marker_pos == MENU_MARKER_UP) menu_marker_pos = MENU_MARKER_DOWN;
                }
                break;

            case UP:
                if(menu_index > 0){
                    --menu_index;
                    --current_menu_point;
                    if(menu_marker_pos == MENU_MARKER_DOWN) menu_marker_pos = MENU_MARKER_UP;
                }
                break;

            case ENTER:
                if(current_menu_point->type == PARENT_MENU && current_menu_point->submenu != 0){
                    current_menu_point = current_menu_point->submenu;
                    menu_index = 0;
                    menu_marker_pos = MENU_MARKER_UP;
                }
                break;

            case EXIT:
                current_menu_point = main_menu_points;
                menu_index = 0;
                menu_marker_pos = MENU_MARKER_UP;
                break;

            case ACTIVATE:
                if(current_menu_point->action != 0) current_menu_point->action();
                break;
        }

        if(buzzer_is_on){
            buzzer_buzz(350);
            wait_ms(100);
            buzzer_off();
            wait_ms(50);
        }
        else wait_ms(150);
        
    }while(1);
}


/***********************************************************************/


int main(void)
{
    register_device(button_init_device, DEV_BUTTON);
    register_device(led_init_device, DEV_LED1); 
    register_device(led_init_device, DEV_LED2); // multiple init function for different devices
    register_device(led_init_device, DEV_LED3);
    register_device(led_init_device, DEV_LED4);
    register_device(lcd_init_device, DEV_LCD);
    register_device(uart_init_device, DEV_UART);
    register_device(buzzer_init_device, DEV_BUZZER);
    register_device(adc_init_device, DEV_ADC);
    register_device(lcd_init_backlight, DEV_LCD_LIGHT);
    keypad_init_device(); // devices need not be registered by the kernel

    register_thread(display_kernel_version, kv_stack, sizeof(kv_stack));
    register_thread(menu, menu_stack, sizeof(menu_stack));
    register_thread(lcd_backlight_monitor_main, lcd_backlight_monitor_stack, sizeof(lcd_backlight_monitor_stack));

    klog_init(uart_puts);

    klog_log("OH OOH, BIG PROBLEM");

    start_os();

    while(1){;} // just in case
 
    return 0;
}


/***********************************************************************/

/* user defined kernel panic, overwrites the default kernel one */
void kernel_panic(void)
{
    cli();
    DDRC |= 1<<LED3;
    while(1){
        for(uint8_t i = 3; i; --i){
            PORTC |= 1<<LED3;
            _delay_ms(100);
            PORTC &= ~(1<<LED3);
            _delay_ms(100);
        }
        for(uint8_t i = 3; i; --i){
            PORTC |= 1<<LED3;
            _delay_ms(300);
            PORTC &= ~(1<<LED3);
            _delay_ms(100);
        }
        for(uint8_t i = 3; i; --i){
            PORTC |= 1<<LED3;
            _delay_ms(100);
            PORTC &= ~(1<<LED4);
            _delay_ms(100);
        }
        _delay_ms(500);
    }
}
