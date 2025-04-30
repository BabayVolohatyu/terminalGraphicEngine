#ifndef OBJECT_H
#define OBJECT_H

#include <math/vector3.h>
#include <math/quaternion.h>

using namespace math;

namespace engine{

struct Point{
  Vector3 position;
  Point();

  explicit Point(const Vector3 &initPosition);

  Point(double x, double y, double z);
};
struct Triangle{
  const Point *vertices[3]{};

  Triangle(const Point* p1, const Point *p2, const Point *p3);

  [[nodiscard]] Vector3 calculateNormal() const;
};

class Object {
  private:
    Vector3 _position;
    Quaternion _rotation;
    std::vector<Point> _vertices;
    std::vector<Triangle> _triangles;

    [[nodiscard]] Matrix<double> getTranslationMatrix() const;

  public:
    Object();

    Object(const Vector3 &position, const Quaternion &rotation);

    void addVertex(const Point &vertex);

    void removeVertex(int index);

    void makeTriangle(int v1, int v2, int v3);

    void rotate(const Quaternion &rotationQuaternion);

    [[nodiscard]] Matrix<double> getModelMatrix() const;

    [[nodiscard]] std::vector<Point> vertices() const;

    [[nodiscard]] std::vector<Triangle> triangles() const;

    [[nodiscard]] Vector3 position() const;

    [[nodiscard]] Quaternion rotation() const;

    void setPosition(const Vector3 &position);

    void setPos(double x, double y, double z);

    void setRot(const Quaternion &rotation);

};
}
#endif
