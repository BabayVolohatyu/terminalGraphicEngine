#ifndef GNOMETERMINALWAYLAND_H
#define GNOMETERMINALWAYLAND_H

#include <terminal/gnomeTerminal.h>

namespace terminal {

class GNOMETerminalWayland:public GNOMETerminal {
  private:
    inline static constexpr const char* getDisplayResolution =
     "gdbus call --session --dest org.gnome.Mutter.DisplayConfig "
     "--object-path /org/gnome/Mutter/DisplayConfig "
     "--method org.gnome.Mutter.DisplayConfig.GetCurrentState "
     "| grep -oP \"\\('.*?@.*?', \\K\\d+, \\d+\" "
     "| head -n1 "
     "| sed 's/,//g'";
    inline static constexpr const char* getTextScalingFactor =
        "gsettings get org.gnome.desktop.interface text-scaling-factor";

  public:
    void rescale(double scale) override;

    void resize(int rows, int cols) override;

    void maximizeTerminal();

    static std::pair<int, int> getRowsAndCols();

    static std::pair<int, int> getResolution();
};
}

#endif