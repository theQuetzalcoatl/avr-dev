#include "Aztec/kernel/kernel.h"


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

#define PARENT_MENU  (15u)
#define PRESENT_MENU (16u)
#define ACTION_MENU  (17u)

#define CUCLI_FENT (0u)
#define CUCLI_LENT (1u)

#define LE      KEYPAD_0
#define FEL     KEYPAD_5
#define BELEPES KEYPAD_9
#define KILEPES KEYPAD_7
#define ACTIVATE KEYPAD_8

#define MENU_SELECTION_SIGN "*"

typedef struct
{
    char *name;
    struct menu_point_s *submenus;
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

static uint16_t get_threads(void)
{
    return get_num_of_threads();
}

static uint8_t buzzer_is_on = FALSE;
static uint16_t toggle_buzzer_sound(void)
{
    buzzer_is_on = !buzzer_is_on;
    lease(DEV_BUZZER);
    return 0;
}

static volatile uint8_t imp_march_plays = FALSE;
static volatile uint8_t imp_march_waiting = FALSE;
static uint16_t play_imp_march(void)
{
    imp_march_plays = !imp_march_plays;
    if(imp_march_plays == TRUE) release(DEV_BUZZER);
    else{
        while(imp_march_waiting == FALSE){;}
        lease(DEV_BUZZER);
    }
    return 0;
}


static volatile uint8_t tetris_plays = FALSE;
static volatile uint8_t tetris_waiting = FALSE;
static uint16_t play_tetris(void)
{
    tetris_plays = !tetris_plays;
    if(tetris_plays == TRUE) release(DEV_BUZZER);
    else{
        while(tetris_waiting == FALSE){;}
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

    uint8_t cucli_helyzet = CUCLI_FENT;
    uint8_t menu_index = 0;
    char buffer[10] = {0};

    menu_point_s music_almenu_pontok[] = {
                                            [0] = {.name = "Imp. March", .type = ACTION_MENU, .action = &play_imp_march, .submenus = 0, .is_end = FALSE},
                                            [1] = {.name = "Tetris", .type = ACTION_MENU, .action = &play_tetris, .submenus = 0, .is_end = TRUE}
                                         };

    menu_point_s sys_info_submenu[] = {
                                        [0] = {.name = "Threads  - ", .type = PRESENT_MENU, .action = &get_threads, .submenus = 0, .is_end = FALSE},
                                        [1] = {.name = "Systicks - ", .type = PRESENT_MENU, .action = &get_systick, .submenus = 0, .is_end = FALSE},
                                        [2] = {.name = "Clock[MHz] - ", .type = PRESENT_MENU, .action = &get_sysclk, .submenus = 0, .is_end = TRUE}
                                      };

    menu_point_s keypad_submenu[] = {
                                       [0] = { .name = "On/Off", .type = ACTION_MENU, .action = &toggle_buzzer_sound, .submenus = 0, .is_end = FALSE},
                                       [1] = { .name = " ", .type = ACTION_MENU, .action = 0, .submenus = 0, .is_end = TRUE},
                                    };

    menu_point_s fo_menu_pontok[] = {
                                        [0] = {.name = "Music         >", .type = PARENT_MENU, .action = 0, .submenus = &music_almenu_pontok, .is_end = FALSE},
                                        [1] = {.name = "System info   >", .type = PARENT_MENU, .action = 0, .submenus = &sys_info_submenu, .is_end = FALSE},
                                        [2] = {.name = "Keypad sound  >", .type = PARENT_MENU, .action = 0, .submenus = &keypad_submenu, .is_end = TRUE},
                                    };

    menu_point_s *current_menu_point = &fo_menu_pontok;
    /*
    lcd_send_command(LCD_CLEAR);
    if(cucli_helyzet == CUCLI_FENT) lcd_print(MENU_SELECTION_SIGN);
    lcd_move_cursor(2,1);
    lcd_print(fo_menu_pontok[menu_index].name);
    if(cucli_helyzet == CUCLI_LENT) lcd_print(MENU_SELECTION_SIGN);
    lcd_move_cursor(2,2);
    lcd_print(fo_menu_pontok[menu_index + 1].name);
*/
    do{

         /* refresh lcd */
        lcd_send_command(LCD_CLEAR);
        if(cucli_helyzet == CUCLI_FENT){
            lcd_print(MENU_SELECTION_SIGN);
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
            lcd_print(MENU_SELECTION_SIGN);
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
            case LE:
                if(current_menu_point->is_end == FALSE) {
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
    if(imp_march_plays == FALSE){   
        release(DEV_BUZZER);
        imp_march_waiting = TRUE;
        while(imp_march_plays == FALSE){;}
        while(lease(DEV_BUZZER) != NO_ERROR){;}
        imp_march_waiting = FALSE;
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
    if(tetris_plays == FALSE){   
        release(DEV_BUZZER);
        tetris_waiting = TRUE;
        while(tetris_plays == FALSE){;}
        while(lease(DEV_BUZZER) != NO_ERROR){;}
        tetris_waiting = FALSE;
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
    register_thread(imperial_march, imp_march_stack, sizeof(imp_march_stack));
    register_thread(tetris, tetris_stack, sizeof(tetris_stack));

    start_os();

    while(1){;} // just in case
 
    return 0;
}