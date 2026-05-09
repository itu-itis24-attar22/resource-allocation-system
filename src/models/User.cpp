#include "User.h"

User::User(int userId,
           const std::string& name,
           const std::string& email,
           const std::string& status,
           const std::string& primaryUnitName,
           std::vector<std::string> assignedRoleNames)
    : userId(userId),
      name(name),
      email(email),
      status(status.empty() ? "active" : status),
      primaryUnitName(primaryUnitName),
      assignedRoleNames(assignedRoleNames) {}

int User::getId() const {
    return userId;
}

std::string User::getName() const {
    return name;
}

std::string User::getEmail() const {
    return email;
}

std::string User::getStatus() const {
    return status;
}

std::string User::getPrimaryUnitName() const {
    return primaryUnitName;
}

std::vector<std::string> User::getAssignedRoleNames() const {
    return assignedRoleNames;
}
