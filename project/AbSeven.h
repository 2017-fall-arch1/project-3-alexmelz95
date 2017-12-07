#ifndef abCircle_included
#define abCircle_included

#include "shape.h"

/** AbShape circle
 *
 *  chords should be a vector of length radius + 1.
 *  Entry at index i is 1/2 chord length at distance i from the circle's center.
 *  This vector can be generated using lcdLib's computeChordVec() (lcddraw.h).
 */
typedef struct AbSeven_s {
  void (*getBounds)(const struct AbSeven_s *shape, const Vec2 *centerPos, Region *bounds);
  int (*check)(const struct AbSeven_s *shape, const Vec2 *centerPos, const Vec2 *pixelLoc);
  int size;
} AbSeven;

/** Required by AbShape
 */
void abSevenGetBounds(const AbSeven *seven, const Vec2 *centerPos, Region *bounds);

/** Required by AbShape
 */
int abSevenCheck(const AbSeven *seven, const Vec2 *centerPos, const Vec2 *pixel);

#endif
