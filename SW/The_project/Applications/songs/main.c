#include "songs.h"
#include "stdbool.h"

#define A3 (220u)
#define C4 (261u)
#define B3 (246u)
#define D4 (293u)
#define E4 (329u)
#define G4 (392u)
#define B4 (493u)
#define C5 (523u)
#define G5 (783u)
#define Bb5 (932u)
#define C6 (1046u)
#define D6 (1174u)
#define Eb6 (1244u)
#define G6 (1567u)

register_t song_stack[CONFIG_MIN_STACK_SIZE + 20];


void tetris(void)
{
  lease(DEV_BUZZER);
  
  buzzer_buzz(E4);
  wait_ms(500);
    
  buzzer_buzz(B3);
  wait_ms(250);
  
  buzzer_buzz(C4);
  wait_ms(250);
  
  buzzer_buzz(D4);
  wait_ms(400);
  
  buzzer_buzz(C4);
  wait_ms(250);
  
  buzzer_buzz(B3);
  wait_ms(250);
  
  buzzer_buzz(A3);
  wait_ms(250);
  buzzer_off();
  wait_ms(120);
  
  buzzer_buzz(A3);
  wait_ms(250);
  
  buzzer_buzz(C4);
  wait_ms(250);
  
  buzzer_buzz(E4);
  wait_ms(400);
  
  buzzer_buzz(D4);
  wait_ms(250);
  
  buzzer_buzz(C4);
  wait_ms(250);
  
  buzzer_buzz(B3);
  wait_ms(500);
  buzzer_off();
  wait_ms(75);
  
  buzzer_buzz(C4);
  wait_ms(250);
  
  buzzer_buzz(D4);
  wait_ms(400);
  
  buzzer_buzz(E4);
  wait_ms(400);
  
  buzzer_buzz(C4);
  wait_ms(400);
  
  buzzer_buzz(A3);
  wait_ms(350);
  buzzer_off();
  wait_ms(75);
  
  buzzer_buzz(A3);
  wait_ms(350);
  buzzer_off();

  release(DEV_BUZZER);
  exit_();
}


void gta_sa_theme(void)
{
  lease(DEV_BUZZER);
  
  buzzer_buzz(G5);
  wait_ms(400);
  buzzer_off();
  wait_ms(70);
    
  buzzer_buzz(G6);
  wait_ms(400);
      
  buzzer_buzz(Eb6);
  wait_ms(300);
  buzzer_off();
  wait_ms(50);

  buzzer_buzz(D6);
  wait_ms(200);
      
  buzzer_buzz(Eb6);
  wait_ms(200);
      
  buzzer_buzz(D6);
  wait_ms(200);
      
  buzzer_buzz(C6);
  wait_ms(200);
      
  buzzer_buzz(D6);
  wait_ms(200);
  buzzer_off();
  wait_ms(150);
  
  buzzer_buzz(D6);
  wait_ms(600);
  buzzer_off();
  wait_ms(150);
    
  buzzer_buzz(Bb5);
  wait_ms(400);
  buzzer_off();
  wait_ms(50);
  
  buzzer_buzz(C6);
  wait_ms(200);
      
  buzzer_buzz(Bb5);
  wait_ms(200);
      
  buzzer_buzz(C6);
  wait_ms(400);
      
  buzzer_buzz(D6);
  wait_ms(300);
  buzzer_off();
  wait_ms(50);
  
  buzzer_buzz(G5);
  wait_ms(300);
  buzzer_off();
  wait_ms(30);
  
  buzzer_buzz(C6);
  wait_ms(300);
  buzzer_off();
  wait_ms(40);
  
  buzzer_buzz(Bb5);
  wait_ms(200);
      
  buzzer_buzz(G5);
  wait_ms(200);
  buzzer_off();

  release(DEV_BUZZER);
  exit_();
}


void imperial_march(void)
{
  lease(DEV_BUZZER);

  for(int i = 0; i < 3; ++i){
     buzzer_buzz(E4);
    wait_ms(550);
    buzzer_off();
    wait_ms(75);    
  }

  for(int i = 0; i < 2; ++i){
     buzzer_buzz(C4);
    wait_ms(450);
    buzzer_off();
    wait_ms(10);    

     buzzer_buzz(G4);
    wait_ms(200);
    buzzer_off();
    wait_ms(10);    

    buzzer_buzz(E4);
    wait_ms(550);
    buzzer_off();
    wait_ms(75);    
  }

  wait_ms(500);

  for(int i = 0; i < 3; ++i){
      buzzer_buzz(B4);
      wait_ms(550);
      buzzer_off();
      wait_ms(75);    
  }

  buzzer_buzz(C5);
  wait_ms(450);
  buzzer_off();
  wait_ms(10);

  buzzer_buzz(G4);
  wait_ms(200);
  buzzer_off();
  wait_ms(10);

  buzzer_buzz(E4);
  wait_ms(550);
  buzzer_off();
  wait_ms(75);

  buzzer_buzz(C4);
  wait_ms(450);
  buzzer_off();
  wait_ms(10);

  buzzer_buzz(G4);
  wait_ms(200);
  buzzer_off();
  wait_ms(10);

  buzzer_buzz(E4);
  wait_ms(550);
  buzzer_off();
  wait_ms(75);

  release(DEV_BUZZER);
  exit_();
}