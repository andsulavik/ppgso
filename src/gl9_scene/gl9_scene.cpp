// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Asteroid and Space objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>

#include "camera.h"
#include "scene.h"
#include "generator.h"
#include "player.h"
#include "space.h"
#include "road.h"
#include "Monster.h"
#include "Pipe.h"

const unsigned int SIZE = 512;

/*!
 * Custom windows for our simple game
 */

class SceneWindow : public ppgso::Window {
private:
  Scene scene, scene2;
  bool animate = true;

  /*!
   * Reset and initialize the game scene
   * Creating unique smart pointers to objects that are stored in the scene object list
   */
  void initScene() {
    scene.objects.clear();

    // Create a camera
    auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 600.0f);
    camera->position.z = -200.0f;
    camera->position.y = 70.0f;
    scene.camera = move(camera);

    // Add space background
    scene.objects.push_back(std::make_unique<Space>());

    // Add generator to scene
   // auto generator = std::make_unique<Generator>();
   // generator->position.y = 10.0f;
   // scene.objects.push_back(move(generator));

    // Add player to the scene
    auto player = std::make_unique<Player>();
    player->position.y = -4.5f;
    player->increase = true;
    auto sh = std::make_unique<Shadow>();
    sh->position.x = player->position.x ;
    sh->position.y = player->position.y;
    sh->rotation.z = -4.6f;
    sh->rotation.x = 1.6f;
    player->shadow = move(sh);
    scene.objects.push_back(move(player));

    auto road1 = std::make_unique<Road>();
    road1->position.x = 1.f;
    road1->position.y = -6.f;
    scene.objects.push_back(move(road1));

      auto road2 = std::make_unique<Road>();
      road2->position.y = 84.f;
      road2->position.x = 90;
      scene.objects.push_back(move(road2));

      auto road3 = std::make_unique<Road>();
      road3->position.y = 25.f;
      road3->position.x = -80;
      auto monster3 = std::make_unique<Monster>();
      monster3->position.x = road3->position.x + rand() % 24 -24;
      road3->monster = move(monster3);
      scene.objects.push_back(move(road3));

      auto road4 = std::make_unique<Road>();
      road4->position.y = 60.f;
      road4->position.x = 6;
      scene.objects.push_back(move(road4));

      auto road5 = std::make_unique<Road>();
      road5->position.y = 116.f;
      road5->position.x = 6;
      auto pipe1 = std::make_unique<Pipe>();
      auto spikes = std::make_unique<cactus>();
      pipe1->position.x = road5->position.x - 15;
      pipe1->position.y = road5->position.y + 22;
      spikes->position.x = pipe1->position.x + 5.2f;
      spikes->position.y = pipe1->position.y + 10.f;
      pipe1->cactus = move(spikes);
      road5->pipe = move(pipe1);
      scene.objects.push_back(move(road5));

      auto road6 = std::make_unique<Road>();
      road6->position.y = 146.f;
      road6->position.x = -80;
      auto monster2 = std::make_unique<Monster>();
      monster2->position.x = road6->position.x + rand() % 24 -24;
      road6->monster = move(monster2);
      scene.objects.push_back(move(road6));

      auto road7 = std::make_unique<Road>();
      road7->position.y = 146.f;
      road7->position.x = -120;
      scene.objects.push_back(move(road7));

      auto road67 = std::make_unique<Road>();
      road67->position.y = 146.f;
      road67->position.x = -160;
      auto monster1 = std::make_unique<Monster>();
      monster1->position.x = road67->position.x + rand() % 24 -24;
      road67->monster = move(monster1);
      scene.objects.push_back(move(road67));

      auto road8 = std::make_unique<Road>();
      road8->position.y = 146.f;
      road8->position.x = -200;
      scene.objects.push_back(move(road8));

      auto road9 = std::make_unique<Road>();
      road9->position.y = 60.f;
      road9->position.x = -300;
      scene.objects.push_back(move(road9));

