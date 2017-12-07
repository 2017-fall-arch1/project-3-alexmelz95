#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include "level1.h"
#include "main_menu.h"

#define GREEN_LED BIT6

void main(){
  P1DIR |= GREEN_LED;
  P1OUT |= GREEN_LED;
  //buzzer_init();
  configureClocks();
  lcd_init();
  shapeInit();
  p2sw_init(15);

  shapeInit();

  enableWDTInterrupts();
  os_sr(0x8);;

  mainMenu1();
}
