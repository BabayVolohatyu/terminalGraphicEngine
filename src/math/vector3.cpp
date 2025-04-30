#include <math/vector3.h>
#include <cmath>

using namespace math;

Vector3::Vector3()
: x_(0.0), y_(0.0), z_(0.0) {}

Vector3::Vector3(double x, double y, double z)
: x_(x), y_(y), z_(z) {}

Vector3 Vector3::operator+(const Vector3 &other) const {
    return {x_ + other.x_, y_ + other.y_, z_ + other.z_};
}

Vector3 Vector3::operator-(const Vector3 &other) const {
    return {x_ - other.x_, y_ - other.y_, z_ - other.z_};
}

Vector3 Vector3::operator-() const {
    return {-x_, -y_, -z_};
}

Vector3 Vector3::operator*(double scalar) const {
    return {x_ * scalar, y_ * scalar, z_ * scalar};
}

Vector3 Vector3::operator/(double scalar) const {
    return {x_ / scalar, y_ / scalar, z_ / scalar};
}

Vector3 Vector3::cross(const Vector3 &other) const {
    return {
        y_ * other.z_ - z_ * other.y_,
        z_ * other.x_- x_ * other.z_,
        x_ * other.y_ - y_ * other.x_
    };
}

double Vector3::dot(const Vector3 &other) const {
    return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}

Vector3 Vector3::normalized() const {
    double mag = magnitude();
    return {x_/mag, y_/mag, z_/mag};
}

double Vector3::magnitude() const {
    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

double Vector3::getX() const {
    return x_;
}

double Vector3::getY() const {
    return y_;
}

double Vector3::getZ() const {
    return z_;
}

void Vector3::setX(double x) {
    x_ = x;
}

void Vector3::setY(double y) {
    y_ = y;
}

void Vector3::setZ(double z) {
    z_ = z;
}
