#ifndef VECTOR_H
#define VECTOR_H

namespace math {

class Vector3 {
  private:

    double x_, y_, z_;

  public:

    Vector3();

    Vector3(double x, double y, double z);

    Vector3 operator+(const Vector3 &other) const;

    Vector3 operator-(const Vector3 &other) const;

    Vector3 operator-() const;

    Vector3 operator*(double scalar) const;

    Vector3 operator/(double scalar) const;

    [[nodiscard]] Vector3 cross(const Vector3& other) const;

    [[nodiscard]] double dot(const Vector3& other) const;

    [[nodiscard]] Vector3 normalized() const;

    [[nodiscard]] double magnitude() const;

    [[nodiscard]] double getX() const;

    [[nodiscard]] double getY() const;

    [[nodiscard]] double getZ() const;

    void setX(double x);

    void setY(double y);

    void setZ(double z);
};
}

#endif
