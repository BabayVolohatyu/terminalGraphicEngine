#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>

namespace terminal {

class Terminal {
  protected:
    virtual void rescale(double scale) = 0;

    virtual void resize(int rows, int cols) = 0;

    static std::string getCommandOutput(const char* cmd);

    virtual ~Terminal() = default;
};
}
#endif