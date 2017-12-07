#ifndef abCircle_included
#define abCircle_included

#include "shape.h"

/** AbShape circle
 *
 *  chords should be a vector of length radius + 1.
 *  Entry at index i is 1/2 chord length at distance i from the circle's center.
 *  This vector can be generated using lcdLib's computeChordVec() (lcddraw.h).
 */
typedef struct AbNine_s {
  void (*getBounds)(const struct AbNine_s *shape, const Vec2 *centerPos, Region *bounds);
  int (*check)(const struct AbNine_s *shape, const Vec2 *centerPos, const Vec2 *pixelLoc);
  int size;
} AbNine;

/** Required by AbShape
 */
void abNineGetBounds(const AbNine *nine, const Vec2 *centerPos, Region *bounds);

/** Required by AbShape
 */
int abNineCheck(const AbNine *nine, const Vec2 *centerPos, const Vec2 *pixel);

#endif
