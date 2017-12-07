#include <p2switches.h>
#include <libTimer.h>
#include <lcddraw.h>
#include "level1.h"
#include "main_menu.h"

#define GREEN_LED BIT6

void wdt_c_handler()
{
  static short count = 0;
  static short timer_count = 0;
  u_int switches = p2sw_read(), i;
  char str[5];
  for(i = 0; i < 4; i++)
    str[i] = (switches & (1<<i)) ? 0 : 1;
  str[4] = 0;
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
    timer_count ++;
    if (count == 15) {
      if(!endGame){
	if(timer_count == 240){
	  timer--;
	  sprintf(time_text, "%02d", timer);
	  drawString5x7(60,5,time_text,COLOR_WHITE,COLOR_BLACK);
	  timer_count = 0;
	}
	collisionCheck(&ml0, &ml1);
	collisionCheck(&ml1, &ml0);
	if(collisionCheck(&ml0, &mlp) || collisionCheck(&ml1, &mlp)|| collisionCheck(&mlp,&ml0) || collisionCheck(&mlp,&ml1)){
	  endGame = 1;
	}
	mlAdvance(&mlp, &fieldFence);
	u_int switches = p2sw_read(), i;
	short str[5];
	for (i = 0; i < 4; i++)
	  str[i] = (switches & (1<<i)) ? 0 : 1;
	str[4] = 0;

	if(str[0]){
	  mlp.velocity.axes[0] = -5;
	  mlp.velocity.axes[1] = 0;
	}
	if(str[1]){
	  mlp.velocity.axes[0] = 0;
	  mlp.velocity.axes[1] = -5;
	}
	if(str[2]){
	  mlp.velocity.axes[0] = 0;
	  mlp.velocity.axes[1] = 5;
	}
	if(str[3]){
	  mlp.velocity.axes[0] = 5;
	  mlp.velocity.axes[1] = 0;
	}
	if(!str[0] && !str[1] && !str[2] && !str[3]){
	  mlp.velocity.axes[0] = 0;
	  mlp.velocity.axes[1] = 0;
	}
	if (p2sw_read())
	  redrawScreen = 1;
	count = 0;
      }
      if(endGame){
	clearScreen(COLOR_BLACK);
	drawString5x7(40,30,"Oh no!",COLOR_GREEN,COLOR_BLACK);
	drawString5x7(20,50,"You were eaten", COLOR_GREEN, COLOR_BLACK);
	drawString5x7(20,60,"by Seven!", COLOR_GREEN, COLOR_BLACK);
	drawString5x7(20,90,"S1: Continue",COLOR_WHITE,COLOR_BLACK);
      }
    }
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
  }
}
