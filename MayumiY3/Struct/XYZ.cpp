#include "XYZ.h"
#include "../utils/math.h"

//
//XYZ
//


//
//Vec3
//
Vec3 operator+(const Vec3& rValue, const Vec3& lValue)
{
	return Vec3(
		rValue.x() + lValue.x(),
		rValue.y() + lValue.y(),
		rValue.z() + lValue.z()
		);
}

Vec3 operator-(const Vec3& rValue, const Vec3& lValue)
{
	return Vec3(
		rValue.x() - lValue.x(),
		rValue.y() - lValue.y(),
		rValue.z() - lValue.z()
	);
}

Vec3 operator*(const Vec3& rValue, const Vec3& lValue)
{
	return Vec3(
		rValue.x() * lValue.x(),
		rValue.y() * lValue.y(),
		rValue.z() * lValue.z()
	);
}

std::ostream& operator<<(std::ostream& ost, Vec3 cor)
{
	return ost << '(' << cor.x() << ',' << cor.y() << ',' << cor.z() << ')';
}

//
//ViewAngle
//
ViewAngle::ViewAngle(float x, float y, float z)
	: m_x(Math::ClampX(x)), m_y(Math::ClampY(y)), m_z(0)
{
	
}

ViewAngle::ViewAngle(float x, float y)
	: ViewAngle(x, y, 0)
{

}

void ViewAngle::set_x(float x)
{
	m_x = Math::ClampX(x);
}

void ViewAngle::set_y(float y)
{
	m_y = Math::ClampY(y);
}

ViewAngle::operator bool() const
{
	if (isnan(m_x) || isnan(m_y) || isnan(m_z)) 
		return false;
	return true;
}

ViewAngle operator+(const ViewAngle& rValue, const ViewAngle& lValue)
{
	return ViewAngle(
		Math::ClampX(rValue.x() + lValue.x()),
		Math::ClampY(rValue.y() + lValue.y()),
		0							  //z = 0
	);
}

ViewAngle operator-(const ViewAngle& rValue, const ViewAngle& lValue)
{
	return ViewAngle(
		Math::ClampX(rValue.x() - lValue.x()),
		Math::ClampY(rValue.y() - lValue.y()),
		0							  //z = 0
	);
}

std::ostream& operator<<(std::ostream& ost, ViewAngle cor)
{
	return ost << '(' << cor.x() << ',' << cor.y() << ',' << cor.z() << ')';
}