      // Add generator to scene
      auto generator = std::make_unique<Generator>();
      generator->position.y = 10.0f;
      scene.objects.push_back(move(generator));

  }

    void initScene2() {
        scene2.objects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 600.0f);
        camera->position.z = -200.0f;
        camera->position.y = 70.0f;
        scene2.camera = move(camera);

        // Add space background
        scene2.objects.push_back(std::make_unique<Space>());

        // auto generator = std::make_unique<Generator>();
        // generator->position.y = 10.0f;
        // scene.objects.push_back(move(generator));

        // Add player to the scene
  /*      if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        if (!texture) {
            texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sphere.bmp"));
        }
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cube.obj");"""*/

    }

public:
  /*!
   * Construct custom game window
   */
  SceneWindow() : Window{"gl9_scene", SIZE, SIZE} {
    //hideCursor();
    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

    // Initialize OpenGL state
    // Enable Z-buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Enable polygon culling
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    initScene();
    initScene2();

  }

  /*!
   * Handles pressed key when the window is focused
   * @param key Key code of the key being pressed/released
   * @param scanCode Scan code of the key being pressed/released
   * @param action Action indicating the key state change
   * @param mods Additional modifiers to consider
   */
  void onKey(int key, int scanCode, int action, int mods) override {
    scene.keyboard[key] = action;

    // Reset
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        Space::texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("bgr.bmp"));
        initScene();
        initScene2();
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        scene.intro = false;
        scene.camera->position.z = -200.0f;
        scene.camera->position.y = 70.0f;
        scene.camera->position.x = 0;
    }

    // Pause
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
      animate = !animate;
    }
  }

  /*!
   * Handle cursor position changes
   * @param cursorX Mouse horizontal position in window coordinates
   * @param cursorY Mouse vertical position in window coordinates
   */
  void onCursorPos(double cursorX, double cursorY) override {
    scene.cursor.x = cursorX;
    scene.cursor.y = cursorY;
  }

  /*!
   * Handle cursor buttons
   * @param button Mouse button being manipulated
   * @param action Mouse bu
   * @param mods
   */
  void onMouseButton(int button, int action, int mods) override {
    if(button == GLFW_MOUSE_BUTTON_LEFT) {
      scene.cursor.left = action == GLFW_PRESS;

      if (scene.cursor.left) {
        // Convert pixel coordinates to Screen coordinates
        double u = (scene.cursor.x / width - 0.5f) * 2.0f;
        double v = - (scene.cursor.y / height - 0.5f) * 2.0f;

        // Get mouse pick vector in world coordinates
        auto direction = scene.camera->cast(u, v);
        auto position = scene.camera->position;

        // Get all objects in scene intersected by ray
        auto picked = scene.intersect(position, direction);

        // Go through all objects that have been picked
        for (auto &obj: picked) {
          // Pass on the click event
          obj->onClick(scene);
        }
      }
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT) {
      scene.cursor.right = action == GLFW_PRESS;
    }
  }

  /*!
   * Window update implementation that will be called automatically from pollEvents
   */
  void onIdle() override {
    // Track time
    static auto time = (float) glfwGetTime();

    // Compute time delta
    float dt = animate ? (float) glfwGetTime() - time : 0;

    time = (float) glfwGetTime();

    // Set gray background
    glClearColor(.5f, .5f, .5f, 0);
    // Clear depth and color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update and render all objects
    scene.update(dt);
    if(scene.playR == 0){
        scene.render();
    }
    else if(scene.playR == 1){
        scene2.update(dt);
        Space::texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("winner.bmp"));
        scene2.render();
    }
    else {
        scene2.update(dt);
        Space::texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("gameover.bmp"));
        scene2.render();
    }
  }
};

int main() {
  // Initialize our window
  SceneWindow window;

  // Main execution loop
  while (window.pollEvents()) {}

  return EXIT_SUCCESS;
}
