#include <glm/gtc/random.hpp>
#include <ppgso/ppgso.h>

#include "generator.h"
#include "particle.h"

bool Generator::update(Scene &scene, float dt) {
  // Accumulate time
  time += dt;
  // Add object to scene when time reaches certain level
    if (time > .1) {
        auto obj = std::make_unique<Particle>();
        obj->position = position;
        obj->position.y += glm::linearRand(200.0f, 250.0f);
        obj->position.x += glm::linearRand(-500.0f, 500.f);
        scene.objects.push_back(move(obj));
        time = 0;
    }

  return true;
}

void Generator::render(Scene &scene) {
  // Generator will not be rendered
}
