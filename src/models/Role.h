#ifndef ROLE_H
#define ROLE_H

#include <string>
#include <vector>

class Role {
private:
    int roleId;
    std::string roleName;
    std::vector<std::string> permissions;

public:
    Role(int roleId,
         const std::string& roleName,
         const std::vector<std::string>& permissions = {});

    int getId() const;
    std::string getName() const;
    std::vector<std::string> getPermissions() const;
};

#endif
