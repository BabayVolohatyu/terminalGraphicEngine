#include <iostream>
#include <terminal/gnomeTerminalWayland.h>
#include <engine/camera.h>
#include <engine/object.h>

using namespace terminal;
using namespace engine;

Object getCube(int l) {
    Object cube;
    std::vector<Point> points;

    points.emplace_back(l, l, l);
    points.emplace_back(l, l, -l);
    points.emplace_back(l, -l, l);
    points.emplace_back(l, -l, -l);
    points.emplace_back(-l, l, l);
    points.emplace_back(-l, l, -l);
    points.emplace_back(-l, -l, l);
    points.emplace_back(-l, -l, -l);

    for (Point &p : points) {
        cube.addVertex(p);
    }

    // Front face (z = +z)
    cube.makeTriangle(0, 2, 4);
    cube.makeTriangle(2, 6, 4);

    // Back face (z = -z)
    cube.makeTriangle(1, 5, 3);
    cube.makeTriangle(3, 5, 7);

    // Top face (y = +y)
    cube.makeTriangle(0, 4, 1);
    cube.makeTriangle(1, 4, 5);

    // Bottom face (y = -y)
    cube.makeTriangle(2, 3, 6);
    cube.makeTriangle(3, 7, 6);

    // Right face (x = +x)
    cube.makeTriangle(0, 1, 2);
    cube.makeTriangle(1, 3, 2);

    // Left face (x = -x)
    cube.makeTriangle(4, 6, 5);
    cube.makeTriangle(5, 6, 7);

    return cube;
}

int main() {
    GNOMETerminalWayland terminal;
    terminal.maximizeTerminal();
    system("clear");
    terminal.rescale(0.5);

    auto [width, height] = GNOMETerminalWayland::getResolution();
    Camera camera(
        Vector3{0, 0, 20},
        Quaternion{0, 0, 0},
        90,
        static_cast<double>(width) / height,
        0.1,
        100
    );
    camera.lookAt(Point(0, 0, 0));

    Object object1 = getCube(4);

    auto [rows, cols] = GNOMETerminalWayland::getRowsAndCols();

    Vector3 xAxis = Vector3{1, 0, 0};
    Vector3 yAxis = Vector3{0, 1, 0};
    Vector3 zAxis = Vector3{0, 0, 1};
    for (int i = 1; i < 480; i++) {
        if (i <= 180)object1.rotate(Quaternion::fromAxis(xAxis, 1));
        else if (i <= 360) object1.rotate(Quaternion::fromAxis(yAxis,1));
        else object1.rotate(Quaternion::fromAxis(zAxis,1));
        system("clear");
        camera.drawObject(object1, cols, rows);
        system("sleep 0.1");
    }

    system("clear");
    terminal.rescale(1.0);

    return 0;
}