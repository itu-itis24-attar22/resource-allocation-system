#ifndef SPACE_H
#define SPACE_H

#include <string>

class Space {
protected:
    int id;
    std::string name;
    int capacity;
    bool hasProjector;
    bool hasWhiteboard;
    bool hasComputers;

public:
    Space(int id, const std::string& name, int capacity,
          bool hasProjector = false,
          bool hasWhiteboard = false,
          bool hasComputers = false);

    int getId() const;
    std::string getName() const;
    int getCapacity() const;

    bool getHasProjector() const;
    bool getHasWhiteboard() const;
    bool getHasComputers() const;

    bool hasFeature(const std::string& feature) const;

    virtual std::string getType() const = 0;
    virtual ~Space() = default;
};

#endif