#include <engine/camera.h>
#include <cmath>

using namespace engine;

Matrix<double> Camera::translate(const Vector3 &position) {
    Matrix<double> result(4, 4);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) result[i][j] = 1;
        }
    }
    result[0][3] = position.getX();
    result[1][3] = position.getY();
    result[2][3] = position.getZ();
    return result;
}

void Camera::display(int screenX, int screenY) {
    std::cout << "\033[" << screenY << ";" << screenX << "H"; //i
    std::cout << "#" << std::flush;
}

void Camera::drawLine(int x0, int y0, int x1, int y1) {
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        display(x0, y0); // Plot the pixel

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err = err - dy;
            x0 = x0 + sx;
        }
        if (e2 < dx) {
            err = err + dx;
            y0 = y0 + sy;
        }
    }
}

void Camera::drawVertices(const Object &object, int screenWidth, int screenHeight) const {
    Matrix<double> modelMatrix = object.getModelMatrix();
    Matrix<double> viewMatrix = this->viewMatrix();
    Matrix<double> projectionMatrix = this->projectionMatrix();
    for (const Point &point: object.getVertices()) {
        Matrix<double> worldPosVector(4, 1);
        worldPosVector[0][0] = point.position.getX();
        worldPosVector[1][0] = point.position.getY();
        worldPosVector[2][0] = point.position.getZ();
        worldPosVector[3][0] = 1;
        worldPosVector = modelMatrix * worldPosVector;
        Matrix<double> viewPosVector = viewMatrix * worldPosVector;
        Matrix<double> clipPosVector = projectionMatrix * viewPosVector;
        if (clipPosVector[3][0] != 0) {
            clipPosVector[0][0] /= clipPosVector[3][0];
            clipPosVector[1][0] /= clipPosVector[3][0];
            clipPosVector[2][0] /= clipPosVector[3][0];
        }
        // Convert to screen space (viewport transformation)
        double screenX = (clipPosVector[0][0] * 0.5 + 0.5) * screenWidth;
        double screenY = (clipPosVector[1][0] * 0.5 + 0.5) * screenHeight; //without 1.0 -... because inverse y-axis

        display(static_cast<int>(screenX), static_cast<int>(screenY));
    }
}

void Camera::drawTriangle(const Point &p1, const Point &p2, const Point &p3) {
    drawLine(static_cast<int>(p1.position.getX()), static_cast<int>(p1.position.getY()),
             static_cast<int>(p2.position.getX()), static_cast<int>(p2.position.getY()));

    drawLine(static_cast<int>(p2.position.getX()), static_cast<int>(p2.position.getY()),
             static_cast<int>(p3.position.getX()), static_cast<int>(p3.position.getY()));

    drawLine(static_cast<int>(p3.position.getX()), static_cast<int>(p3.position.getY()),
             static_cast<int>(p1.position.getX()), static_cast<int>(p1.position.getY()));
}

void Camera::drawTriangles(const Object &object, int screenWidth, int screenHeight) const {
    Matrix<double> modelMatrix = object.getModelMatrix();
    Matrix<double> viewMatrix = this->viewMatrix();
    Matrix<double> projectionMatrix = this->projectionMatrix();

    for (const auto &triangle: object.getTriangles()) {
        Point screenPoints[3];

        for (int i = 0; i < 3; ++i) {
            const Point *vertex = triangle.vertices[i];

            Matrix<double> worldPosVector(4, 1);
            worldPosVector[0][0] = vertex->position.getX();
            worldPosVector[1][0] = vertex->position.getY();
            worldPosVector[2][0] = vertex->position.getZ();
            worldPosVector[3][0] = 1;

            worldPosVector = modelMatrix * worldPosVector;
            Matrix<double> viewPosVector = viewMatrix * worldPosVector;
            Matrix<double> clipPosVector = projectionMatrix * viewPosVector;

            if (clipPosVector[3][0] != 0) {
                clipPosVector[0][0] /= clipPosVector[3][0];
                clipPosVector[1][0] /= clipPosVector[3][0];
                clipPosVector[2][0] /= clipPosVector[3][0];
            }

            double screenX = (clipPosVector[0][0] * 0.5 + 0.5) * screenWidth;
            double screenY = (clipPosVector[1][0] * 0.5 + 0.5) * screenHeight;

            screenPoints[i] = {screenX, screenY, 0};
        }

        drawTriangle(screenPoints[0], screenPoints[1], screenPoints[2]);
    }
}

