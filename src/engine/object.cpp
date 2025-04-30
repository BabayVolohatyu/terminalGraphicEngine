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
    translation[0][3] = _position.getX();
    translation[1][3] = _position.getY();
    translation[2][3] = _position.getZ();
    return translation;
}

Object::Object()
: _position{0, 0, 0}, _rotation{1, 0, 0, 0}{}

Object::Object(const Vector3 &position, const Quaternion &rotation)
: _position{position}, _rotation{rotation}{}

void Object::addVertex(const Point &vertex) {
    _vertices.push_back(vertex);
}

void Object::removeVertex(int index) {
    if (index < _vertices.size() && index >= 0) {
        // First remove all triangles containing this vertex
        for (int i = 0; i < _triangles.size(); ) { // notice no i++
            bool triangleHasVertex = false;
            for (const Point* p : _triangles[i].vertices) {
                if (&_vertices[index] == p) {
                    triangleHasVertex = true;
                    break;
                }
            }
            if (triangleHasVertex) {
                _triangles.erase(_triangles.begin() + i); // don't increment i
            } else {
                i++; // increment only if no erase
            }
        }

        // Now remove the vertex itself
        _vertices.erase(_vertices.begin() + index);
    }
}

void Object::makeTriangle(int v1, int v2, int v3) {
    if (v1 >= 0 && v1 < _vertices.size() &&
          v2 >= 0 && v2 < _vertices.size() &&
          v3 >= 0 && v3 < _vertices.size())
    {
        _triangles.emplace_back(&_vertices[v1], &_vertices[v2], &_vertices[v3]);
    }
}

void Object::rotate(const Quaternion &rotationQuaternion) {
    _rotation = rotationQuaternion * _rotation;
    for (Point &p: _vertices) {
        Vector3 localVertex(p.position.getX() - _position.getX(),
            p.position.getY() - _position.getY(),
            p.position.getZ() - _position.getZ());
        Vector3 rotatedVertex = _rotation * localVertex;
        p.position =rotatedVertex + _position;
    }
}

Matrix<double> Object::getModelMatrix() const {
    Matrix<double> translationMatrix = getTranslationMatrix();
    Matrix<double> rotationMatrix = Quaternion::toMatrix(_rotation);
    return translationMatrix*rotationMatrix;
}

std::vector<Point> Object::vertices() const {
    return _vertices;
}

std::vector<Triangle> Object::triangles() const {
    return _triangles;
}

Vector3 Object::position() const {
    return _position;
}

Quaternion Object::rotation() const {
    return _rotation;
}

void Object::setPosition(const Vector3 &position) {
    Vector3 translation = position - _position;
    for (Point &p: _vertices) {
        p.position.setX(p.position.getX() + translation.getX());
        p.position.setY(p.position.getY() + translation.getY());
        p.position.setZ(p.position.getZ() + translation.getZ());
    }
    _position = position;
}

void Object::setPos(double x, double y, double z) {
    Vector3 newPos(x, y, z);
    Vector3 translation = newPos - _position;
    for (Point &p: _vertices) {
        p.position.setX(p.position.getX() + translation.getX());
        p.position.setY(p.position.getY() + translation.getY());
        p.position.setZ(p.position.getZ() + translation.getZ());
    }
    _position = newPos;
}

void Object::setRot(const Quaternion &rotation) {
    _rotation = Quaternion(1, 0, 0, 0);
    rotate(rotation);
}

