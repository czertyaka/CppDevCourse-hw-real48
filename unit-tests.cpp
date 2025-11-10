#include <ostream>
#include <iostream>

#include "real48.hpp"

using math::Real48;

std::ostream& operator<<(std::ostream& os, const Real48 number) {
    os << double{number};
    return os;
}

int main() {
    std::cout << Real48{1e100} << std::endl;
}
