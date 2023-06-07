// sheild.h


#ifndef SHEILD_H
#define SHEILD_H


#include <vector>
#include "headers.h"
#include "object.h"
#include "ship.h"



#define SHEILD_WIDTH 20		     // sheild is 20 m wide
#define SHEILD_MASS 5000		 // sheild is 5000 kg


class Sheild : public Object {

    static float verts[];

public:

    Sheild(vec3 position, vec3 vel, quaternion orient)
        : Object(position, verts, SHEILD_WIDTH, SHEILD_MASS) {

        orientation = orient;
        velocity = vel;
    };

    Sheild(vec3 position)
        : Object(position, verts, SHEILD_WIDTH, SHEILD_MASS)
    {};

    void attach(float deltaT);
    void destroy();

};

#endif

