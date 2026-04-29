#include "User.h"

User::User(int userId, const std::string& name)
    : userId(userId), name(name) {}

int User::getId() const {
    return userId;
}

std::string User::getName() const {
    return name;
}
