#include "flightMap.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <stack>
#include <stdexcept>

FlightMapClass::FlightMapClass() : size(0) {}

FlightMapClass::FlightMapClass(const FlightMapClass& f) = default;

FlightMapClass::~FlightMapClass() = default;

void FlightMapClass::ReadCities(std::ifstream& myCities) {
    cities.clear();

    int cityCount = 0;
    myCities >> cityCount;

    std::string city;
    while (myCities >> city) {
        cities.push_back(city);
    }

    std::sort(cities.begin(), cities.end());
    size = static_cast<int>(cities.size());

    if (cityCount != size) {
        std::cerr << "Warning: city count in file does not match parsed cities.\n";
    }

    map.assign(size, std::list<flightRec>());
    visited.assign(size, false);
}

void FlightMapClass::BuildMap(std::ifstream& myFlights) {
    if (size == 0) {
        throw std::runtime_error("ReadCities must be called before BuildMap.");
    }

    for (auto& routes : map) {
        routes.clear();
    }

    flightRec flight;
    while (myFlights >> flight.flightNum >> flight.origin >> flight.destination >> flight.price) {
        int originIndex = GetCityNumber(flight.origin);
        if (originIndex >= 0 && CheckCity(flight.destination)) {
            map[originIndex].push_back(flight);
        }
    }

    for (auto& routes : map) {
        routes.sort();
    }
}

void FlightMapClass::DisplayMap() {
    std::cout << "Origin              Destination        Flight Price\n";
    std::cout << "========================================================\n";

    for (int i = 0; i < size; ++i) {
        if (map[i].empty()) {
            continue;
        }

        bool first = true;
        for (const auto& flight : map[i]) {
            if (first) {
                std::cout << "From " << cities[i] << " to: ";
                first = false;
            } else {
                std::cout << std::string(cities[i].size() + 10, ' ');
            }

            std::cout << std::left << std::setw(18) << flight.destination
                      << std::right << std::setw(6) << flight.flightNum
                      << "  $" << std::setw(4) << flight.price << '\n';
        }
    }
}

bool FlightMapClass::CheckCity(std::string cityName) const {
    return std::binary_search(cities.begin(), cities.end(), cityName);
}

void FlightMapClass::DisplayAllCities() const {
    for (const auto& city : cities) {
        std::cout << city << '\n';
    }
}

void FlightMapClass::MarkVisited(int city) {
    if (city >= 0 && city < size) {
        visited[city] = true;
    }
}

void FlightMapClass::UnvisitAll() {
    std::fill(visited.begin(), visited.end(), false);
}

bool FlightMapClass::IsVisited(int city) const {
    return city >= 0 && city < size && visited[city];
}

bool FlightMapClass::GetNextCity(std::string fromCity, std::string& nextCity) {
    int fromIndex = GetCityNumber(fromCity);
    if (fromIndex < 0) {
        return false;
    }

    for (const auto& flight : map[fromIndex]) {
        int destinationIndex = GetCityNumber(flight.destination);
        if (destinationIndex >= 0 && !IsVisited(destinationIndex)) {
            nextCity = flight.destination;
            return true;
        }
    }

    return false;
}

int FlightMapClass::GetCityNumber(std::string cityName) const {
    auto it = std::lower_bound(cities.begin(), cities.end(), cityName);
    if (it == cities.end() || *it != cityName) {
        return -1;
    }
    return static_cast<int>(std::distance(cities.begin(), it));
}

std::string FlightMapClass::GetCityName(int cityNumber) const {
    if (cityNumber < 0 || cityNumber >= size) {
        return "";
    }
    return cities[cityNumber];
}

void FlightMapClass::FindPath(std::string originCity, std::string destinationCity) {
    std::cout << "Request is to fly from " << originCity << " to " << destinationCity << ".\n";

    if (!CheckCity(originCity) || !CheckCity(destinationCity)) {
        std::cout << "Sorry, EastWest airline does not fly from "
                  << originCity << " to " << destinationCity << ".\n";
        return;
    }

    const int origin = GetCityNumber(originCity);
    const int destination = GetCityNumber(destinationCity);

    UnvisitAll();

    std::vector<int> parent(size, -1);
    std::vector<flightRec> parentFlight(size);
    std::stack<int> routeStack;

    routeStack.push(origin);
    MarkVisited(origin);

    bool found = false;

    while (!routeStack.empty()) {
        int current = routeStack.top();

        if (current == destination) {
            found = true;
            break;
        }

        bool advanced = false;
        for (const auto& flight : map[current]) {
            int next = GetCityNumber(flight.destination);
            if (next >= 0 && !IsVisited(next)) {
                MarkVisited(next);
                parent[next] = current;
                parentFlight[next] = flight;
                routeStack.push(next);
                advanced = true;
                break;
            }
        }

        if (!advanced) {
            routeStack.pop();
        }
    }

    if (!found) {
        std::cout << "Sorry, EastWest airline does not fly from "
                  << originCity << " to " << destinationCity << ".\n";
        return;
    }

    std::vector<flightRec> itinerary;
    int cursor = destination;
    while (cursor != origin) {
        itinerary.push_back(parentFlight[cursor]);
        cursor = parent[cursor];
    }
    std::reverse(itinerary.begin(), itinerary.end());

    std::cout << "EastWest airline serves between these two cities.\n";
    std::cout << "The flight itinerary is:\n";
    std::cout << "Flight # From                 To                   Cost\n";
    std::cout << "--------------------------------------------------------\n";

    int total = 0;
    for (const auto& flight : itinerary) {
        std::cout << std::left << std::setw(9) << flight.flightNum
                  << std::setw(21) << flight.origin
                  << std::setw(21) << flight.destination
                  << "$" << std::right << std::setw(4) << flight.price << '\n';
        total += flight.price;
    }

    std::cout << "--------------------------------------------------------\n";
    std::cout << "Total: $" << total << "\n";
}
