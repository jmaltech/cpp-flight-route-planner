#include "flightMap.h"

#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string citiesFile = "data/cities.dat";
    std::string flightsFile = "data/flights.dat";
    std::string requestsFile = "data/requests.dat";

    if (argc == 4) {
        citiesFile = argv[1];
        flightsFile = argv[2];
        requestsFile = argv[3];
    } else if (argc != 1) {
        std::cerr << "Usage: " << argv[0]
                  << " [cities.dat flights.dat requests.dat]\n";
        return 1;
    }

    std::ifstream cities(citiesFile);
    std::ifstream flights(flightsFile);
    std::ifstream requests(requestsFile);

    if (!cities || !flights || !requests) {
        std::cerr << "Unable to open one or more data files.\n";
        return 1;
    }

    FlightMapClass flightMap;
    flightMap.ReadCities(cities);
    flightMap.BuildMap(flights);

    flightMap.DisplayMap();
    std::cout << '\n';

    std::string origin;
    std::string destination;
    while (requests >> origin >> destination) {
        flightMap.FindPath(origin, destination);
        std::cout << '\n';
    }

    return 0;
}
