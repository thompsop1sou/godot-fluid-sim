#ifndef VEC_3_H
#define VEC_3_H

#include <godot_cpp/variant/vector3.hpp>

class Vec3
{
public:
	// Member Variables
	float x, y, z;
	// Constant Members Variables
	static const Vec3 ZERO;
	static const Vec3 UP;
	static const Vec3 DOWN;
	static const Vec3 LEFT;
	static const Vec3 RIGHT;
	static const Vec3 FORE;
	static const Vec3 BACK;
	// Constructors and Destructors
	Vec3();
	Vec3(const Vec3& other_vec3);
	Vec3(float xyz);
	Vec3(float p_x, float p_y, float p_z);
	Vec3(const godot::Vector3 &godot_vector_3);
	~Vec3();
	// Overloaded Assignment Operators
	Vec3& operator = (const Vec3& other_vec3);
	Vec3& operator += (const Vec3& other_vec3);
	Vec3& operator -= (const Vec3& other_vec3);
	Vec3& operator *= (const Vec3& other_vec3);
	Vec3& operator *= (const float other_float);
	Vec3& operator /= (const Vec3& other_vec3);
	Vec3& operator /= (const float other_float);
	operator godot::Vector3() const;
	// Member Functions
	float length() const;
	float length_squared() const;
	float distance(const Vec3& other_vec3) const;
	float distance_squared(const Vec3& other_vec3) const;
	float dot(const Vec3& other_vec3) const;
	Vec3 normalized() const;
};

// Overloaded Binary Operators
Vec3 operator + (const Vec3& left_vec3, const Vec3& right_vec3);
Vec3 operator - (const Vec3& left_vec3, const Vec3& right_vec3);
Vec3 operator * (const Vec3& left_vec3, const Vec3& right_vec3);
Vec3 operator / (const Vec3& left_vec3, const Vec3& right_vec3);
Vec3 operator * (const Vec3& left_vec3, const float right_float);
Vec3 operator / (const Vec3& left_vec3, const float right_float);
Vec3 operator * (const float left_float, const Vec3& right_vec3);
Vec3 operator / (const float left_float, const Vec3& right_vec3);
bool operator == (const Vec3& left_vec3, const Vec3& right_vec3);
bool operator != (const Vec3& left_vec3, const Vec3& right_vec3);

#endif