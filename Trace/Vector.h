#pragma once
#include <Windows.h>
#include <math.h>
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h

#define M_PI_2      (M_PI * 2.f)

#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.

#define M_PHI		1.61803398874989484820 // golden ratio

// NJS: Inlined to prevent floats from being autopromoted to doubles, as with the old system.
#ifndef RAD2DEG
#define RAD2DEG(x)  ((float)(x) * (float)(180.f / M_PI_F))
#endif

#ifndef DEG2RAD
#define DEG2RAD(x)  ((float)(x) * (float)(M_PI_F / 180.f))
#endif

// MOVEMENT INFO
enum
{
	PITCH = 0,	// up / down
	YAW,		// left / right
	ROLL		// fall over
};

struct Vector
{
	float X, Y, Z;

	inline Vector(void) {}
	inline Vector(const float x, const float y, const float z)
	{
		X = x; Y = y; Z = z;
	}

	inline Vector operator + (const Vector& A) const
	{
		return Vector(X + A.X, Y + A.Y, Z + A.Z);
	}

	inline Vector operator + (const float A) const
	{
		return Vector(X + A, Y + A, Z + A);
	}

	inline Vector operator * (const float A) const
	{
		return Vector(A*X, A*Y, A*Z);
	}

	inline Vector operator * (const Vector& A) const
	{
		return Vector(A.X*X, A.Y*Y, A.Z*Z);
	}

	inline Vector operator - (const float A) const
	{
		return Vector(A*X, A*Y, A*Z);
	}

	inline Vector operator - (const Vector& A) const
	{
		return Vector(A.X - X, A.Y - Y, A.Z - Z);
	}

	inline Vector operator / (const float A) const
	{
		return Vector(A / X, A / Y, A / Z);
	}

	inline Vector operator / (const Vector& A) const
	{
		return Vector(A.X / X, A.Y / Y, A.Z / Z);
	}

	float dot(const Vector &vec) const
	{
		return X * vec.X + Y * vec.Y + Z * vec.Z;
	}

	inline float lengthSquared()
	{
		return X * X + Y * Y + Z * Z;
	}
	inline Vector get()
	{
		return { X,Y,Z };
	}
	inline float length()
	{
		return (float)sqrt(lengthSquared());
	}

	inline Vector perpendicularTo()
	{
		return Vector(Z, Y, -X);
	}

	inline Vector Normalize()
	{
		float length = this->length();
		if (length != 0)
		{
			float inv = 1.0f / length;
			X *= inv;
			Y *= inv;
			Z *= inv;
		}
		return Vector(X, Y, Z);
	}

	inline float DistTo(const Vector& A) {
		float out = sqrtf(powf(X - A.X, 2) + powf(Y - A.Y, 2) + powf(Z - A.Z, 2));
		return out < 0 ? out * -1 : out;
	}
};