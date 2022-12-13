#include "Aztec/kernel/kernel.h"
#include <stdlib.h>
#include <stdbool.h>

/***** Applications *****/

#include "./Applications/heartbeat/heartbeat.h"

/************************/

#define BACKLIGHT_ON_DURATION_SEC (10u)

register_t lcd_backlight_monitor_stack[CONFIG_MIN_STACK_SIZE+28];
void lcd_backlight_monitor(void)
{
    while(lease(DEV_LCD_LIGHT) != NO_ERROR){;}
    uint32_t t1 = 0;
    uint32_t t2 = 0;

    for(;;){
        t2 = get_uptime();

        if(keypad_get_pressed_key_nonblocking() != KEYPAD_NO_NUM){
            t1 = t2; // resetting the timer 
            lcd_turn_backligh_on();
        }
        
        if(t2 - t1 > BACKLIGHT_ON_DURATION_SEC){
            t1 = t2;
            lcd_turn_backligh_off();
        }
    }
    release(DEV_LCD_LIGHT);
}

/***********************************************************************/


register_t kv_stack[CONFIG_MIN_STACK_SIZE + 10];
void display_kernel_version(void)
{
    lease(DEV_LCD);
    lease(DEV_LCD_LIGHT);
    lcd_turn_backligh_on();
    lcd_move_cursor(4,1);
    lcd_print("Aztec "KERNEL_VERSION" ");
    wait_ms(2000);
    lcd_send_command(LCD_CLEAR);
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

static uint8_t buzzer_is_on = false;
static uint16_t toggle_buzzer_sound(void)
{
    buzzer_is_on = !buzzer_is_on;
    lease(DEV_BUZZER);
    return 0;
}

static volatile uint8_t imp_march_plays = false;
static volatile uint8_t imp_march_waiting = false;
static uint16_t play_imp_march(void)
{
    imp_march_plays = !imp_march_plays;
    if(imp_march_plays == true) release(DEV_BUZZER);
    else{
        while(imp_march_waiting == false){;}
        lease(DEV_BUZZER);
    }
    return 0;
}


static volatile uint8_t tetris_plays = false;
static volatile uint8_t tetris_waiting = false;
static uint16_t play_tetris(void)
{
    tetris_plays = !tetris_plays;
    if(tetris_plays == true) release(DEV_BUZZER);
    else{
        while(tetris_waiting == false){;}
        lease(DEV_BUZZER);
    }
    return 0;
}


static volatile uint8_t gta_plays = false;
static volatile uint8_t gta_waiting = false;
static uint16_t play_gta(void)
{
    gta_plays = !gta_plays;
    if(gta_plays == true) release(DEV_BUZZER);
    else{
        while(gta_waiting == false){;}
        lease(DEV_BUZZER);
    }
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


static void pause_imp_march(void)
{
    if(imp_march_plays == false){   
        release(DEV_BUZZER);
        imp_march_waiting = true;
        while(imp_march_plays == false){;}
        while(lease(DEV_BUZZER) != NO_ERROR){;}
        imp_march_waiting = false;
    }
}


register_t imp_march_stack[CONFIG_MIN_STACK_SIZE + 30];
void imperial_march(void)
{
    wait_ms(1500);
    
    while(lease(DEV_BUZZER) != NO_ERROR){;}

    while(1){
        for(int i = 0; i < 3; ++i){
            /* E4 */
            buzzer_buzz(329);
            wait_ms(550);
            buzzer_off();
            wait_ms(75);

            pause_imp_march();
        }

        for(int i = 0; i < 2; ++i){
            /* C4 */
            buzzer_buzz(261);
            wait_ms(450);
            buzzer_off();
            wait_ms(10);

            pause_imp_march();

            /* G4 */
            buzzer_buzz(392);
            wait_ms(200);
            buzzer_off();
            wait_ms(10);

            pause_imp_march();

            /* E4 */
            buzzer_buzz(329);
            wait_ms(550);
            buzzer_off();
            wait_ms(75);

            pause_imp_march();

        }

        wait_ms(500);

        for(int i = 0; i < 3; ++i){
            /* B4 */
            buzzer_buzz(493);
            wait_ms(550);
            buzzer_off();
            wait_ms(75);

            pause_imp_march();
        }

        /* C4 */
        buzzer_buzz(523);
        wait_ms(450);
        buzzer_off();
        wait_ms(10);

        pause_imp_march();

        /* G4 */
        buzzer_buzz(392);
        wait_ms(200);
        buzzer_off();
        wait_ms(10);

        pause_imp_march();

        /* E4 */
        buzzer_buzz(329);
        wait_ms(550);
        buzzer_off();
        wait_ms(75);

        pause_imp_march();

        /* C4 */
        buzzer_buzz(261);
        wait_ms(450);
        buzzer_off();
        wait_ms(10);

        pause_imp_march();

        /* G4 */
        buzzer_buzz(392);
        wait_ms(200);
        buzzer_off();
        wait_ms(10);

        pause_imp_march();

        /* E4 */
        buzzer_buzz(329);
        wait_ms(550);
        buzzer_off();
        wait_ms(75);

        pause_imp_march();

        wait_ms(1000);
    }
}

/***********************************************************************/

static void pause_tetris(void)
{
    if(tetris_plays == false){   
        release(DEV_BUZZER);
        tetris_waiting = true;
        while(tetris_plays == false){;}
        while(lease(DEV_BUZZER) != NO_ERROR){;}
        tetris_waiting = false;
    }
}

register_t tetris_stack[CONFIG_MIN_STACK_SIZE + 30];
void tetris(void)
{
    wait_ms(1700);
    
    while(lease(DEV_BUZZER) != NO_ERROR){;}

    while(1){
        /* E4 */
        buzzer_buzz(329);
        wait_ms(500);
        buzzer_off();

        pause_tetris();

        /* B3 */
        buzzer_buzz(246);
        wait_ms(250);
        buzzer_off();

        pause_tetris();

        /* C4 */
        buzzer_buzz(261);
        wait_ms(250);
        buzzer_off();

        pause_tetris();

        /* D4 */
        buzzer_buzz(293);
        wait_ms(400);
        buzzer_off();

        pause_tetris();

        /* C4 */
        buzzer_buzz(261);
        wait_ms(250);
        buzzer_off();

        pause_tetris();

        /* B3 */
        buzzer_buzz(246);
        wait_ms(250);
        buzzer_off();

        pause_tetris();

        /* A3 */
        buzzer_buzz(220);
        wait_ms(250);
        buzzer_off();

        pause_tetris();

        wait_ms(120);

        /* A3 */
        buzzer_buzz(220);
        wait_ms(250);
        buzzer_off();

        pause_tetris();

        /* C4 */
        buzzer_buzz(261);
        wait_ms(250);
        buzzer_off();

        pause_tetris();

        /* E4 */
        buzzer_buzz(329);
        wait_ms(400);
        buzzer_off();

        pause_tetris();

        /* D4 */
        buzzer_buzz(293);
        wait_ms(250);
        buzzer_off();

        pause_tetris();

        /* C4 */
        buzzer_buzz(261);
        wait_ms(250);
        buzzer_off();

        pause_tetris();

        /* B3 */
        buzzer_buzz(246);
        wait_ms(500);
        buzzer_off();

        pause_tetris();

        wait_ms(75);

        /* C4 */
        buzzer_buzz(261);
        wait_ms(250);
        buzzer_off();

        pause_tetris();

        /* D4 */
        buzzer_buzz(293);
        wait_ms(400);
        buzzer_off();

        pause_tetris();

        /* E4 */
        buzzer_buzz(329);
        wait_ms(400);
        buzzer_off();

        pause_tetris();

        /* C4 */
        buzzer_buzz(261);
        wait_ms(400);
        buzzer_off();

        pause_tetris();

        /* A3 */
        buzzer_buzz(220);
        wait_ms(350);
        buzzer_off();

        pause_tetris();

        wait_ms(75);

        /* A3 */
        buzzer_buzz(220);
        wait_ms(350);
        buzzer_off();

        pause_tetris();

        wait_ms(1000);
    }
}


/***********************************************************************/

static void pause_gta(void)
{
    if(gta_plays == false){   
        release(DEV_BUZZER);
        gta_waiting = true;
        while(gta_plays == false){;}
        while(lease(DEV_BUZZER) != NO_ERROR){;}
        gta_waiting = false;
    }
}


register_t gta_sa_stack[CONFIG_MIN_STACK_SIZE + 20];
void gta_sa_theme(void)
{
    wait_ms(1700);
    
    while(lease(DEV_BUZZER) != NO_ERROR){;}

    while(4){
        /* G5 */
        buzzer_buzz(783);
        wait_ms(400);
        buzzer_off();

        wait_ms(70);

        pause_gta();

        /* G6 */
        buzzer_buzz(1567);
        wait_ms(400);
        buzzer_off();

        pause_gta();

        /* Eb6 */
        buzzer_buzz(1244);
        wait_ms(300);
        buzzer_off();

        pause_gta();

        wait_ms(50);

        /* D6 */
        buzzer_buzz(1174);
        wait_ms(200);
        buzzer_off();

        pause_gta();

        /* Eb6 */
        buzzer_buzz(1244);
        wait_ms(200);
        buzzer_off();

        pause_gta();

        /* D6 */
        buzzer_buzz(1174);
        wait_ms(200);
        buzzer_off();

        pause_gta();

        /* C6 */
        buzzer_buzz(1046);
        wait_ms(200);
        buzzer_off();

        pause_gta();

        /* D6 */
        buzzer_buzz(1174);
        wait_ms(200);
        buzzer_off();

        pause_gta();

        wait_ms(150);

        /* D6 */
        buzzer_buzz(1174);
        wait_ms(600);
        buzzer_off();

        wait_ms(150);

        pause_gta();

        /* Bb5 */
        buzzer_buzz(932);
        wait_ms(400);
        buzzer_off();

        pause_gta();

        wait_ms(50);

        /* C6 */
        buzzer_buzz(1046);
        wait_ms(200);
        buzzer_off();

        pause_gta();

        /* Bb5 */
        buzzer_buzz(932);
        wait_ms(200);
        buzzer_off();

        pause_gta();

        /* C6 */
        buzzer_buzz(1046);
        wait_ms(400);
        buzzer_off();

        pause_gta();

        /* D6 */
        buzzer_buzz(1174);
        wait_ms(300);
        buzzer_off();

        pause_gta();

        wait_ms(50);

        /* G5 */
        buzzer_buzz(783);
        wait_ms(300);
        buzzer_off();

        pause_gta();

        wait_ms(30);

        /* C6 */
        buzzer_buzz(1046);
        wait_ms(300);
        buzzer_off();

        pause_gta();

        wait_ms(40);

        /* Bb5 */
        buzzer_buzz(932);
        wait_ms(200);
        buzzer_off();

        pause_gta();

        /* G5 */
        buzzer_buzz(783);
        wait_ms(200);
        buzzer_off();

        pause_gta();

        wait_ms(1000);

    }
}

/***********************************************************************/


int main(void)
{
    register_device(button_init_device, DEV_BUTTON);
    register_device(led_init_device, DEV_LED1); 
    register_device(led_init_device, DEV_LED2);
    register_device(led_init_device, DEV_LED3);
    register_device(led_init_device, DEV_LED4);
    register_device(lcd_init_device, DEV_LCD);
    register_device(uart_init_device, DEV_UART);
    register_device(buzzer_init_device, DEV_BUZZER);
    register_device(adc_init_device, DEV_ADC);
    register_device(lcd_init_backlight, DEV_LCD_LIGHT);
    keypad_init_device(); // devices need not be registered by the kernel

    register_thread(main_heartbeat, heartbeat_stack, sizeof(heartbeat_stack));
    register_thread(display_kernel_version, kv_stack, sizeof(kv_stack));
    register_thread(menu, menu_stack, sizeof(menu_stack));
    register_thread(imperial_march, imp_march_stack, sizeof(imp_march_stack));
    register_thread(tetris, tetris_stack, sizeof(tetris_stack));
    register_thread(gta_sa_theme, gta_sa_stack, sizeof(gta_sa_stack));
    register_thread(lcd_backlight_monitor, lcd_backlight_monitor_stack, sizeof(lcd_backlight_monitor_stack));

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