#ifndef CLASSROOM_H
#define CLASSROOM_H

#include "Space.h"

class Classroom : public Space {
public:
    Classroom(int id, const std::string& name, int capacity,
              bool hasProjector = false,
              bool hasWhiteboard = false,
              bool hasComputers = false,
              bool isAvailable = true);

    std::string getType() const override;
};

#endif