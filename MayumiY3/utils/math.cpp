#include "math.h"
#include "../Struct/CS.h"

#include <DirectXMath.h>

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)

float Math::ClampX(float x)
{
    if (x > 89.0f) return 89.0f;
    else if (x < -89.0f) return -89.0f;
    return x;
}

float Math::ClampY(float y)
{
    if (y > 180.0f) return 180.0f;
    else if (y < -180.0f) return -180.0f;
    return y;
}

float Math::getRandF(float min, float max)
//https://stackoverflow.com/questions/686353/random-float-number-generation
{
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

//
//Vec3 Math
//

float Math::Vector::length(Vec3 vector)
{
    return sqrtf(vector.x()* vector.x() + vector.y()* vector.y() + vector.z()* vector.z());
}

float Math::Vector::distance(Vec3 source, Vec3 target)
{
    Vec3 delta = source - target;
    return length(delta);
}

float Math::Vector::distanceVA(ViewAngle source, ViewAngle target)
{
    ViewAngle delta = ViewAngle(
        Math::getNormalized(source.x() - target.x()),
        Math::getNormalized(source.y() - target.y())
    );
    return sqrtf(delta.x() * delta.x() + delta.y() * delta.y() + delta.z() * delta.z());  //@todo lengthVA function, or template for vec and va
}

ViewAngle Math::Vector::getViewAngleBetween(Vec3 source, Vec3 destVec)
{
    Vec3 delta = destVec - source;
    float deltaLength = length(delta);

    float pitch = float(-asin(delta.z() / deltaLength) * (180.0 / M_PI));
    float yaw = float(atan2(delta.y(), delta.x()) * (180.0 / M_PI));

    return ViewAngle(pitch, yaw);
}

void Math::Vector::Normalize(Vec3& vec)
{
    vec.set_x(getNormalized(vec.x()));
    vec.set_y(getNormalized(vec.y()));
    vec.set_z(0.0f);
}