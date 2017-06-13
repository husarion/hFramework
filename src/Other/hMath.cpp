/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include "hMath.h"

#include <stdio.h>
#include <stdint.h>
#include <cmath>

#include <hUtils.h>

#define TOLERANCE 0.00001f

namespace hFramework
{

float invSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	int32_t i = *(int32_t*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

hQuaternion hQuaternion::fromAxisAndAngle(float x, float y, float z, float angle)
{
	angle = d2r(angle / 2);

	float c = cosf(angle);
	float s = sinf(angle);

	return hQuaternion(c, x * s, y * s, z * s);
}
hQuaternion operator*(const hQuaternion& q1, const hQuaternion& q2)
{
	float a1 = q1.scalar;
	float a2 = q1.x;
	float a3 = q1.y;
	float a4 = q1.z;
	float b1 = q2.scalar;
	float b2 = q2.x;
	float b3 = q2.y;
	float b4 = q2.z;

	hQuaternion q;
	q.scalar = a1 * b1 - a2 * b2 - a3 * b3 - a4 * b4;
	q.x = a1 * b2 + a2 * b1 + a3 * b4 - a4 * b3;
	q.y = a1 * b3 - a2 * b4 + a3 * b1 + a4 * b2;
	q.z = a1 * b4 + a2 * b3 - a3 * b2 + a4 * b1;

	return q;
}
hQuaternion hQuaternion::operator+(const hQuaternion& q2)
{
	hQuaternion q;
	q.scalar = scalar + q2.scalar;
	q.x = x + q2.x;
	q.y = y + q2.y;
	q.z = z + q2.z;

	return q;
}
hQuaternion hQuaternion::operator*(float val)
{
	hQuaternion q;
	q.scalar = scalar * val;
	q.x = x * val;
	q.y = y * val;
	q.z = z * val;

	return q;
}
void hQuaternion::normalize()
{
	float mag2 = scalar * scalar + x * x + y * y + z * z;
	if (fabsf(mag2) > TOLERANCE && fabsf(mag2 - 1.0f) > TOLERANCE)
	{
		float mag = invSqrt(mag2);
		scalar *= mag;
		x *= mag;
		y *= mag;
		z *= mag;
	}
}
hQuaternion hQuaternion::normalized()
{
	hQuaternion q(scalar, x, y, z);
	q.normalize();
	return q;
}
void hQuaternion::toPitchRollYaw(float& pitch, float& roll, float& yaw)
{
	float q0 = scalar;
	float q1 = x;
	float q2 = y;
	float q3 = z;

	float q2_2 = q2 * q2;
	pitch = atanf(2.0f * (q0 * q1 + q2 * q3) / (1.0f - 2.0f * (q1 * q1 + q2_2)));
	roll = asinf(2.0f * (q0 * q2 - q3 * q1));
	yaw = atan2f(2.0f * (q0 * q3 + q1 * q2), (1.0f - 2.0f * (q2_2 + q3 * q3)));
}
void hQuaternion::print() const
{
	printf("%6f,%6f,%6f,%6f", scalar, x, y, z);
}
hVector3D hQuaternion::rotatedVector(const hVector3D& v)
{
	float vx = v.x;
	float vy = v.y;
	float vz = v.z;

	float w = scalar;

	float ox = (w * w + x * x - y * y - z * z) * vx + (2 * x * y - 2 * w * z)         * vy + (2 * x * z + 2 * w * y)         * vz;
	float oy = (2 * x * y + 2 * w * z)         * vx + (w * w - x * x + y * y - z * z) * vy + (2 * y * z - 2 * w * x)         * vz;
	float oz = (2 * x * z - 2 * w * y)         * vx + (2 * y * z + 2 * w * x)         * vy + (w * w - x * x - y * y + z * z) * vz;
	return hVector3D(ox, oy, oz);
}

void hVector3D::normalize()
{
	float m = invSqrt(lengthSq());
	x *= m;
	y *= m;
	z *= m;
}

}
