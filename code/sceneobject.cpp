#include "sceneobject.h"

#include "util.h"
#include "vec3.h"

#include <cmath>
#include <utility>


/**
 * @brief SceneObject::SceneObject
 */
SceneObject::SceneObject() :
    _color(1.0, 1.0, 1.0)
{
}

/**
 * @brief SceneObject::SceneObject
     * @param color Color of the object, if it is not overwritten by the derived class.
 */
SceneObject::SceneObject(const Vec3d& color) :
    _color(color)
{
}

/**
 * @brief Plane::intersect
 */
bool Plane::intersect(const Ray &ray, double &t) const
{
    double denom = this->_normal.dot(ray.dir);

    // check if parallel
    if (!Vec3d::approxEq(denom, 0.))
    {
        const Vec3d origin2point = this->_point - ray.origin;

        t = origin2point.dot(this->_normal) / denom;

        return (t >= 0.);
    }

    return false;
}

/**
 * @brief Plane::getSurfaceColor
 * @param p_hit point where the surface was hit
 */
Vec3d Plane::getSurfaceColor(const Vec3d &p_hit) const
{
    // generate grey/white chess board pattern
    const double pi = std::acos(-1);

    const double freq = 0.125;
    const double s = cos(p_hit[0] * 2.0 * pi * freq) * cos(p_hit[2] * 2.0 * pi * freq);

    return Vec3d(0.2) + (s > 0) * Vec3d(0.4);
}


/**
 * @brief Sphere::intersect
 * @param ray Reference to the ray to be checked for intersection with objects.
 * @param t Reference to the intersection distance along the ray.
 */
bool Sphere::intersect(const Ray &ray, double &t) const
{
double a, b, c, D;

	a = ray.dir.dot(ray.dir);
	b = (2.0 * ray.dir).dot(ray.origin - this->_center);
	c = (ray.origin - this->_center).dot(ray.origin - this->_center) - (_radius * _radius);
	D = pow(b,2.) - (4 * a * c);

	if (D > 0) {
		double t1, t2;
		t1 = ((-1)*b + sqrt(D)) / (2 * a);
		t2 = ((-1)*b - sqrt(D)) / (2 * a);
		if (t1 < t2) {
			t = t1;
		}
		else
		{
			t = t2;
		}
		return true;
	}
	else if (D == 0)
	{
		t = -b / (2 * a);
		return true;

	}
	else
	{
		return false;
	}
}

/**
 * @brief Sphere::getSurfaceColor
 */
Vec3d Sphere::getSurfaceColor(const Vec3d &p_hit) const
{
    return this->_color;
}
