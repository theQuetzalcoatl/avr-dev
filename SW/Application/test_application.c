#include "Aztec/kernel/kernel.h"

static uint8_t song_playing = FALSE;

/***********************************************************************/

register_t heartbeat_stack[CONFIG_MIN_STACK_SIZE+10];
void heartbeat(void)
{
    lease(DEV_LED1);
    for(;;){
        led_1_toggle();
        wait_us(500000);
    }
    release(DEV_LED1);
}

/***********************************************************************/

#if 0
register_t thread_3_stack[CONFIG_MIN_STACK_SIZE+20];
void thread_3(void)
{
    
    wait_ms(10);
    while(lease(DEV_LCD) == K_ERR_INVALID_DEVICE_ACCESS){;}
    lease(DEV_ADC);
    
    char buff[5] = {0};
    adc_t val = 0;
    
    lcd_turn_backligh_on();

    while(1){
        /* adc conversion print */
        adc_convert(&val);
        itoa(val, buff, 10);
        lcd_move_cursor(1, 1);
        lcd_print(buff);
        
        /* lcd params print */
        char lcd_buff[3] = {0};
        uint8_t rows = lcd_get_row_num();
        uint8_t cols = lcd_get_column_num();
        lcd_move_cursor(1, 2);

        lcd_buff[0] = rows/10 + '0';
        lcd_buff[1] = rows%10 + '0';

        lcd_print("rows:");
        lcd_print( lcd_buff );

        lcd_buff[0] = cols/10 + '0';
        lcd_buff[1] = cols%10 + '0';

        lcd_print(" cols:");
        lcd_print( lcd_buff );

        wait_ms(750);
        lcd_send_command(LCD_CLEAR);
    }
    release(DEV_LCD);
    
}
#endif

/***********************************************************************/


register_t kv_stack[CONFIG_MIN_STACK_SIZE + 10];
void display_kernel_version(void)
{
    lease(DEV_LCD);
    lcd_turn_backligh_on();
    lcd_move_cursor(4,1);
    lcd_print("Aztec "KERNEL_VERSION" ");
    wait_ms(2000);
    lcd_send_command(LCD_CLEAR);
    exit_(); 
}

/***********************************************************************/

#define PARENT_MENU (15u)
#define ACTION_MENU (16u)

#define CUCLI_FENT (0u)
#define CUCLI_LENT (1u)

#define LE      KEYPAD_0
#define FEL     KEYPAD_5
#define BELEPES KEYPAD_9
#define KILEPES KEYPAD_7

#define MENU_SELECTION_SIGN ">"

typedef struct
{
    char *name;
    struct menu_point_s *submenus;
    uint8_t type;
    uint8_t menu_size;
    void (*action)(void);
}menu_point_s;


