#include "User.h"

User::User(int id, const std::string& name, const std::string& role)
    : id(id), name(name), role(role) {}

int User::getId() const {
    return id;
}

std::string User::getName() const {
    return name;
}

std::string User::getRole() const {
    return role;
}