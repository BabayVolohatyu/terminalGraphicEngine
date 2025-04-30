#include <math/quaternion.h>
#include <cmath>

using namespace math;

void Quaternion::approximate(double &w, double &x, double &y, double &z) {
    if (std::abs(w) <= kMinLimit) w = 0;
    if (std::abs(x) <= kMinLimit) x = 0;
    if (std::abs(y) <= kMinLimit) y = 0;
    if (std::abs(z) <= kMinLimit) z = 0;
}

double Quaternion::approximate(double value) {
    return std::abs(value)<=kMinLimit?0:value;
}

double Quaternion::degreesToRadians(double degrees) {
    return degrees * M_PI / 180;
}

Quaternion::Quaternion()
: w_(1), x_(0), y_(0), z_(0) {}

Quaternion::Quaternion(double w, double x, double y, double z)
 : w_(w), x_(x), y_(y), z_(z){}

Quaternion::Quaternion(double xRotation, double yRotation, double zRotation) {
    double phi = degreesToRadians(xRotation);
    double theta = degreesToRadians(yRotation);
    double psi = degreesToRadians(zRotation);
    w_ = std::cos(phi / 2) * std::cos(theta / 2) * std::cos(psi / 2)
       + std::sin(phi / 2) * std::sin(theta / 2) * std::sin(psi / 2);

    x_ = std::sin(phi / 2) * std::cos(theta / 2) * std::cos(psi / 2)
               + std::cos(phi / 2) * std::sin(theta / 2) * std::sin(psi / 2);

    y_ = std::cos(phi / 2) * std::sin(theta / 2) * std::cos(psi / 2)
               - std::sin(phi / 2) * std::cos(theta / 2) * std::sin(psi / 2);

    z_ = std::cos(phi / 2) * std::cos(theta / 2) * std::sin(psi / 2)
               - std::sin(phi / 2) * std::sin(theta / 2) * std::cos(psi / 2);

    approximate(w_, x_, y_, z_);
}

Quaternion::Quaternion(const Vector3 &rotation)
    : Quaternion{rotation.getX(), rotation.getY(), rotation.getZ()}{}

Quaternion::Quaternion(double w, const Vector3 &rotation)
: Quaternion{w, rotation.getX(), rotation.getY(), rotation.getZ()}{}

Matrix<double> Quaternion::toMatrix(const Quaternion &quaternion) {
    Matrix<double> result(4, 4, 0);

    double w = quaternion.w_;
    double x = quaternion.x_;
    double y = quaternion.y_;
    double z = quaternion.z_;

    result[0][0] = approximate( 1 - 2 * (y * y + z * z));
    result[0][1] = approximate( 2 * (x * y - w * z));
    result[0][2] = approximate(2 * (x * z + w * y));

    result[1][0] = approximate( 2 * (x * y + w * z));
    result[1][1] = approximate(1 - 2 * (x * x + z * z));
    result[1][2] = approximate(2 * (y * z - w * x));

    result[2][0] = approximate(2 * (x * z - w * y));
    result[2][1] = approximate(2 * (y * z + w * x));
    result[2][2] = approximate(1 - 2 * (x * x + y * y));

    result[3][3] = 1;

    return result;
}

Quaternion Quaternion::fromAxis(const Vector3 &axis, double angle) {
    double halfAngle = degreesToRadians(angle) / 2;
    double sinHalfAngle = std::sin(halfAngle);
    return {
        cos(halfAngle),
        axis.getX() * sinHalfAngle,
        axis.getY() * sinHalfAngle,
        axis.getZ() * sinHalfAngle
    };
}

Quaternion Quaternion::normalized() const {
    double mag = magnitude();
    return {approximate(w_/mag),
                      approximate(x_/mag),
                      approximate(y_/mag),
                      approximate(z_/mag)};
}

Quaternion Quaternion::conjugated() const {
    return {w_, -x_, -y_, -z_};
}

double Quaternion::magnitude() const {
    return std::sqrt(w_ * w_ + x_ * x_ + y_ * y_ + z_ * z_);
}

Quaternion Quaternion::operator*(const Quaternion &other) const noexcept {
    double newW = w_ * other.w_ - x_ * other.x_ - y_ * other.y_ - z_ * other.z_;
    double newX = w_ * other.x_ + x_ * other.w_ + y_ * other.z_ - z_ * other.y_;
    double newY = w_ * other.y_ - x_ * other.z_ + y_ * other.w_ + z_ * other.x_;
    double newZ = w_ * other.z_ + x_ * other.y_ - y_ * other.x_ + z_ * other.w_;

    approximate(newW, newX, newY, newZ);

    return {newW, newX, newY, newZ};
}

Vector3 Quaternion::operator*(const Vector3 &v) const {
    Quaternion q_v(0, v.getX(), v.getY(), v.getZ());
    Quaternion q_conj = conjugated();
    Quaternion q_rotated = *this * q_v * q_conj;
    return {q_rotated.x_, q_rotated.y_, q_rotated.z_};
}

Quaternion & Quaternion::operator*=(const Quaternion &other) noexcept {
    const double oldW = w_;
    const double oldX = x_;
    const double oldY = y_;
    const double oldZ = z_;

    w_ = oldW * other.w_ - oldX * other.x_ - oldY * other.y_ - oldZ * other.z_;
    x_ = oldW * other.x_ + oldX * other.w_ + oldY * other.z_ - oldZ * other.y_;
    y_ = oldW * other.y_ - oldX * other.z_ + oldY * other.w_ + oldZ * other.x_;
    z_ = oldW * other.z_ + oldX * other.y_ - oldY * other.x_ + oldZ * other.w_;

    approximate(w_, x_, y_, z_);

    return *this;
}

double Quaternion::getW() const {
    return w_;
}

double Quaternion::getX() const {
    return x_;
}

double Quaternion::getY() const {
    return y_;
}

double Quaternion::getZ() const {
    return z_;
}

void Quaternion::setW(double w) {
    w_ = w;
}

void Quaternion::setX(double x) {
    x_ = x;
}

void Quaternion::setY(double y) {
    y_ = y;
}

void Quaternion::setZ(double z) {
    z_ = z;
}
