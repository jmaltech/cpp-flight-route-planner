CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -pedantic
TARGET := flight_route_planner
SOURCES := main.cpp flightMap.cpp type.cpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES) flightMap.h type.h
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o
