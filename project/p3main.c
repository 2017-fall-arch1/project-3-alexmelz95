#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

#define GREEN_LED BIT6

AbRect rect10 = {abRectGetBounds, abRectCheck, {10,10}};

Region fence = {{10,30}, {SHORT_EDGE_PIXELS-10, LONG_EDGE_PIXELS-10}};

Layer layer1 = {
  (AbShape *) &rect10,
  {screenWidth/2, screenHeight/2},
  {0,0} ,{0,0},
  COLOR_RED,
  0,
};
Layer layer0 = {
  (AbShape *) &rect10,
  {(screenWidth/2)+15, (screenHeight/2)+10},
  {0,0},{0,0},
  COLOR_RED,
  0,
};

int
main(){
  configureClocks();
  lcd_init();
  shapeInit();
  Vec2 rectPos = screenCenter, circlePos = {30, screenHeight - 30};

  clearScreen(COLOR_BLUE);
  shapeInit();

  layerDraw(&layer0);
}
