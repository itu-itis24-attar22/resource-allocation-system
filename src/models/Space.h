#ifndef SPACE_H
#define SPACE_H

#include <string>

class Space {
protected:
    int id;
    std::string name;
    int capacity;

public:
    Space(int id, const std::string& name, int capacity);
    virtual ~Space() = default;

    int getId() const;
    std::string getName() const;
    int getCapacity() const;

    virtual std::string getType() const = 0;
};

#endif