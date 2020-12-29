#include "road.h"
#include "scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Road::mesh;
std::unique_ptr<ppgso::Texture> Road::texture;
std::unique_ptr<ppgso::Shader> Road::shader;

Road::Road() {
    // Scale the default model
    scale *= 10.0f;
    scale.x = 50.0f;
    origX = position.x;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) {
        texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sphere.bmp"));
    }
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cube.obj");
}

bool Road::update(Scene &scene, float dt) {
    if(monster != nullptr){
        if(monster->position.x >= position.x-24 and !left){
            monster->rotation.z = 4.8f;
            monster->position.x -= 0.48f;
        }
        else if(monster->position.x <= position.x +24 and left){
            monster->rotation.z = -4.8f;
            monster->position.x += 0.48f;
        }
        else{
            if(left){
                left = false;
            }
            else{
                left = true;
            }
        }
        monster->position.y = position.y + 4.5f;
        monster->update(scene, dt);
    }
    if(pipe != nullptr){
        if(position.x >= origX + 20.f){
            right = true;
        }
        else if(position.x <= origX -20.f){
            right = false;
        }
        if(right){
            position.x -= 0.6f;
        }
        else{
            position.x += 0.6f;
        }
        pipe->posRoad = position.x;
        pipe->update(scene, dt);
    }
    generateModelMatrix();
    return true;
}

void Road::render(Scene &scene) {
    shader->use();
    if(monster != nullptr){
        monster->render(scene);
    }
    if(pipe != nullptr){
        pipe->render(scene);
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

void Road::onClick(Scene &scene) {
    std::cout << "Player has been clicked!" << std::endl;
}
