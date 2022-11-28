#include <iostream>
#include <memory>
#include "scene.h"
#include "sceneobject.h"

// TODO set the test according to your current exercise.
const static bool TEST_RAY_GENERATION = false;
const static bool TEST_SPHERE_INTERSECT = true;

// random number generation seed
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
    // TODO 2.1.3
    // Check all objects if they got hit by the traced ray. (cf. lecture slide 44)
    // If any object got hit, return the one closest to the camera as 'hitObject'.
    // END TODO
    ///////////
    double currentNearest = 2147483647;

    for (unsigned i=0; i < objects.size(); i++) {
        double old_t_near = t_near;
        if((*objects[i]).intersect(ray,t_near)){
            //std::cout << "HIT" << std::endl;
            if(currentNearest > t_near) {
                currentNearest = t_near;
                hitObject = objects[i];
            } else {
                t_near = old_t_near;
            }
        };
    }
    //std::cout << "NOT HIT" << std::endl;
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
    // set the background color as dark blue
    Vec3d hitColor(0, 0, 0.2);

    ///////////
    // TODO: 2.1.2
    // Trace the ray by calling 'trace(...)'. If an object gets hit, calculate the hit point
    // and retrieve the surface color 'hitColor' from the 'hitObject'.
    // cf. lecture slide 44
    // END TODO
    ///////////
    double t_near;
    std::shared_ptr<SceneObject> hitObject;
    //std::cout << ray.dir << std::endl;
    if(trace(ray,objects,t_near,hitObject)){
        //calculate hit point
        Vec3d surface_point = t_near * ray.dir;
        //std::cout << surface_point << std::endl;
        hitColor = (*hitObject).getSurfaceColor(surface_point);
        //std::cout << hitColor << std::endl;
    }
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

    // camera position in world coordinates (in origin)
    const Vec3d cameraPos(0., 0., 0.);
    // view plane parameters (FoV: ~53.13°)
    const double l = -1.;   // left
    const double r = +1.;   // right
    const double b = -1.;   // bottom
    const double t = +1.;   // top
    const double d = +2.;   // distance to camera

    ///////////
    // TODO: 2.1.1 (cf. slides 39-43)
    // Cast a ray from 'cameraPos' through the center(!) of each pixel on the viewplane.
    // Use the view plane parametrization given above (l,r,b,t,d).
    //
    // NOTE:
    // - It's a right handed coordinate system.
    // - The camera looks in negative z direction, the up-vector is (0,1,0) (positive y direction).
    // - The texture coordinates run from left to right and from top to bottom!
    //
    // END TODO
    ///////////
    Vec3d up = Vec3d(0,1,0);
    Vec3d w = Vec3d(0,0,1);
    Vec3d u = up.cross(w);
    Vec3d v = w.cross(u);

    //Vec3d w = (cameraPos - )
    std::cout << framebuffer.size() << std::endl;

    for(int y = 0; y < HEIGHT; y++ ) {
        for(int x = 0; x < WIDTH; x++ ) {
            double scalar_u = l + (r-l) * (x+0.5) / WIDTH;
            double scalar_v = t + (b-t) * (y+0.5) / HEIGHT;
            //std::cout << scalar_u << std::endl;

            Vec3d s = scalar_u * u + scalar_v * v - d * w;

            Ray shoot = Ray();
            shoot.origin = cameraPos;
            shoot.dir = s;


            framebuffer[y*WIDTH + x] = (castRay(shoot,objects));
        }
    }
    std::cout << framebuffer.size() << std::endl;

    // save the framebuffer an a PPM image
    saveAsPPM("./result.ppm", viewport, framebuffer);

    // Compare the resulting image to the reference images.
    // Enable the test according to your current exercise.
    // You may need to adapt the given path according to your build setup!
    if (TEST_RAY_GENERATION)
    {
        // Check your ray generation and setup against the reference.
        comparePPM("../reference_rayGeneration.ppm",
                   "ray generation test", framebuffer);
    }
    else if (TEST_SPHERE_INTERSECT)
    {
        // Check your ray-sphere intersection against the reference.
        comparePPM("../reference_sphereIntersection.ppm",
                   "sphere intersection test", framebuffer);
    }
}

/**
 * @brief main routine.
 *        Generates the scene and invokes the rendering.
 * @return
 */
int main()
{
 /*   std::vector<std::shared_ptr<SceneObject>> objects;

    // random number generation
    std::mt19937 mtGen(SEED);
    std::uniform_real_distribution<double> distrib(-0.5, 0.5);

    // generate a scene containing one plane and a bunch of pseudo randomly distributed spheres
    Vec3d planeNormal(0., 1., 0.);
    planeNormal.normalize();
    std::shared_ptr<SceneObject> plane = std::make_shared<Plane>(Vec3d(0., -1., 5.), planeNormal);
    objects.push_back(std::shared_ptr<SceneObject>(plane));

    const int numSpheres = 32;
    for (int i = 0; i < numSpheres; ++i)
    {
        const Vec3d randPos(distrib(mtGen)*10., distrib(mtGen)*10., -12. + distrib(mtGen)*-10.);
        const double randRadius = (0.5 + distrib(mtGen));
        objects.push_back(std::shared_ptr<SceneObject>(std::make_shared<Sphere>(randPos, randRadius)));
    }

    const Vec3i viewport(WIDTH, HEIGHT, 0);
    render(viewport, objects);

    return 0;*/
        // Generate the scene
    const auto objects = create_scene();

    // Start rendering
    const Vec3i viewport(WIDTH, HEIGHT, 0);

    render(viewport, objects);

    return 0;

}
