#ifndef USERFACTORY_H
#define USERFACTORY_H

#include <string>
#include "User.h"

class UserFactory {
public:
    static User* createUser(int userId, const std::string& name, const std::string& roleName);
};

#endif
