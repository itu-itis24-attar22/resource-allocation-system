#include "Role.h"

Role::Role(int roleId,
           const std::string& roleName,
           const std::vector<std::string>& permissions)
    : roleId(roleId), roleName(roleName), permissions(permissions) {}

int Role::getId() const {
    return roleId;
}

std::string Role::getName() const {
    return roleName;
}

std::vector<std::string> Role::getPermissions() const {
    return permissions;
}
