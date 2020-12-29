//
// Created by Andrej on 12. 12. 2019.
//

#include "shadow.h"
#include "player.h"
#include "scene.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Shadow::mesh;
std::unique_ptr<ppgso::Texture> Shadow::texture;
std::unique_ptr<ppgso::Shader> Shadow::shader;

Shadow::Shadow() {
    // Scale the default model
    scale *= 3.05f;
    scale.z = 0;
    //scale.x = 105.0f;

    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) {
        texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("cactus.bmp"));
    }
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("Toot_Braustein.obj");
}
bool Shadow::update(Scene &scene, float dt) {
    position.x = posPlayerX;
    position.y = posPlayerY +1;
    generateModelMatrix();
    return true;
}

void Shadow::render(Scene &scene) {
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

void Shadow::onClick(Scene &scene) {
    std::cout << "Player has been clicked!" << std::endl;
}
