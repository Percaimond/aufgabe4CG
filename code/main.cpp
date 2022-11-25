#include "scene.h"
#include "sceneobject.h"
#include "util.h"
#include "vec3.h"

#include <limits>
#include <memory>
#include <vector>

// TODO: Set the test according to your current exercise.
const static bool TEST_RAY_GENERATION = false;
const static bool TEST_SPHERE_INTERSECT = false;

// Random number generation seed
const static int SEED = 42;

const static int WIDTH = 600;
const static int HEIGHT = 600;

/**
 * @brief Method to check a ray for intersections with any object of the scene.
 * @param ray The ray to trace.
 * @param objects Vector of pointers to all scene objects.
 * @param t_near The intersection distance from the ray origin to the closest point hit.
 * @param hitObject The closest object hit.
 * @return true on hit, false otherwise
 */
bool trace(const Ray &ray,
           const std::vector<std::shared_ptr<SceneObject>> &objects,
           double &t_near, std::shared_ptr<SceneObject> &hitObject)
{
    ///////////
    // TODO
    // Check all objects if they got hit by the traced ray.
    // If any object got hit, return the one closest to the camera as 'hitObject'.
    //
    // cf., lecture slide 49
    // END TODO
    ///////////

    return (hitObject != nullptr);
}

/**
 * @brief Cast a ray into the scene. If the ray hits at least one object,
 *        the color of the object closest to the camera is returned.
 * @param ray The ray that's being cast.
 * @param objects All scene objects.
 * @return The color of a hit object that is closest to the camera.
 *         Return dark blue if no object was hit.
 */
Vec3d castRay(const Ray &ray, const std::vector<std::shared_ptr<SceneObject>> &objects)
{
    // Set the background color as dark blue
    Vec3d hitColor(0.0, 0.0, 0.2);

    ///////////
    // TODO
    // Trace the ray by calling 'trace(...)'. If an object gets hit, calculate the hit point
    // and retrieve the surface color 'hitColor' from the 'hitObject'.
    //
    // cf., lecture slide 49
    // END TODO
    ///////////

    return hitColor;
}

/**
 * @brief The rendering method, loop over all pixels in the framebuffer, shooting
 *        a ray through each pixel with the origing being the camera position.
 * @param viewport Size of the framebuffer.
 * @param objects Vector of pointers to all objects contained in the scene.
 */
void render(const Vec3i viewport, const std::vector<std::shared_ptr<SceneObject>> &objects)
{
    std::vector<Vec3d> framebuffer(static_cast<size_t>(viewport[0] * viewport[1]));

    // Camera position in world coordinates (at the origin)
    const Vec3d cameraPos(0.0, 0.0, 0.0);

    // View plane parameters (FoV: ~53.13°)
    const double l = -1.0;   // left
    const double r = +1.0;   // right
    const double b = -1.0;   // bottom
    const double t = +1.0;   // top
    const double d = +2.0;   // distance to camera

    ///////////
    // TODO
    // Cast a ray from 'cameraPos' through the center(!) of each pixel on the view plane.
    // Use the view plane parametrization given above (l,r,b,t,d).
    //
    // NOTE:
    // - It's a right handed coordinate system.
    // - The camera looks in negative z direction, the up-vector is (0,1,0) (positive y direction).
    // - The texture coordinates run from left to right and from top to bottom!
    //
    // cf., lecture slides 37ff
    // END TODO
    ///////////

    // Save the framebuffer an a PPM image
    saveAsPPM("./result.ppm", viewport, framebuffer);

    // Compare the resulting image to the reference images.
    // Enable the test according to your current exercise.
    // You may need to adapt the given path according to your build setup!
    if (TEST_RAY_GENERATION)
    {
        // Check your ray generation and setup against the reference.
        comparePPM("../reference_rayGeneration.ppm", "ray generation test", framebuffer);
    }
    else if (TEST_SPHERE_INTERSECT)
    {
        // Check your ray-sphere intersection against the reference.
        comparePPM("../reference_sphereIntersection.ppm", "sphere intersection test", framebuffer);
    }
}

/**
 * @brief main routine.
 *        Generates the scene and invokes the rendering.
 * @return
 */
int main()
{
    // Generate the scene
    const auto objects = create_scene();

    // Start rendering
    const Vec3i viewport(WIDTH, HEIGHT, 0);

    render(viewport, objects);

    return 0;
}
