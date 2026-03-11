#ifndef CLASSROOM_H
#define CLASSROOM_H

#include <string>

class Classroom {
private:
    int id;
    std::string name;
    int capacity;

public:
    Classroom(int id, const std::string& name, int capacity);

    int getId() const;
    std::string getName() const;
    int getCapacity() const;
};

#endif