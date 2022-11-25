#pragma once

#include "util.h"
#include "vec3.h"

/**
 * @brief The SceneObject class.
 */
class SceneObject
{
public:
    /**
     * @brief Construct a white scene object.
     */
    SceneObject();

    /**
     * @brief Construct a scene object with user-defined color.
     * @param color Color of the object, if it is not overwritten by the derived class.
     */
    SceneObject(const Vec3d& color);

    /**
     * @brief Destructor
     */
    virtual ~SceneObject() {}

    /**
     * @brief Pure virtual method to compute the intersection of the scene object with a ray.
     * @param ray The ray to check for intersection.
     * @param t Distance on the ray of the intersection.
     * @return true on intersection, false otherwise.
     */
    virtual bool intersect(const Ray &ray, double &t) const = 0;

    /**
     * @brief Pure virtual method to get the surface color of the SceneObject.
     * @param p_hit The point on the surface that was hit.
     * @return The surface color of the SceneObject.
     */
    virtual Vec3d getSurfaceColor(const Vec3d &p_hit) const = 0;

protected:
    Vec3d _color;   //< color of the scene object, RGB [0,1]
};


/**
 * @brief The Plane class.
 *        A plane is represented by a point on the plane and a normal.
 */
class Plane : public SceneObject
{
public:
    Plane(const Vec3d &point, const Vec3d &normal) : _point(point), _normal(normal) {}
    ~Plane() override {}

    bool intersect(const Ray &ray, double &t) const override;

    Vec3d getSurfaceColor(const Vec3d &p_hit) const override;

protected:
    Vec3d _point;   //< Point on the plane.
    Vec3d _normal;  //< Normal of the plane.
};


/**
 * @brief The Sphere class.
 *        A sphere is represented implicitly by a center and a radius.
 */
class Sphere : public SceneObject
{
public:
    Sphere(const Vec3d &center, const double &radius) : _radius(radius), _center(center) {}
    Sphere(const Vec3d& center, const double& radius, const Vec3d& color) : SceneObject(color), _radius(radius), _center(center) {}
    ~Sphere() override {}

    bool intersect(const Ray &ray, double &t) const override;

    Vec3d getSurfaceColor(const Vec3d &p_hit) const override;

protected:
    double _radius;  //< Radius of the sphere.
    Vec3d _center;  //< Center of the sphere.
};
