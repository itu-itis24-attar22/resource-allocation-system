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
    bool isAvailable;

public:
    Space(int id, const std::string& name, int capacity,
          bool hasProjector = false,
          bool hasWhiteboard = false,
          bool hasComputers = false,
          bool isAvailable = true);

    int getId() const;
    std::string getName() const;
    int getCapacity() const;

    bool getHasProjector() const;
    bool getHasWhiteboard() const;
    bool getHasComputers() const;

    bool getIsAvailable() const;
    void setAvailability(bool availability);

    bool hasFeature(const std::string& feature) const;

    virtual std::string getType() const = 0;
    virtual ~Space() = default;
};

#endif