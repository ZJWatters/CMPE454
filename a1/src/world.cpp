// world.cpp


#include "world.h"
#include "ship.h"
#include "main.h"
#include "gpuProgram.h"
#include "strokefont.h"
#include "Sheild.h"
#include "lives.h"

#include <sstream>


void World::start()

{
    // Create a ship at the centre of the world

    ship = new Ship(0.5 * (worldMin + worldMax));

    // Create a sheild somewhere in the world

    //sheild = new Sheild(randIn01() * (worldMin + worldMax));

    // Create some large asteroids

    // Pick a random position at least 20% away from the origin (which
    // is where the ship will be).

    asteroids.clear();
    shells.clear();

    for (int i = 0; i < NUM_INITIAL_ASTEROIDS; i++) {

        vec3 pos;
        do {
            pos = vec3(randIn01(), randIn01(), 0);
        } while ((pos - vec3(0.5, 0.5, 0)).length() < 0.20);

        asteroids.push_back(new Asteroid(pos % (worldMax - worldMin) + worldMin));
    }

    // Increase asteroid speed in later rounds

    for (unsigned int i = 0; i < asteroids.size(); i++) {
        asteroids[i]->velocity = ((1 + round) * ASTEROID_SPEED_ROUND_FACTOR) * asteroids[i]->velocity;
        asteroids[i]->angularVelocity = ((1 + round) * ASTEROID_SPEED_ROUND_FACTOR) * asteroids[i]->angularVelocity;
    }

    state = RUNNING;
}


void World::updateState(float elapsedTime)

{
    if (state == PAUSED)
        return;

    if (asteroids.size() == 0) {
        round++;
        start();
        return;
    }

    // See if any keys are pressed for thrust

    if (state == RUNNING) {

        if (rightKey == DOWN)
            ship->rotateCW(elapsedTime);

        if (leftKey == DOWN)
            ship->rotateCCW(elapsedTime);

        if (upKey == DOWN)
            ship->addThrust(elapsedTime);
    }

    // Update the ship

    ship->updatePose(elapsedTime);

    // Update the asteroids (check for asteroid collisions)

    for (unsigned int i = 0; i < asteroids.size(); i++) {
        asteroids[i]->updatePose(elapsedTime);
        if (state == RUNNING && ship->intersects(*asteroids[i]))
            gameOver();
    }

  // Update the sheild (check for asteroid intersections)

    /*if (state == RUNNING && ship->intersects(*sheild)) {
        sheild->attach(elapsedTime);
        for (unsigned int i = 0; i < asteroids.size(); i++) {
            if (sheild->intersects(*asteroids[i])) {
                asteroids.erase(asteroids.begin() + i);
                sheild->destroy();
            }
        }
    }*/

    // Update the shells (check for asteroid collisions)

    for (unsigned int i = 0; i < shells.size(); i++) {
        shells[i]->elapsedTime += elapsedTime;

        if (shells[i]->elapsedTime > SHELL_MAX_TIME) { // remove an old shell

            shells.erase(shells.begin() + i);
            i--;

        }
        else { // move a not-too-old shell

            vec3 prevPosition = shells[i]->centrePosition();
            shells[i]->updatePose(elapsedTime);

            // Check for shell/asteroid collision

            Segment path(prevPosition, shells[i]->centrePosition());

            // YOUR CODE HERE
            //
            // Check each of the asteroids to see if it has intersected the
            // shell's path.  Be sure to handle cases where the shell was on
            // one side of the asteroid the last time you checked, but on
            // the OTHER side this time.  If so, either (a) remove the
            // asteroid if it is too small or (b) break the asteroids into
            // two.  Also increment the score according to the asteroid's
            // scoreValue.
            //
            // - An asteroid is removed if (asteroids->scaleFactor * ASTEROID_SCALE_FACTOR_REDUCTION < MIN_ASTEROID_SCALE_FACTOR).
            //
            // - A split asteroid should add velocities to the two
            //   sub-asteroids in opposite directions perpendicular to the
            //   direction of the shell.
            //
            // - the sub-asteroid scaleFactor and scoreValue should be
            //   modified from those of the parent asteroid.

            // check collision
            int newAsteroids = 0;

            for (unsigned int j = 0; j < asteroids.size() - newAsteroids; j++) {
                if (state == RUNNING && asteroids[j]->intersects(path)) {
                    // remove asteroid if its to small and update the score
                    if (asteroids[j]->scaleFactor * ASTEROID_SCALE_FACTOR_REDUCTION < MIN_ASTEROID_SCALE_FACTOR) {
                        // remove the hit asteroid and update player score
                        score += asteroids[j]->scoreValue;
                        asteroids.erase(asteroids.begin() + j);
                        j--; 
                    }
                    else {
                        // break the asteroid into two new ones
                        Asteroid* asteroid1 = new Asteroid(asteroids[j]->position);
                        Asteroid* asteroid2 = new Asteroid(asteroids[j]->position);

                        // find the orientation angle of the two new steroids
                        float angle1 = shells[i]->orientation.angle() - (M_PI / 2); // 90 degrees CW of the shell angle
                        float angle2 = shells[i]->orientation.angle() + (M_PI / 2); // 90 degrees CCW of the shell angle

                        // find the orientation of the two new asteroids
                        quaternion ori1 = quaternion(angle1, vec3(0, 0, 1));
                        quaternion ori2 = quaternion(angle2, vec3(0, 0, 1));

                        //find the velocity of both
                        float newV = ASTEROID_SPEED*ASTEROID_SPEED_INCREASE;
                        vec3 vel1 = vec3(-newV * sin(angle1), newV * cos(angle1), 0);
                        vec3 vel2 = vec3(-newV * sin(angle2), newV * cos(angle2), 0);

                        // setting orientation and velocity of new asteroids
                        asteroid1->orientation = ori1;
                        asteroid2->orientation = ori2;
                        asteroid1->velocity = vel1;
                        asteroid2->velocity = vel2;

                        // modifiying asteroid scoreValue and scaleFactor
                        asteroid1->scoreValue /= 2;
                        asteroid2->scoreValue /= 2;
                        asteroid1->scaleFactor /= 3;
                        asteroid2->scaleFactor /= 3;

                        // add the two asteroids to the array and update the new asteroids
                        asteroids.push_back(asteroid1);
                        asteroids.push_back(asteroid2);
                        newAsteroids += 2;

                        // remove the hit asteroid and update player score
                        score += asteroids[j]->scoreValue;
                        asteroids.erase(asteroids.begin() + j);
                        j--;
                    }
                
                    // remove shell that has already collided
                    shells.erase(shells.begin() + i);
                }
            }
        }
    }
}


