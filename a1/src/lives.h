// lives.h


#ifndef LIVES
#define LIVES


#include <vector>
#include "headers.h"
#include "object.h"

#define SHIP_WIDTH 10		 // ship is 10 m wide
#define SHIP_MASS 5000		 // ship is 5000 kg

class Lives : public Object {

  static float verts[];

 public:

  Lives( vec3 position )
    : Object( position, verts, SHIP_WIDTH, SHIP_MASS )
    {};
};


#endif
