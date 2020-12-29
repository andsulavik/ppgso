#include "player.h"
#include "scene.h"
#include "road.h"
#include "Pipe.h"
#include "Monster.h"
#include "shadow.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Player::mesh;
std::unique_ptr<ppgso::Texture> Player::texture;
std::unique_ptr<ppgso::Shader> Player::shader;

Player::Player() {
  // Scale the default model
  scale *= 3.0f;
  position.y = 4.0f;
  // Initialize static resources if needed
  if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
  if (!texture) {
      texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Toot_Braustein.bmp"));
  }
  if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Toot_Braustein.obj");
}
bool decrease, increase = true, death = false, intangibleL = true, intangibleR = true, monsterKill = false, pipe = false;
float dist1 = 0, dist2 = 0;

bool Player::update(Scene &scene, float dt) {
  // Fire delay increment
  fireDelay += dt;
  float closestDist = 0;
  for (auto &obj : scene.objects) {
    // Ignore self in scene
    if (obj.get() == this) continue;

    // We only need to collide with asteroids and projectiles, ignore other objects
    auto road = dynamic_cast<Road*>(obj.get()); // dynamic_pointer_cast<Road>(obj);
    auto pipe = dynamic_cast<Pipe*>(obj.get());
    auto monster = dynamic_cast<Monster*>(obj.get());
    if (!road and !pipe and !monster) continue;

    // When colliding with other asteroids make sure the object is older than .5s
    // This prevents excessive collisions when asteroids explode.
    //   if (asteroid && age < 0.5f) continue;

    // Compare distance to approximate size of the asteroid estimated from scale.y
    if(closestDist != 0 and closestDist > distance(position, obj->position)){
        closestRd = road;
        closestDist = distance(position, obj->position);
    }
    else if(closestDist == 0){
        closestRd = road;
        closestDist = distance(position, obj->position);
    }
  }
  if((closestRd->monster != nullptr) and !monsterKill and ((closestRd->monster->position.x + 5.5f >= position.x and closestRd->monster->position.x <= position.x) or (closestRd->monster->position.x - 5.5f <= position.x and closestRd->monster->position.x >= position.x)) and abs(abs(closestRd->monster->position.y)-abs(position.y)) < 9.5f){
      position.y += 10.6f;
      monsterKill = true;
  }
  if((position.y <= closestRd->position.y + 4.f and position.y >= closestRd->position.y-17.f) and (position.x < closestRd->position.x-2.f or position.x > closestRd->position.x+2.f)){
      intangibleL = true;
      intangibleR = true;
      if(position.x > closestRd->position.x - 30.f and (position.x < closestRd->position.x)){
        intangibleR = false;
        intangibleL = true;
    }
    else if(position.x < closestRd->position.x + 30.f and (position.x > closestRd->position.x)){
        intangibleL = false;
        intangibleR = true;
    }

  }
  else{
    intangibleL = true;
    intangibleR = true;
  }
  if (glm::length(closestRd->position.x - position.x) < (closestRd->scale.x + scale.x)*0.6 and distance(position, closestRd->position) < (closestRd->scale.y + scale.y) *2.f ) {
    death = false;
  }
  else{
    death = true;
  }
  if(monsterKill == true){
      position.y -= 1.6f;
      rotation.x -= 0.05f;
      rotation.z -= 0.005f;
      rotation.y -= 0.005f;
      scene.camera->position.y -= 1.6f;
      if(position.y <= -50){
          monsterKill = false;
          return false;
      }
  }
  else {
      if(closestRd->pipe != nullptr){
          if(closestRd->right){
              position.x -= 0.6f;
              scene.camera->position.x -= 0.6f;
          }
          else{
              position.x += 0.6f;
              scene.camera->position.x += 0.6f;
          }
      }
      if (position.y >= dist1 + 45.f) {
          increase = false;
          decrease = true;
      }
      if (increase) {
          position.y += 1.6f;
          scene.camera->position.y += 1.6f;
      } else if (decrease or (decrease and increase)) {
          if (closestRd->pipe != nullptr and position.x <= closestRd->pipe->position.x + 11.f and position.x >= closestRd->pipe->position.x - 11.f and position.y <= closestRd->pipe->position.y + 1.f and position.y >= closestRd->pipe->position.y - 0.5f) {
              pipe = true;
              if(closestRd->pipe->cactus->position.y > closestRd->pipe->position.y +1.f){
                  monsterKill = true;
                  pipe = death = increase = decrease = false;
              }
          }
          else{
              pipe = false;
              position.y -= 1.6f;
              scene.camera->position.y -= 1.6f;
          }
      } else if (death) {
          increase = false;
          decrease = true;
          position.y -= 1.6f;
          scene.camera->position.y -= 1.6f;
      }
      if ((position.y >= closestRd->position.y + 4.f and position.y <= closestRd->position.y + 4.8f) and !death) {
          decrease = increase = false;
      }
      if (position.y <= -50 and death) {
          death = false;
          increase = true;
          return false;
      }
      if (rotation.z == NULL) {
          //rotation.z = 4.4f;
          rotation.z = 9;
      }

      // Keyboard controls
      if (scene.keyboard[GLFW_KEY_LEFT]) {
          if (increase or decrease) {
              if (intangibleR) {
                  position.x += 10 * 8 * dt;
                  scene.camera->position.x += 10 * 8 * dt;
              }
          } else {
              if (intangibleR) {
                  position.x += 10 * 6 * dt;
                  scene.camera->position.x += 10 * 6 * dt;
              }
          }
          rotation.z = -4.8f;
          if (scene.keyboard[GLFW_KEY_SPACE] and !death and !increase and !decrease or (scene.keyboard[GLFW_KEY_SPACE] and !increase and pipe)) {
              if(pipe){
                  dist1 = closestRd->pipe->position.y;
              }
              else{
                  dist1 = closestRd->position.y;
              }
              increase = true;
          }
      } else if (scene.keyboard[GLFW_KEY_RIGHT]) {
          if (increase or decrease) {
              if (intangibleL) {
                  position.x -= 10 * 8 * dt;
                  scene.camera->position.x -= 10 * 8 * dt;
              }
          } else {
              if (intangibleL) {
                  position.x -= 10 * 6 * dt;
                  scene.camera->position.x -= 10 * 6 * dt;
              }
          }
          rotation.z = 4.8f;
          if (scene.keyboard[GLFW_KEY_SPACE] and !death and !increase and !decrease or (scene.keyboard[GLFW_KEY_SPACE] and !increase and pipe)) {
              increase = true;
              if(pipe){
                  dist1 = closestRd->pipe->position.y;
              }
              else{
                  dist1 = closestRd->position.y;
              }
          }
      } else if (scene.keyboard[GLFW_KEY_SPACE] and !death and !increase and !decrease or (scene.keyboard[GLFW_KEY_SPACE] and pipe)) {
          increase = true;
          if(pipe){
              dist1 = closestRd->pipe->position.y;
          }
          else{
              dist1 = closestRd->position.y;
          }

      }
  }
    if(shadow != nullptr){

        shadow->posPlayerX = position.x;
        shadow->posPlayerY = closestRd->position.y+5.f;
        shadow->update(scene, dt);
    }


  generateModelMatrix();
  return true;
}

void Player::render(Scene &scene) {
  shader->use();
    if(shadow != nullptr and !death ){
        if(position.x <= closestRd->position.x + 10.5f){
            shadow->render(scene);
        }
    }

  // Set up light
  shader->setUniform("LightDirection", scene.lightDirection);

  // use camera
  shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
  shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

  // render mesh
  shader->setUniform("ModelMatrix", modelMatrix);
  shader->setUniform("Texture", *texture);
  mesh->render();
}

void Player::onClick(Scene &scene) {
  std::cout << "Player has been clicked!" << std::endl;
}
