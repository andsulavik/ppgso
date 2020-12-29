#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "camera.h"

/*
 * Scene is an object that will aggregate all scene related data
 * Objects are stored in a list of objects
 * Keyboard and Mouse states are stored in a map and struct
 */
class Scene{
  public:
    /*!
     * Update all objects in the scene
     * @param time
     */
    int playR = 3;
    bool intro = true;
    void update(float time);
    float pom = 0;
    int position = 0;
    glm::vec3 CamPositions[7] = {{0,20.f,-200.f}, {0,20.f,-100.f}, {-80, 25.f, -100.f}, {6, 116.f, -100.f}, {-80, 146.f, -100.f}, {-300, 60.f, -100.f}, {0,70.f,-200.f}};
    /*!
     * Render all objects in the scene
     */
    void render();

    /*!
     * Pick objects using a ray
     * @param position - Position in the scene to pick object from
     * @param direction - Direction to pick objects from
     * @return Objects - Vector of pointers to intersected objects
     */
    std::vector<Object*> intersect(const glm::vec3 &position, const glm::vec3 &direction);

    // Camera object
    std::unique_ptr<Camera> camera;

    // All objects to be rendered in scene
    std::list< std::unique_ptr<Object> > objects;

    // Keyboard state
    std::map< int, int > keyboard;

    // Lights, in this case using only simple directional diffuse lighting
    glm::vec3 lightDirection{-1.0f, 1.0f, -1.0f};

    // Store cursor state
    struct {
      double x, y;
      bool left, right;
    } cursor;
};

#endif // _PPGSO_SCENE_H
