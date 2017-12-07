#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include "sound.h"

#define GREEN_LED BIT6

int timer = 31;
char time_text[10];
u_char endGame = 0, win = 0;

AbRect player = {abRectGetBounds, abRectCheck, {5,5}};

AbRectOutline fieldOutline = {	/* playing field */
  abRectOutlineGetBounds, abRectOutlineCheck,
  {screenWidth/2 - 10, screenHeight/2 - 10}
};


Layer layer1 = {		/**< Layer with an orange circle */
  (AbShape *)&circle8,
  {(screenWidth/2)+10, (screenHeight/2)+5}, /**< bit below & right of center */
  {0,0}, {0,0},				    /* last & next pos */
  COLOR_VIOLET,
  0,
};

Layer fieldLayer = {
  (AbShape *) &fieldOutline,
  {(screenWidth/2), (screenHeight/2)},
  {0,0},{0,0},
  COLOR_WHITE,
  &layer1,
};

Layer layer0 = {
  (AbShape *)&circle8,
  {(screenWidth/2)-10, (screenHeight/2)+5},
  {0,0},{0,0},
  COLOR_RED,
  &fieldLayer,
};

Layer playerLayer = {
  (AbShape *)&player,
  {screenWidth/2, screenHeight-15},
  {0,0},{0,0},
  COLOR_GREEN,
  &layer0,
};

typedef struct MovLayer_s{
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
}MovLayer;

/* initial value of {0,0} will be overwritten */
MovLayer ml1 = {&layer1, {3,3}, 0 }; /**< not all layers move */
MovLayer ml0 = {&layer0, {-3,6}, &ml1};
MovLayer mlp = {&playerLayer, {0,0}, &ml0}; /* Player */

