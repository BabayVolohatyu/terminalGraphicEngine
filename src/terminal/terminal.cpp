#include <terminal/terminal.h>
#include <memory>

using namespace terminal;

std::string Terminal::getCommandOutput(const char *cmd) {
    std::string result;
    char buffer[128];

    std::unique_ptr<FILE, int(*)(FILE*)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }

    return result;
}