Camera::Camera(const Vector3 &position,
               const Quaternion &rotation,
               double fov,
               double aspectRatio,
               double nearClip,
               double farClip)
    : position_{position},
      rotation_{rotation},
      fov_{fov},
      aspectRatio_{aspectRatio},
      nearClip_{nearClip},
      farClip_{farClip} {
}

void Camera::translatePosition(const Vector3 &direction, double amount) {
    position_ = position_ + (direction * amount);
}

void Camera::moveForward(double amount) {
    translatePosition(rotation_ * forward, amount);
}

void Camera::moveRight(double amount) {
    translatePosition(rotation_ * right, amount);
}

void Camera::moveUp(double amount) {
    translatePosition(rotation_ * up, amount);
}

void Camera::lookAt(const Point &point) {
    Vector3 desiredForward = {
        point.position.getX() - this->position_.getX(),
        point.position.getY() - this->position_.getY(),
        point.position.getZ() - this->position_.getZ()
    };
    Vector3 currentForward = this->getRotation() * this->forward;

    double cosTheta = currentForward.dot(desiredForward.normalized());

    if (cosTheta > 0.9999) return;

    if (cosTheta < -0.9999) {
        Quaternion turn = Quaternion::fromAxis(worldUp, M_PI);
        Quaternion newRotation = turn.normalized() * this->getRotation();
        this->rotation_ = newRotation.normalized();
    }

    Vector3 rotAxis = currentForward.cross(desiredForward);

    double angle = acos(cosTheta);

    Quaternion rotQuat = Quaternion::fromAxis(rotAxis.normalized(), angle);
    Quaternion newRotation = rotQuat.normalized() * this->getRotation();
    this->rotation_ = newRotation.normalized();
}

void Camera::rotate(const Vector3 &eulerAngles) {
    Quaternion deltaRotation(eulerAngles);
    rotation_ = deltaRotation * rotation_;
}

Matrix<double> Camera::viewMatrix() const {
    Matrix<double> translationMatrix = translate(-position_);
    Matrix<double> rotationMatrix = Quaternion::toMatrix(rotation_).transpose();
    return translationMatrix * rotationMatrix;
}

Matrix<double> Camera::projectionMatrix() const {
    double fovRadians = fov_ * (M_PI / 180);
    double tanHalfFov = tan(fovRadians / 2);
    double range = nearClip_ - farClip_;

    Matrix<double> proj(4, 4, 0);
    proj[0][0] = 1 / (tanHalfFov * aspectRatio_);
    proj[1][1] = 1 / tanHalfFov;
    proj[2][2] = (-nearClip_ - farClip_) / range;
    proj[2][3] = 2 * farClip_ * nearClip_ / range;
    proj[3][2] = 1;
    proj[3][3] = 0;

    return proj;
}

void Camera::drawObject(const Object &object, int screenWidth, int screenHeight) const {
    drawTriangles(object, screenWidth, screenHeight);
}

Vector3 Camera::getPosition() const {
    return position_;
}

Quaternion Camera::getRotation() const {
    return rotation_;
}

double Camera::getFov() const {
    return fov_;
}

double Camera::getAspectRatio() const {
    return aspectRatio_;
}

double Camera::getNearClip() const {
    return nearClip_;
}

double Camera::getFarClip() const {
    return farClip_;
}

void Camera::setPosition(const Vector3 &position) {
    position_ = position;
}

void Camera::setRotation(const Quaternion &rotation) {
    rotation_ = rotation;
}

void Camera::setFov(double fov) {
    fov_ = fov;
}

void Camera::setAspectRatio(double aspectRatio) {
    aspectRatio_ = aspectRatio;
}

void Camera::setNearClip(double nearClip) {
    nearClip_ = nearClip;
}

void Camera::setFarClip(double farClip) {
    farClip_ = farClip;
}
