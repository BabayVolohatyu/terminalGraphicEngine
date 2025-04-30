#include <terminal/gnomeTerminalWayland.h>
#include <sstream>
using namespace terminal;

void GNOMETerminalWayland::rescale(double scale) {
    std::string command = "gsettings set org.gnome.desktop.interface text-scaling-factor " + std::to_string(scale);
    system(command.c_str());
}

void GNOMETerminalWayland::resize(int rows, int cols) {
    std::string command = "printf '\\e[8;" + std::to_string(rows) + ";" + std::to_string(cols) + "t'";
    system(command.c_str());
}

std::pair<int, int> GNOMETerminalWayland::getRowsAndCols() {
    std::istringstream issResolution(getCommandOutput(getDisplayResolution));
    std::istringstream issScale(getCommandOutput(getTextScalingFactor));
    int width, height;
    issResolution >> width >> height;
    double scale;
    issScale >> scale;
    int rows = static_cast<int>(static_cast<double>(height) / (19.29 * scale));
    int cols = static_cast<int>(static_cast<double>(width) / (9.05 * scale));
    return {rows, cols};
}

std::pair<int, int> GNOMETerminalWayland::getResolution() {
    std::istringstream issResolution(getCommandOutput(getDisplayResolution));
    int width, height;
    issResolution >> width >> height;
    return {width, height};
}

void GNOMETerminalWayland::maximizeTerminal() {
    auto [rows,cols] = getRowsAndCols();
    resize(rows, cols);
}