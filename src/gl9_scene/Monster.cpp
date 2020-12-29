//
// Created by Andrej on 12. 12. 2019.
//

#include "Monster.h"
#include "road.h"
#include "scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Monster::mesh;
std::unique_ptr<ppgso::Texture> Monster::texture;
std::unique_ptr<ppgso::Shader> Monster::shader;

Monster::Monster() {
    // Scale the default model
    scale *= 3.0f;
    //scale.x = 105.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) {
        texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("Ogre.bmp"));
    }
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Ogre.obj");
}

bool Monster::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Monster::render(Scene &scene) {
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

void Monster::onClick(Scene &scene) {
    std::cout << "Player has been clicked!" << std::endl;
}
