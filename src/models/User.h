#ifndef USER_H
#define USER_H

#include <string>

class User {
private:
    int id;
    std::string name;
    std::string role;

public:
    User(int id, const std::string& name, const std::string& role);

    int getId() const;
    std::string getName() const;
    std::string getRole() const;
};

#endif