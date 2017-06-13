/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef __HMATH_H__
#define __HMATH_H__

#include <stdio.h>
#include <cmath>

namespace hFramework
{

class hVector3D
{
public:
	float x;
	float y;
	float z;

	hVector3D() : x(0), y(0), z(0) { }
	hVector3D(float x, float y, float z) : x(x), y(y), z(z) {  }

	float lengthSq()
	{
		return x * x + y * y + z * z;
	}
	float length()
	{
		return sqrtf(lengthSq());
	}

	void normalize();
	hVector3D normalized()
	{
		hVector3D r(x, y, z);
		r.normalize();
		return r;
	}

	void print() const
	{
		printf("%6f,%6f,%6f", x, y, z);
	}
};

class hQuaternion
{
public:
	float scalar, x, y, z;

	hQuaternion() : scalar(1), x(0), y(0), z(0) { }
	hQuaternion(float scalar, float x, float y, float z) : scalar(scalar), x(x), y(y), z(z) { }

	hQuaternion conjugate()
	{
		return hQuaternion(scalar, -x, -y, -z);
	}

	static hQuaternion fromAxisAndAngle(float x, float y, float z, float angle);
	friend hQuaternion operator*(const hQuaternion& q1, const hQuaternion& q2);
	hQuaternion operator*(float val);
	hQuaternion operator+(const hQuaternion& q);

	void normalize();
	hQuaternion normalized();

	void toPitchRollYaw(float& pitch, float& roll, float& yaw);

	void print() const;

	hVector3D rotatedVector(const hVector3D& v);
};

}

#endif
