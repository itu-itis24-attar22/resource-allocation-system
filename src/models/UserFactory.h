#ifndef USERFACTORY_H
#define USERFACTORY_H

#include <string>
#include <vector>
#include "User.h"

struct UserProfileData {
    std::string email;
    std::string status = "active";
    std::string studentNo;
    std::string program;
    int yearLevel = 0;
    std::string title;
    std::string officeRoom;
    std::string assistantType;
    std::string jobTitle;
    std::string adminLevel;
    std::string primaryUnitName;
    std::vector<std::string> assignedRoleNames;
};

class UserFactory {
public:
    static User* createUser(int userId, const std::string& name, const std::string& roleName);
    static User* createUser(int userId,
                            const std::string& name,
                            const std::string& roleName,
                            const UserProfileData& profile);
};

#endif
