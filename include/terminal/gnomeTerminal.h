#ifndef GNOMETERMINAL_H
#define GNOMETERMINAL_H

#include <terminal/terminal.h>

namespace terminal {

class GNOMETerminal: public Terminal {
  public:
    void rescale(double scale) override = 0;

    void resize(int rows, int cols) override = 0;
};
}

#endif