void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;
  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1],
      bounds.botRight.axes[0], bounds.botRight.axes[1]);
      for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
        for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
          Vec2 pixelPos = {col, row};
          u_int color = bgColor;
          Layer *probeLayer;
          for (probeLayer = layers; probeLayer;
            probeLayer = probeLayer->next) { /* probe all layers, in order */
              if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
                color = probeLayer->color;
                break;
              } /* if probe check */
            } // for checking all layers at col, row
            lcd_writeColor(color);
          } // for col
        } // for row
      } // for moving layer being updated
    }



    //Region fence = {{10,30}, {SHORT_EDGE_PIXELS-10, LONG_EDGE_PIXELS-10}}; /**< Create a fence region */

    /** Advances a moving shape within a fence
    *
    *  \param ml The moving shape to be advanced
    *  \param fence The region which will serve as a boundary for ml
    */
    void mlAdvance(MovLayer *ml, Region *fence)
    {
      Vec2 newPos;
      u_char axis;
      Region shapeBoundary;
      for (; ml; ml = ml->next) {
        vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
        abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
        for (axis = 0; axis < 2; axis ++) {
          if ((shapeBoundary.topLeft.axes[axis] < fence->topLeft.axes[axis]) ||
          (shapeBoundary.botRight.axes[axis] > fence->botRight.axes[axis]) ) {
            int velocity = ml->velocity.axes[axis] = -ml->velocity.axes[axis];
            newPos.axes[axis] += (2*velocity);
          }	/**< if outside of fence */
        } /**< for axis */
        ml->layer->posNext = newPos;
      } /**< for ml */
    }

    int collisionCheck(MovLayer *obj1, MovLayer *obj2){
      Region obj2_bound;
      Region obj1_bound;
      Vec2 coordinates;
      u_char x;

      abShapeGetBounds(obj2->layer->abShape, &obj2->layer->posNext, &obj2_bound);
      vec2Add(&coordinates, &obj1->layer->pos, &obj1->velocity);
      abShapeGetBounds(obj1->layer->abShape, &coordinates, &obj1_bound);

      if(abShapeCheck(obj2->layer->abShape, &obj2->layer->pos, &obj1_bound.topLeft) ||
      abShapeCheck(obj2->layer->abShape, &obj2->layer->pos, &obj1_bound.botRight) ){

        int velocity = obj1->velocity.axes[0] = -obj1->velocity.axes[0];
        coordinates.axes[0] += (2*velocity);
        //buzzer_set_period(1500); //hit sound
        return 1;

      }
      return 0;
    }


    u_int bgColor = COLOR_BLACK;     /**< The background color */
    int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

    Region fieldFence;		/**< fence around playing field  */


    /** Initializes everything, enables interrupts and green LED,
    *  and handles the rendering for the screen
    */
    void main()
    {
      P1DIR |= GREEN_LED;		/**< Green led on when CPU on */
      P1OUT |= GREEN_LED;

      configureClocks();
      lcd_init();
      shapeInit();
      buzzer_init();
      p2sw_init(15);

      shapeInit();

      layerInit(&playerLayer);
      layerDraw(&playerLayer);


      layerGetBounds(&fieldLayer, &fieldFence);


      enableWDTInterrupts();      /**< enable periodic interrupt */
      or_sr(0x8);	              /**< GIE (enable interrupts) */


      for(;;) {
        while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
        P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
        or_sr(0x10);	      /**< CPU OFF */
      }
      P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
      redrawScreen = 0;
      movLayerDraw(&mlp, &playerLayer);
    }
  }

  void wdt_c_handler()
  {
    static short count = 0;
    P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
    u_int switches = p2sw_read(), i;
    char str[5];
    for(i = 0; i < 4; i++)
      str[i] = (switches & (1<<i)) ? 0 : 1;
    str[4] = 0;
    count ++;
    if (count%15 == 0 && !endGame) {
      buzzer_set_period(0);
      if(count%240 == 0){
        timer--;
        if(timer == 0){
          redrawScreen = 0;
          clearScreen(COLOR_BLACK);
          endGame = 1;
          win = 1;
          winner();
          drawString5x7(50,30,"YAY!",COLOR_GREEN,COLOR_BLACK);
          drawString5x7(10,50,"You wake up and", COLOR_GREEN, COLOR_BLACK);
          drawString5x7(10,60,"realize that it", COLOR_GREEN, COLOR_BLACK);
          drawString5x7(10,70,"was just a", COLOR_GREEN, COLOR_BLACK);
          drawString5x7(10,80,"nightmare, whew!", COLOR_GREEN, COLOR_BLACK);
          drawString5x7(30,110,"You Made It!",COLOR_WHITE,COLOR_BLACK);
          buzzer_set_period(0);
        }
        sprintf(time_text, "%02d", timer);
        drawString5x7(60,5,time_text,COLOR_WHITE,COLOR_BLACK);
      }
      collisionCheck(&ml0, &ml1);
      collisionCheck(&ml1, &ml0);
      if(collisionCheck(&ml0, &mlp) || collisionCheck(&ml1, &mlp) || collisionCheck(&mlp,&ml0) || collisionCheck(&mlp,&ml1)){
        buzzer_set_period(4000);
        endGame = 1;
        // lose();
      }
      mlAdvance(&mlp, &fieldFence);
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
      if(endGame && !win){
        clearScreen(COLOR_BLACK);
        drawString5x7(40,30,"OH NO!",COLOR_RED,COLOR_BLACK);
        drawString5x7(20,50,"Seven Ate Nine!", COLOR_RED, COLOR_BLACK);
        drawString5x7(10,90,"Press Button Below",COLOR_WHITE,COLOR_BLACK);
        drawString5x7(30,110,"To Try Again", COLOR_WHITE, COLOR_BLACK);
        redrawScreen = 0;
        buzzer_set_period(0);
      }
      // if(endGame && win){
      //   clearScreen(COLOR_BLACK);
      //   drawString5x7(50,30,"YAY!",COLOR_GREEN,COLOR_BLACK);
      //   drawString5x7(10,50,"You wake up and", COLOR_GREEN, COLOR_BLACK);
      //   drawString5x7(10,60,"realize that it", COLOR_GREEN, COLOR_BLACK);
      //   drawString5x7(10,70,"was just a", COLOR_GREEN, COLOR_BLACK);
      //   drawString5x7(10,80,"nightmare, whew!", COLOR_GREEN, COLOR_BLACK);
      //   drawString5x7(30,110,"You Made It!",COLOR_WHITE,COLOR_BLACK);
      //   redrawScreen = 0;
      //   buzzer_set_period(0);
      // }
    }
    P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
  }