void World::draw()

{
    // Transform [worldMin,worldMax] to [(-1,-1),(+1,+1)].

    mat4 worldToViewTransform;

    worldToViewTransform
        = translate(-1, -1, 0)
        * scale(2.0 / (worldMax.x - worldMin.x), 2.0 / (worldMax.y - worldMin.y), 1)
        * translate(-worldMin.x, -worldMin.y, 0);

    // Draw all world elements, passing in the worldToViewTransform so
    // that they can append their own transforms before passing the
    // complete transform to the vertex shader.

    objectGPUProg->activate();

    ship->draw(worldToViewTransform, objectGPUProg);
    //sheild->draw(worldToViewTransform, objectGPUProg);

    for (unsigned int i = 0; i < shells.size(); i++)
        shells[i]->draw(worldToViewTransform, objectGPUProg);

    for (unsigned int i = 0; i < asteroids.size(); i++)
        asteroids[i]->draw(worldToViewTransform, objectGPUProg);

    objectGPUProg->deactivate();

    // Draw the title

    strokeFont->drawStrokeString("ASTEROIDS", 0, 0.85, 0.06, 0, CENTRE);

    // Draw messages according to game state

    if (state == BEFORE_GAME) {

        strokeFont->drawStrokeString("PRESS 's' TO START, 'p' TO PAUSE DURING GAME", 0, -.9, 0.06, 0, CENTRE);

    }
    else {

        // draw score

        stringstream ss;
        ss.setf(ios::fixed, ios::floatfield);
        ss.precision(1);
        ss << "SCORE " << score;
        strokeFont->drawStrokeString(ss.str(), -0.95, 0.75, 0.06, 0, LEFT);

        if (state == AFTER_GAME) {

            // Draw "game over" message

            strokeFont->drawStrokeString("GAME OVER", 0, 0, 0.12, 0, CENTRE);
            strokeFont->drawStrokeString("PRESS 's' TO START, 'p' TO PAUSE DURING GAME", 0, -0.9, 0.06, 0, CENTRE);
        }
    }
}


void World::gameOver()

{
    state = AFTER_GAME;
}


void World::togglePause()

{
    if (state == RUNNING)
        state = PAUSED;
    else if (state == PAUSED)
        state = RUNNING;
}
