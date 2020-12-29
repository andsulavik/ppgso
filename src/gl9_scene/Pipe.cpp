//
// Created by Andrej on 12. 12. 2019.
//

#include "Pipe.h"
#include "road.h"
#include "scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Pipe::mesh;
std::unique_ptr<ppgso::Texture> Pipe::texture;
std::unique_ptr<ppgso::Shader> Pipe::shader;

Pipe::Pipe() {
    // Scale the default model
    scale *= 0.05f;
    scale.z = 0.05f;
    scale.x = 0.1f;
    //scale.x = 105.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) {
        texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("stars.bmp"));
    }
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Pipe.obj");
}
bool up = true;
float max ,min;
bool Pipe::update(Scene &scene, float dt) {
    position.x = posRoad-15;
    max = position.y + 10.f;
    min = position.y -7.f;
    if(cactus != nullptr){
        if(cac <= 1 and up){
            cac += 0.7*dt;
        }
        else if(!up and cac >= 0){
            cac -= 0.6*dt;
        }
        else{
            up = !up;
        }
        cactus->position.y = glm::lerp(max, min, cac);
        cactus->posPipe = position.x;
        cactus->update(scene, dt);
    }
    generateModelMatrix();
    return true;
}

void Pipe::render(Scene &scene) {
    shader->use();
    if(cactus != nullptr){
        cactus->render(scene);
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

void Pipe::onClick(Scene &scene) {
    std::cout << "Player has been clicked!" << std::endl;
}