register_t menu_stack[CONFIG_MIN_STACK_SIZE + 100];
void menu(void)
{
    wait_ms(1000);
    while(lease(DEV_BUZZER) != NO_ERROR){;}
    while(lease(DEV_LCD) != NO_ERROR){;}

    uint8_t cucli_helyzet = CUCLI_FENT;
    uint8_t menu_index = 0;

    menu_point_s music_almenu_pontok[] = {
                                            [0] = {.name = "Imp. March", .type = ACTION_MENU, .action = 0, .submenus = 0},
                                            [1] = {.name = "Tetris", .type = ACTION_MENU, .action = 0, .submenus = 0}
                                         };
    for(int i = 0; i < sizeof(music_almenu_pontok)/sizeof(music_almenu_pontok[0]); ++i) music_almenu_pontok[i].menu_size = sizeof(music_almenu_pontok)/sizeof(music_almenu_pontok[0]);

    menu_point_s fo_menu_pontok[] = {
                                        [0] = {.name = "Music...", .type = PARENT_MENU, .action = 0, .submenus = &music_almenu_pontok},
                                        [1] = {.name = "System info...", .type = PARENT_MENU, .action = 0, .submenus = 0},
                                        [2] = {.name = "Menu 1", .type = ACTION_MENU, .action = 0, .submenus = 0},
                                        [3] = {.name = "Menu 2", .type = ACTION_MENU, .action = 0, .submenus = 0},
                                        [4] = {.name = "Menu 3", .type = ACTION_MENU, .action = 0, .submenus = 0}
                                    };
    for(int i = 0; i < sizeof(fo_menu_pontok)/sizeof(fo_menu_pontok[0]); ++i) fo_menu_pontok[i].menu_size = sizeof(fo_menu_pontok)/sizeof(fo_menu_pontok[0]);

    menu_point_s *current_menu_point = &fo_menu_pontok;
    
    lcd_send_command(LCD_CLEAR);
    if(cucli_helyzet == CUCLI_FENT) lcd_print(MENU_SELECTION_SIGN);
    lcd_move_cursor(2,1);
    lcd_print(fo_menu_pontok[menu_index].name);
    if(cucli_helyzet == CUCLI_LENT) lcd_print(MENU_SELECTION_SIGN);
    lcd_move_cursor(2,2);
    lcd_print(fo_menu_pontok[menu_index + 1].name);

    while(1){
        char key = keypad_get_pressed_key();
        switch(key)
        {
            case LE:
                if(menu_index < current_menu_point->menu_size - 1) {
                    ++menu_index;
                    ++current_menu_point;
                    if(cucli_helyzet == CUCLI_FENT) cucli_helyzet = CUCLI_LENT;
                }
                break;

            case FEL:
                if(menu_index > 0){
                    --menu_index;
                    --current_menu_point;
                    if(cucli_helyzet == CUCLI_LENT) cucli_helyzet = CUCLI_FENT;
                }
                break;

            case BELEPES:
                if(current_menu_point->type == PARENT_MENU && current_menu_point->submenus != 0){
                    current_menu_point = current_menu_point->submenus;
                    menu_index = 0;
                    cucli_helyzet = CUCLI_FENT;
                }
                break;

            case KILEPES:
                current_menu_point = &fo_menu_pontok;
                menu_index = 0;
                cucli_helyzet = CUCLI_FENT;
            break;
        }

        /* refresh lcd */
        lcd_send_command(LCD_CLEAR);
        if(cucli_helyzet == CUCLI_FENT){
            lcd_print(MENU_SELECTION_SIGN);
            lcd_print(current_menu_point->name);
            lcd_move_cursor(2,2);
            lcd_print((current_menu_point+1)->name);
        }
        else{
            lcd_move_cursor(2,1);
            lcd_print((current_menu_point - 1)->name);
            lcd_move_cursor(1,2);
            lcd_print(MENU_SELECTION_SIGN);
            lcd_print((current_menu_point)->name);
        }
        
        buzzer_buzz(350);
        wait_ms(100);
        buzzer_off();
        wait_ms(50);
    }
}

/***********************************************************************/

register_t imp_march_stack[CONFIG_MIN_STACK_SIZE + 10];
void imperial_march(void)
{
    wait_ms(1000);
    while(lease(DEV_BUZZER) != NO_ERROR){;}


    while(1){
        buzzer_buzz(20);
    }
}


/***********************************************************************/

int main(void)
{
    init_device(button_init_device, DEV_BUTTON);
    init_device(led_init_device, DEV_LED1); 
    init_device(led_init_device, DEV_LED2);
    init_device(led_init_device, DEV_LED3);
    init_device(led_init_device, DEV_LED4);
    init_device(lcd_init_device, DEV_LCD);
    init_device(uart_init_device, DEV_UART);
    init_device(buzzer_init_device, DEV_BUZZER);
    init_device(adc_init_device, DEV_ADC);
    keypad_init_device();

    register_thread(heartbeat, heartbeat_stack, sizeof(heartbeat_stack));
    register_thread(display_kernel_version, kv_stack, sizeof(kv_stack));
    register_thread(menu, menu_stack, sizeof(menu_stack));

    start_os();

    while(1){;} // just in case
 
    return 0;
}