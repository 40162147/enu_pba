#include <glm/glm.hpp>
#include "main.h"
using namespace std;
using namespace glm;

static dvec3 gravity = dvec3(0, -10.0, 0);
static dvec3 wind = dvec3(1.5, 0, 0);

void UpdatePhysics_Verlet(const double t, const double dt) {
  for (size_t i = 0; i < balls.size(); i++)
  {
    // *********************************
	  dvec3 acc = (gravity += wind);
    // calcualte velocity from current and previous position
	  balls[i].velocity = balls[i].position - balls[i].position_prev;
    // set previous position to current position
	  balls[i].position_prev = balls[i].position;
	  balls[i].position += balls[i].velocity + acc * dt * dt;

    // *********************************

    if (balls[i].position.y <= 0.0f) {
      //Note: We can't use: 
      // balls[i].velocity.y = -balls[i].velocity.y;
      // As verlet calculates it's own velocity, instead we must use impulses.
      // but our simple model can't support that yet, so let's just hack it
      balls[i].position_prev = balls[i].position + (balls[i].position - balls[i].position_prev);
    }
  }
}