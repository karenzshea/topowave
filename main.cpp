#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
extern "C" {
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
};

#include "tile_index.hpp"
#include "tile.hpp"

int main(const int argc, const char* argv[]) {
    // Expecting file like 'N52E013.hgt'
    if (argc < 2) throw std::runtime_error("Missing argument file or path to files");
    const char* filepath = argv[1];
    struct stat filepath_stat;
    if (stat(filepath, &filepath_stat) == -1)
    {
        throw std::runtime_error("Could not stat file or file path");
    }

    std::vector<const char*> files;
    // handle either a single file or a path to a directory
    if ((filepath_stat.st_mode & S_IFMT) == S_IFREG)
    {
        files.push_back(std::move(filepath));
    }
    else if ((filepath_stat.st_mode & S_IFMT) == S_IFDIR)
    {
        hite::readFileDir(filepath, files);
    }
    std::cout << "Reading " << files.size() << " path(s)..." << std::endl;

    // TODO read tile data into tile index structure for querying
    // i.e. read all tilepaths into elevation tile instances, and provide
    // an interface for querying a tile
    std::size_t i = 0;
    for (; i < files.size(); i++)
    {
        char fullpath[strlen(filepath) + 1 + strlen(files[i])];
        strcpy(fullpath, filepath);
        strcat(fullpath, "/");
        strcat(fullpath, files[i]);
        std::cout << fullpath << std::endl;
        hite::ElevationTile tile(fullpath);

        hite::Coordinate top_of_hill(13.414049, 52.550679);
        hite::Elevation top = tile.GetElevation(top_of_hill);
        hite::Coordinate bottom_of_hill(13.409757, 52.528493);
        hite::Elevation bottom = tile.GetElevation(bottom_of_hill);
        std::cout << top << std::endl;
        std::cout << bottom << std::endl;
    }
}