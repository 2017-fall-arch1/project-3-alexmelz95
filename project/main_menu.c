#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <p2switches.h>

u_char menu = 1;
u_char lvl1 = 0;

void mainMenu1(){
  drawString5x7(30,30,"SEVEN ATE NINE", COLOR_GREEN, COLOR_BLACK);
  drawString5x7(10,50,"You, Six, are the", COLOR_GREEN,COLOR_BLACK);
  drawString5x7(10,60,"only one who knows", COLOR_GREEN, COLOR_BLACK);
  drawString5x7(10,70,"Seven's dark secret", COLOR_GREEN, COLOR_BLACK);
}
//I made a change
