// Marceli Buczek 339966

#include "traceroute.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Użycie: " << argv[0] << " <adres IP>" << std::endl;
        return 1;
    }
    Traceroute traceroute(argv[1]);
    traceroute.run();

    return 0;
}