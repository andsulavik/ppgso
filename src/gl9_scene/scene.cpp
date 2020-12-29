#include "scene.h"
#include "player.h"

class Renderable {
public:
    // Virtual destructor is needed for abstract interfaces
    virtual ~Renderable() = default;

    /// Render the object
    /// \param camera - Camera to use for rendering
    virtual void render(const Camera& camera) = 0;

    /// Update the object. Useful for specifing animation and behaviour.
    /// \param dTime - Time delta
    /// \param scene - Scene reference
    /// \return - Return true to keep object in scene
    virtual bool update(float dTime, Scene &scene) = 0;
};
void Scene::update(float time) {
    if(intro){
        if(pom <= 1){
            pom += 0.01;
        }
        else{
            pom = 0.f;
            position += 1;
            if(position == 6){
                intro = false;
            }
        }
        if(position!=5) {
            camera->position = glm::lerp(CamPositions[position], CamPositions[position + 1], pom);
        }
    }
    camera->update();


    // Use iterator to update all objects so we can remove while iterating
  auto i = std::begin(objects);
  playR = 2;
  while (i != std::end(objects)) {
    // Update and remove from list if needed
    auto obj = i->get();
    auto player = dynamic_cast<Player*>(obj);
    if ( player ) {
        playR = 0;
    }
    if( player and player->closestRd->position.x == -300 and player->position.y >= 60.f and  player->position.y <= 69.f and player->position.x >= -324 and player->position.x <= -276)
        playR = 1;
    if (!obj->update(*this, time))
        i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
    else
      ++i;
  }
}

void Scene::render() {
  // Simply render all objects
  for ( auto& obj : objects )
    obj->render(*this);
}

std::vector<Object*> Scene::intersect(const glm::vec3 &position, const glm::vec3 &direction) {
  std::vector<Object*> intersected = {};
  for(auto& object : objects) {
    // Collision with sphere of size object->scale.x
    auto oc = position - object->position;
    auto radius = object->scale.x;
    auto a = glm::dot(direction, direction);
    auto b = glm::dot(oc, direction);
    auto c = glm::dot(oc, oc) - radius * radius;
    auto dis = b * b - a * c;

    if (dis > 0) {
      auto e = sqrt(dis);
      auto t = (-b - e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }

      t = (-b + e) / a;

      if ( t > 0 ) {
        intersected.push_back(object.get());
        continue;
      }
    }
  }

  return intersected;
}
