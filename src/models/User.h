#ifndef USER_H
#define USER_H

#include <string>
#include "UserRole.h"

class User {
private:
    int userId;
    std::string name;
    UserRole role;

public:
    User(int userId, const std::string& name, UserRole role);

    int getId() const;
    std::string getName() const;
    UserRole getRole() const;
};

#endif