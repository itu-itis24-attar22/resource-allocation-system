#include "User.h"

User::User(int userId, const std::string& name, UserRole role)
    : userId(userId), name(name), role(role) {}

int User::getId() const {
    return userId;
}

std::string User::getName() const {
    return name;
}

UserRole User::getRole() const {
    return role;
}