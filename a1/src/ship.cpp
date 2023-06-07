// ship.cpp


#include "ship.h"


void Ship::rotateCW( float deltaT )

{
	// The below if statement works to keep the ship from reversing its rotation however
	// you can not turn CW to start the game. Must start by turning left or else the ship disappears
	//if (orientation.angle() < 0.1) orientation = quaternion(6.28, orientation.axis());
    orientation = quaternion( - SHIP_ROTATION_SPEED * deltaT, vec3(0,0,1) ) * orientation;
	if (orientation.angle() < 0.1) orientation = quaternion(6.28, orientation.axis());
}


void Ship::rotateCCW( float deltaT )

{
	// check if orientaton has reached 6.28 (full circle) then reset to slightly past it
	//if (orientation.angle() > 6.28) orientation = quaternion(0.1,orientation.axis());
    orientation = quaternion( + SHIP_ROTATION_SPEED * deltaT, vec3(0,0,1) ) * orientation;
	if (orientation.angle() > 6.26) orientation = quaternion(0.1, orientation.axis());

}


void Ship::addThrust( float deltaT )

{
  // Thrust is in the ship's +y direction.  Make sure to change the
  // velocity in that direction in the *world* coordinate system,
  // since the object velocity is in the world coordinate system.

  // YOUR CODE HERE
	velocity.x -= SHIP_THRUST_ACCEL * sin(orientation.angle()) * deltaT;
	velocity.y += SHIP_THRUST_ACCEL * cos(orientation.angle()) * deltaT;
}


Shell * Ship::fireShell()

{
  // YOUR CODE HERE (below, find the correct position, velocity, and orientation for the shell)

	vec3 shellVelocity = { -SHELL_SPEED * sin(orientation.angle()), SHELL_SPEED * cos(orientation.angle()), 0 };

  // Position and orientation will be the same as the ship
  return new Shell(position, shellVelocity, orientation);
}


// Ship model consisting of line segments
//
// These are in a ARBITRARY coordinate system and get remapped to the
// world coordinate system (with the centre of the ship at (0,0) and
// width SHIP_WIDTH) when the VAO is set up.


float Ship::verts[] = {

   3,0,  0,9,
   0,9, -3,0, 
  -3,0,  0,1,
   0,1,  3,0,

  9999
};

