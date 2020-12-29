//
// Created by Andrej on 12. 12. 2019.
//

#include "cactus.h"
#include "Pipe.h"
#include "scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> cactus::mesh;
std::unique_ptr<ppgso::Texture> cactus::texture;
std::unique_ptr<ppgso::Shader> cactus::shader;

cactus::cactus() {
    // Scale the default model
    scale *= 0.34f;
    scale.z = 0.1f;
    //scale.x = 105.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) {
        texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("cactus.bmp"));
    }
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("spikes.obj");
}

bool cactus::update(Scene &scene, float dt) {
    position.x = posPipe + 5.2f;
    generateModelMatrix();
    return true;
}

void cactus::render(Scene &scene) {
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

void cactus::onClick(Scene &scene) {
    std::cout << "Player has been clicked!" << std::endl;
}

