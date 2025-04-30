#ifndef QUATERNION_H
#define QUATERNION_H

#include <math/vector3.h>
#include <math/matrix.h>

namespace math {

class Quaternion {
  private:
    constexpr double static kMinLimit = 1e-10;

    double w_, x_, y_, z_;

    static void approximate(double &w, double &x, double &y, double &z);

    double approximate(double value);

    static double degreesToRadians(double degrees);
  public:
    Quaternion();

    explicit Quaternion(double w, double x, double y, double z);

    Quaternion(double w, const Vector3 &rotation);

    explicit Quaternion(const Vector3 &rotation);

    Quaternion(double xRotation, double yRotation, double zRotation);

    ~Quaternion() = default;

    static Matrix<double> toMatrix(const Quaternion& quaternion);

    static Quaternion fromAxis(const Vector3 &axis, double angle);

    [[nodiscard]] Quaternion normalized() const;

    [[nodiscard]] Quaternion conjugated() const;

    [[nodiscard]] double magnitude() const;

    Quaternion operator*(const Quaternion &other) const noexcept;

    Vector3 operator*(const Vector3 &v) const;

    Quaternion &operator*=(const Quaternion &other) noexcept;

    [[nodiscard]] double getW() const;

    [[nodiscard]] double getX() const;

    [[nodiscard]] double getY() const;

    [[nodiscard]] double getZ() const;

    void setW(double w);

    void setX(double x);

    void setY(double y);

    void setZ(double z);
};
}

#endif