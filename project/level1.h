#ifndef level1_included
#define level1_included
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

extern int timer;
extern char time_text[10];
extern u_char endGame;
extern int redrawScreen;
extern Region fieldFence;

typedef struct MovLayer_s{
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
}MovLayer;

extern MovLayer ml1, ml0, mlp;

void mlAdvance(MovLayer *ml, Region *fence);

int collisionCheck(MovLayer *obj1, MovLayer *obj2);


#endif
