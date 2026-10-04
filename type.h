#ifndef TYPE_H
#define TYPE_H

#include <iostream>
#include <string>

struct flightRec {
    std::string origin;
    int flightNum;
    std::string destination;
    int price;

    bool operator<(const flightRec& rhs) const;
    bool operator==(const flightRec& rhs) const;

    friend std::ostream& operator<<(std::ostream& os, const flightRec& f);
};

#endif
