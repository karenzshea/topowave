#ifndef hite_TILE_HPP
#define hite_TILE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <regex>
#include <string>
extern "C" {
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
};

namespace hite
{

// TODO encode way to return no data for Elevation
using Elevation = std::int16_t;
constexpr const int MAX_TILE_SIZE = 3601;
constexpr const double MAX_DOUBLE = std::numeric_limits<double>::max();
constexpr const int MAX_INT = std::numeric_limits<int>::max();
constexpr const int NUM_DEGREE_TILES = 360 * 180;

struct Coordinate {
    Coordinate() = default;
    Coordinate(double Lon, double Lat)
        : Longitude(Lon), Latitude(Lat) {
    }
    double Longitude = MAX_DOUBLE;
    double Latitude = MAX_DOUBLE;
};

// e.g. 13.415852,52.485185 => 0.415852,0.485185
struct TileCoordinate {
    TileCoordinate() : U(MAX_DOUBLE), V(MAX_DOUBLE)
    {
    }
    TileCoordinate(double u, double v)
        : U(u), V(v) {
    }
    double U; // column
    double V; // row
};

struct PixelCoordinate {
    PixelCoordinate(int x, int y)
        : X(x), Y(y) {
    }
    int X; // column
    int Y; // row
};

TileCoordinate GetTileCoordinate(const Coordinate &coord);

struct ElevationTile {
    ElevationTile(int _x, int _y, const char* filepath);
    ElevationTile(const char* filepath);
    ElevationTile(int _x, int _y);
    ElevationTile();
    ~ElevationTile();
    // methods
    Elevation GetElevation(const Coordinate &coord);
    Elevation GetInterpolatedData(const TileCoordinate &tile_coord);
    Elevation GetPixelData(const PixelCoordinate &pixel_coord);

    // members
    int x = MAX_INT;
    int y = MAX_INT;
    //std::vector<unsigned char> elevation{(MAX_TILE_SIZE * MAX_TILE_SIZE) * sizeof(int16_t)};
    std::vector<unsigned char> elevation;
    char *map = nullptr;
    // Q how to define default value for struct stat?
    struct stat file_stat;

    private:
    bool InsideTile(const Coordinate &coordinate);
    int fd;
};
}

#endif