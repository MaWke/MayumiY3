#pragma once
#include<iostream>

class Vec3{
//
//xyz can be set to any value.
//
public:
	Vec3() : m_x(0), m_y(0), m_z(0) {}
	Vec3(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}
	
	float x() const { return m_x; }
	float y() const { return m_y; }
	float z() const { return m_z; }

	void set_x(float x) { m_x = x; }
	void set_y(float y) { m_y = y; }
	void set_z(float z) { m_z = z; }
private:
	float m_x;
	float m_y;
	float m_z;
};


Vec3 operator+(const Vec3& rValue, const Vec3& lValue);		//x+x, y+y, z+z
Vec3 operator-(const Vec3& rValue, const Vec3& lValue);		//x-x, y-y, z-z
Vec3 operator*(const Vec3& rValue, const Vec3& lValue);		//x*x, y*y, z*z

std::ostream& operator<<(std::ostream& ost, Vec3 cor);	//for debugging

//-----------------------------------------------------------

class ViewAngle{
//
//xyz values are checked x[-89,89], y[0,180], z[0]
//
public:
	ViewAngle(float x, float y, float z);	//clamp angles before setting
	ViewAngle(float x, float y);			//ViewAngle z == 0
	ViewAngle() : m_x(0), m_y(0), m_z(0) { };

	float x() const { return m_x; }
	float y() const { return m_y; }
	float z() const { return m_z; }

	void set_x(float x);
	void set_y(float y);

	operator bool() const;					//checks for NaN
											//check before setting player ViewAngles.
private:
	float m_x;
	float m_y;
	float m_z;

	//void set_z(float z);	//disabled for ViewAngle
};

ViewAngle operator+(const ViewAngle& rValue, const ViewAngle& lValue);		//x+x, y+y, z=0 with clamping
ViewAngle operator-(const ViewAngle& rValue, const ViewAngle& lValue);		//x-x, y-y, z=0 with clamping

std::ostream& operator<<(std::ostream& ost, ViewAngle cor);	//for debugging