#ifndef SPACEFACTORY_H
#define SPACEFACTORY_H

#include <string>
#include "Space.h"

class SpaceFactory {
public:
    static Space* createSpace(int id,
                              const std::string& name,
                              const std::string& type,
                              int capacity,
                              bool hasProjector,
                              bool hasWhiteboard,
                              bool hasComputers,
                              bool isAvailable,
                              const std::string& building);
};

#endif
