#include <p2switches.h>
#include <libTimer.h>
#include <lcddraw.h>
#include "level1.h"
#include "main_menu.h"

#define GREEN_LED BIT6

void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  if(menu){
    if(str[0]){
      menu = 0;
      lvl1 = 1;
      start1();
    }
    //if(str[1])
    //controlInstructions();
  }
  if(lvl1){
    if (count%15 == 0) {
      if(count%240 == 0){
        timer--;
        sprintf(time_text, "%02d", timer);
        drawString5x7(60,5,time_text,COLOR_WHITE,COLOR_BLACK);
      }
      collisionCheck(&ml0, &ml1);
      collisionCheck(&ml1, &ml0);
      if(collisionCheck(&ml0, &mlp) || collisionCheck(&ml1, &mlp)|| collisionCheck(&mlp,&ml0) || collisionCheck(&mlp,&ml1)){
        endGame();
      }
      mlAdvance(&mlp, &fieldFence);
      gameSwitchCheck();
    }
  }
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}
