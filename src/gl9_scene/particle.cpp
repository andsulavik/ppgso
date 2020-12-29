#include <glm/gtc/random.hpp>
#include "particle.h"
#include "player.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>


// Static resources
std::unique_ptr<ppgso::Mesh> Particle::mesh;
std::unique_ptr<ppgso::Texture> Particle::texture;
std::unique_ptr<ppgso::Shader> Particle::shader;
float windL = -0.5, windR = 5, ca;
int x ,y;
Particle::Particle() {
    // Set random scale speed and rotation
    scale *= glm::linearRand(1.0f, 1.5f);
    speed = {glm::linearRand(-.5f, .5f), -15.f, glm::linearRand(-.5f, .5f)};
    rotation = {0, 0, 0};
    rotMomentum = {0, 0, 0};

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("cactus.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");
}
bool Particle::update(Scene &scene, float dt) {
    // Count time alive
    age += dt;

    // Animate position according to time
    position += speed * dt;
    position.x += glm::linearRand(-5.f, 5.f)/25.f;
    position.z += glm::linearRand(-5.f, 5.f)/20.f;
    position.y += glm::linearRand(-5.f, 5.f)/30.f;

    // Rotate the object
    rotation += rotMomentum * dt;

    // Delete when alive longer than 10s or out of visibility
    if (position.y < -10) return false;
    // Collide with scene
    for (auto &obj : scene.objects) {
        // Ignore self in scene
        if (obj.get() == this) continue;

        // We only need to collide with asteroids and projectiles, ignore other objects
        auto asteroid = dynamic_cast<Particle*>(obj.get()); // dynamic_pointer_cast<Asteroid>(obj);
        auto pl = dynamic_cast<Player*>(obj.get());
        if (!asteroid) continue;
        if (ca < 1) {
            ca += 0.00001*dt;
        } else {
            ca = 0;
            windL = (rand() % 11 + (-5))/10.f;
            windR = windL*(-1);
            printf("%f %f\n", windL, windR);
        }
        asteroid->position.x += glm::lerp(windL, windR, ca)*dt;
        // When colliding with other asteroids make sure the object is older than .5s
        // This prevents excessive collisions when asteroids explode.
        if (asteroid && age < 0.5f) continue;

        // Compare distance to approximate size of the asteroid estimated from scale.
        if (distance(position, obj->position) < (obj->scale.y + scale.y) * 0.7f) {
            // Destroy self
            return false;
        }
    }

    // Generate modelMatrix from position, rotation and scale
    generateModelMatrix();

    return true;
}

void Particle::render(Scene &scene) {
    shader->use();

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

void Particle::onClick(Scene &scene) {
    std::cout << "Asteroid clicked!" << std::endl;
    age = 10000;
}