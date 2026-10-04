#ifndef FLIGHTMAPCLASS_H
#define FLIGHTMAPCLASS_H

#include "type.h"

#include <fstream>
#include <list>
#include <string>
#include <vector>

class FlightMapClass {
public:
    FlightMapClass();
    FlightMapClass(const FlightMapClass& f);
    ~FlightMapClass();

    void ReadCities(std::ifstream& myCities);
    void BuildMap(std::ifstream& myFlights);
    void DisplayMap();

    bool CheckCity(std::string cityName) const;
    void DisplayAllCities() const;
    void MarkVisited(int city);
    void UnvisitAll();
    bool IsVisited(int city) const;
    bool GetNextCity(std::string fromCity, std::string& nextCity);
    int GetCityNumber(std::string cityName) const;
    std::string GetCityName(int cityNumber) const;
    void FindPath(std::string originCity, std::string destinationCity);

private:
    int size;
    std::vector<std::string> cities;
    std::vector<std::list<flightRec>> map;
    std::vector<bool> visited;
};

#endif
