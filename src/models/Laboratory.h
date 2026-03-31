#ifndef LABORATORY_H
#define LABORATORY_H

#include "Space.h"

class Laboratory : public Space {
public:
    Laboratory(int id, const std::string& name, int capacity,
               bool hasProjector = false,
               bool hasWhiteboard = false,
               bool hasComputers = false,
               bool isAvailable = true);

    std::string getType() const override;
};

#endif