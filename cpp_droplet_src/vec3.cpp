#include <cmath>
#include "vec3.h"

// Static Member Variables

const Vec3 Vec3::ZERO = Vec3(0.0, 0.0, 0.0);
const Vec3 Vec3::DOWN = Vec3(0.0, -1.0, 0.0);
const Vec3 Vec3::UP = Vec3(0.0, 1.0, 0.0);
const Vec3 Vec3::LEFT = Vec3(-1.0, 0.0, 0.0);
const Vec3 Vec3::RIGHT = Vec3(1.0, 0.0, 0.0);
const Vec3 Vec3::BACK = Vec3(0.0, 0.0, -1.0);
const Vec3 Vec3::FORE = Vec3(0.0, 0.0, 1.0);

// Constructors and Destructors

Vec3::Vec3() : x(0.0), y(0.0), z(0.0)
{}

Vec3::Vec3(const Vec3& other_vec3) :
	x(other_vec3.x),
	y(other_vec3.y),
	z(other_vec3.z)
{}

Vec3::Vec3(float xyz) :
	x(xyz),
	y(xyz),
	z(xyz)
{}

Vec3::Vec3(float p_x, float p_y, float p_z) :
	x(p_x),
	y(p_y),
	z(p_z)
{}

Vec3::Vec3(godot::Vector3 &godot_vector3) :
	x(godot_vector3.x),
	y(godot_vector3.y),
	z(godot_vector3.z)
{}

Vec3::~Vec3()
{}

// Overloaded Assignment Operators

Vec3& Vec3::operator = (const Vec3& other_vec3)
{
	x = other_vec3.x;
	y = other_vec3.y;
	z = other_vec3.z;
	return *this;
}

Vec3& Vec3::operator += (const Vec3& other_vec3)
{
	x += other_vec3.x;
	y += other_vec3.y;
	z += other_vec3.z;
	return *this;
}

Vec3& Vec3::operator -= (const Vec3& other_vec3)
{
	x -= other_vec3.x;
	y -= other_vec3.y;
	z -= other_vec3.z;
	return *this;
}

Vec3& Vec3::operator *= (const Vec3& other_vec3)
{
	x *= other_vec3.x;
	y *= other_vec3.y;
	z *= other_vec3.z;
	return *this;
}

Vec3& Vec3::operator *= (const float other_float)
{
	x *= other_float;
	y *= other_float;
	z *= other_float;
	return *this;
}

Vec3& Vec3::operator /= (const Vec3& other_vec3)
{
	x /= other_vec3.x;
	y /= other_vec3.y;
	z /= other_vec3.z;
	return *this;
}

Vec3& Vec3::operator /= (const float other_float)
{
	x /= other_float;
	y /= other_float;
	z /= other_float;
	return *this;
}

Vec3::operator godot::Vector3() const
{
	return godot::Vector3(x, y, z);
}

// Member Functions

float Vec3::length() const
{
	return std::sqrt(x * x + y * y + z * z);
}

float Vec3::length_squared() const
{
	return x * x + y * y + z * z;
}

float Vec3::distance(const Vec3& other_vec3) const
{
	Vec3 diff = *this - other_vec3;
	return diff.length();
}

float Vec3::distance_squared(const Vec3& other_vec3) const
{
	Vec3 diff = *this - other_vec3;
	return diff.length_squared();
}

float Vec3::dot(const Vec3& other_vec3) const
{
	return x * other_vec3.x + y * other_vec3.y + z * other_vec3.z;
}

Vec3 Vec3::normalized() const
{
	return *this / length();
}

// Overloaded Binary Operators

Vec3 operator + (const Vec3& left_vec3, const Vec3& right_vec3)
{
	return Vec3(left_vec3.x + right_vec3.x,
		left_vec3.y + right_vec3.y,
		left_vec3.z + right_vec3.z);
}

Vec3 operator - (const Vec3& left_vec3, const Vec3& right_vec3)
{
	return Vec3(left_vec3.x - right_vec3.x,
		left_vec3.y - right_vec3.y,
		left_vec3.z - right_vec3.z);
}

Vec3 operator * (const Vec3& left_vec3, const Vec3& right_vec3)
{
	return Vec3(left_vec3.x * right_vec3.x,
		left_vec3.y * right_vec3.y,
		left_vec3.z * right_vec3.z);
}

Vec3 operator / (const Vec3& left_vec3, const Vec3& right_vec3)
{
	return Vec3(left_vec3.x / right_vec3.x,
		left_vec3.y / right_vec3.y,
		left_vec3.z / right_vec3.z);
}

Vec3 operator * (const Vec3& left_vec3, const float right_float)
{
	return Vec3(left_vec3.x * right_float,
		left_vec3.y * right_float,
		left_vec3.z * right_float);
}

Vec3 operator / (const Vec3& left_vec3, const float right_float)
{
	return Vec3(left_vec3.x / right_float,
		left_vec3.y / right_float,
		left_vec3.z / right_float);
}

Vec3 operator * (const float left_float, const Vec3& right_vec3)
{
	return Vec3(left_float * right_vec3.x,
		left_float * right_vec3.y,
		left_float * right_vec3.z);
}

Vec3 operator / (const float left_float, const Vec3& right_vec3)
{
	return Vec3(left_float / right_vec3.x,
		left_float / right_vec3.y,
		left_float / right_vec3.z);
}

bool operator == (const Vec3& left_vec3, const Vec3& right_vec3)
{
	return left_vec3.x == right_vec3.x &&
		   left_vec3.y == right_vec3.y &&
		   left_vec3.z == right_vec3.z;
}

bool operator != (const Vec3& left_vec3, const Vec3& right_vec3)
{
	return left_vec3.x != right_vec3.x ||
		   left_vec3.y != right_vec3.y ||
		   left_vec3.z != right_vec3.z;
}
