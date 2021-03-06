#ifndef hite_TILE_HPP
#define hite_TILE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <memory>
extern "C" {
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
};

#include "constants.hpp"
#include "mem_mapper.hpp"

namespace hite
{

struct IntCoordinate {
    IntCoordinate() = default;
    IntCoordinate(double Lon, double Lat)
    {
        // is it OK to implicitly narrow double -> int?
        if (Lon < 180.0 && Lon > -180.0)
        {
            Longitude = Lon;
        }
        if (Lat < 90.0 && Lat > -90.0)
        {
            Latitude = Lat;
        }
    }
    IntCoordinate(int Lon, int Lat)
    {
        if (Lon < 180 && Lon > -180)
        {
            Longitude = Lon;
        }
        if (Lat < 90 && Lat > -90)
        {
            Latitude = Lat;
        }
    }
    int Longitude = MAX_INT;
    int Latitude = MAX_INT;

    bool IsValid();
};

struct Coordinate {
    Coordinate() = default;
    Coordinate(double Lon, double Lat)
    {
        if (Lon < 180.0 && Lon > -180.0)
        {
            Longitude = Lon;
        }
        if (Lat < 90.0 && Lat > -90.0)
        {
            Latitude = Lat;
        }
    }
    double Longitude = MAX_DOUBLE;
    double Latitude = MAX_DOUBLE;
};

// e.g. 13.415852,52.485185 => 0.415852,0.485185
struct CoordinateDecimal {
    CoordinateDecimal() : U(MAX_DOUBLE), V(MAX_DOUBLE)
    {
    }
    CoordinateDecimal(double u, double v)
        : U(u), V(v) {
    }
    double U; // column
    double V; // row
};

bool operator==(const CoordinateDecimal& lhs, const CoordinateDecimal& rhs);

struct PixelCoordinate {
    PixelCoordinate(int x, int y)
        : X(x), Y(y) {
    }
    int X; // column
    int Y; // row
};

CoordinateDecimal GetCoordinateDecimal(const Coordinate &coord);

struct ElevationTile {
    ElevationTile& operator=(ElevationTile&&) = default;
    ElevationTile(ElevationTile&&) = default;
    ElevationTile(int _x, int _y, const std::string &filepath);
    ElevationTile();
    ~ElevationTile();
    ElevationTile& operator=(const ElevationTile&) = delete;
    ElevationTile(const ElevationTile&) = delete;

    // methods
    Elevation GetPixelData(const PixelCoordinate &pixel_coord);
    bool isValid();

    // members
    // Q what are these for?
    int x = MAX_INT;
    int y = MAX_INT;

    private:
    bool InsideTile(const Coordinate &coordinate);
    int fd;
    MemMap mm;
};

}

#endif
