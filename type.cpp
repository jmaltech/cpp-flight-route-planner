#include "type.h"

#include <iomanip>

bool flightRec::operator<(const flightRec& rhs) const {
    if (destination != rhs.destination) {
        return destination < rhs.destination;
    }
    return flightNum < rhs.flightNum;
}

bool flightRec::operator==(const flightRec& rhs) const {
    return origin == rhs.origin && destination == rhs.destination;
}

std::ostream& operator<<(std::ostream& os, const flightRec& f) {
    os << std::left << std::setw(18) << f.destination
       << std::right << std::setw(6) << f.flightNum
       << "  $" << std::setw(4) << f.price;
    return os;
}
