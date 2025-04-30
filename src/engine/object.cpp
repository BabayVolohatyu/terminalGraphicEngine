#include <engine/object.h>

using namespace engine;

Point::Point()
:position{0,0,0}{}

Point::Point(const Vector3 &initPosition)
:position{initPosition} {}

Point::Point(double x, double y, double z) {
    position = Vector3{x, y, z};
}

Triangle::Triangle(const Point *p1, const Point *p2, const Point *p3) {
    vertices[0] = p1;
    vertices[1] = p2;
    vertices[2] = p3;
}

Vector3 Triangle::calculateNormal() const {
    Vector3 edgeVector1{vertices[1]->position - vertices[0]->position};
    Vector3 edgeVector2{vertices[2]->position - vertices[0]->position};
    Vector3 normal = edgeVector1.cross(edgeVector2);
    return normal.normalized();
}

Matrix<double> Object::getTranslationMatrix() const {
    Matrix<double> translation(4, 4);
    for (int i = 0; i < 4; i++) {
        translation[i][i] = 1;
    }
    translation[0][3] = position_.getX();
    translation[1][3] = position_.getY();
    translation[2][3] = position_.getZ();
    return translation;
}

Object::Object()
: position_{0, 0, 0}, rotation_{1, 0, 0, 0}{}

Object::Object(const Vector3 &position, const Quaternion &rotation)
: position_{position}, rotation_{rotation}{}

void Object::addVertex(const Point &vertex) {
    vertices_.push_back(vertex);
}

void Object::removeVertex(int index) {
    if (index < vertices_.size() && index >= 0) {
        // First remove all triangles containing this vertex
        for (int i = 0; i < triangles_.size(); ) { // notice no i++
            bool triangleHasVertex = false;
            for (const Point* p : triangles_[i].vertices) {
                if (&vertices_[index] == p) {
                    triangleHasVertex = true;
                    break;
                }
            }
            if (triangleHasVertex) {
                triangles_.erase(triangles_.begin() + i); // don't increment i
            } else {
                i++; // increment only if no erase
            }
        }

        // Now remove the vertex itself
        vertices_.erase(vertices_.begin() + index);
    }
}

void Object::makeTriangle(int v1, int v2, int v3) {
    if (v1 >= 0 && v1 < vertices_.size() &&
          v2 >= 0 && v2 < vertices_.size() &&
          v3 >= 0 && v3 < vertices_.size())
    {
        triangles_.emplace_back(&vertices_[v1], &vertices_[v2], &vertices_[v3]);
    }
}

void Object::rotate(const Quaternion &rotationQuaternion) {
    rotation_ = rotationQuaternion * rotation_;
    for (Point &p: vertices_) {
        Vector3 localVertex(p.position.getX() - position_.getX(),
            p.position.getY() - position_.getY(),
            p.position.getZ() - position_.getZ());
        Vector3 rotatedVertex = rotation_ * localVertex;
        p.position =rotatedVertex + position_;
    }
}

Matrix<double> Object::getModelMatrix() const {
    Matrix<double> translationMatrix = getTranslationMatrix();
    Matrix<double> rotationMatrix = Quaternion::toMatrix(rotation_);
    return translationMatrix*rotationMatrix;
}

std::vector<Point> Object::vertices() const {
    return vertices_;
}

std::vector<Triangle> Object::triangles() const {
    return triangles_;
}

Vector3 Object::position() const {
    return position_;
}

Quaternion Object::rotation() const {
    return rotation_;
}

void Object::setPosition(const Vector3 &position) {
    Vector3 translation = position - position_;
    for (Point &p: vertices_) {
        p.position.setX(p.position.getX() + translation.getX());
        p.position.setY(p.position.getY() + translation.getY());
        p.position.setZ(p.position.getZ() + translation.getZ());
    }
    position_ = position;
}

void Object::setPos(double x, double y, double z) {
    Vector3 newPos(x, y, z);
    Vector3 translation = newPos - position_;
    for (Point &p: vertices_) {
        p.position.setX(p.position.getX() + translation.getX());
        p.position.setY(p.position.getY() + translation.getY());
        p.position.setZ(p.position.getZ() + translation.getZ());
    }
    position_ = newPos;
}

void Object::setRot(const Quaternion &rotation) {
    rotation_ = Quaternion(1, 0, 0, 0);
    rotate(rotation);
}

