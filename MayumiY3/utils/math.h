#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "../Struct/XYZ.h"

namespace Math
{
    float ClampX(float x);
    float ClampY(float y);

	float getRandF(float min, float max);

	//
	//Vec3 Math
	//
	namespace Vector {
		float length(Vec3 vector);
		float distance(Vec3 source, Vec3 target);
		float distanceVA(ViewAngle source, ViewAngle target);
		ViewAngle getViewAngleBetween(Vec3 sourceViewAngle, Vec3 destinationVec);
		void Normalize(Vec3& vec);
	}

	template<class T>
	float getNormalized(T val)
	{
		while (val < -180.0f) val += 360.0f;
		while (val > 180.0f)  val -= 360.0f;
		return val;
	}
}
