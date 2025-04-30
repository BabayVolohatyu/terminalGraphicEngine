#ifndef CAMERA_H
#define CAMERA_H

#include <math/vector3.h>
#include <math/quaternion.h>

#include <engine/object.h>

using namespace math;

namespace engine{

class Camera{
  private:
    Vector3 position_; // Camera position in world space
    Quaternion rotation_; // Camera orientation
    double fov_; // Field of view (in degrees)
    double aspectRatio_; // Aspect ratio (width / height)
    double nearClip_; // Near clipping plane
    double farClip_; // Far clipping plane

    static Matrix<double> translate(const Vector3 &position);

    static void display(int screenX, int screenY);

    static void drawLine(int x0, int y0, int x1, int y1) ;

    void drawVertices(const Object &object, int screenWidth, int screenHeight) const;

    static void drawTriangle(const Point &p1, const Point &p2, const Point &p3) ;

    void drawTriangles(const Object &object, int screenWidth, int screenHeight) const;

  public:
    const Vector3 worldForward{0, 0, -1};

    const Vector3 worldUp{0, 1, 0};

    const Vector3 worldRight{1, 0, 0};

    //add dependency between angles and those three vectors
    Vector3 forward{0, 0, -1};

    Vector3 up{0, 1, 0};

    Vector3 right{1, 0, 0};

    explicit Camera(const Vector3 &position = {0, 0, 0},
           const Quaternion &rotation = {0, 0, 0},
           double fov = 90,
           double aspectRatio = 1.77,// 16/9
           double nearClip = 0.1,
           double farClip = 1000);

    //function accepts two parameters for its variants that work accordingly to camera rotation
    void translatePosition(const Vector3 &direction, double amount);

    void moveForward(double amount);

    void moveRight(double amount);

    void moveUp(double amount);

    void lookAt(const Point &point);

    void rotate(const Vector3 &eulerAngles);

    [[nodiscard]] Matrix<double> viewMatrix() const;

    [[nodiscard]] Matrix<double> projectionMatrix() const;

    //draws only triangles for now
    void drawObject(const Object &object, int screenWidth, int screenHeight) const;

    [[nodiscard]] Vector3 getPosition() const;

    [[nodiscard]] Quaternion getRotation() const;

    [[nodiscard]] double getFov() const;

    [[nodiscard]] double getAspectRatio() const;

    [[nodiscard]] double getNearClip() const;

    [[nodiscard]] double getFarClip() const;

    void setPosition(const Vector3 &position);

    void setRotation(const Quaternion &rotation);

    void setFov(double fov);

    void setAspectRatio(double aspectRatio);

    void setNearClip(double nearClip);

    void setFarClip(double farClip);
};
}

#endif
