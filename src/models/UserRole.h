#ifndef USERROLE_H
#define USERROLE_H

#include <string>

enum class UserRole {
    Student,
    Instructor,
    Staff,
    Administrator
};

inline std::string userRoleToString(UserRole role) {
    switch (role) {
        case UserRole::Student: return "Student";
        case UserRole::Instructor: return "Instructor";
        case UserRole::Staff: return "Staff";
        case UserRole::Administrator: return "Administrator";
        default: return "Unknown";
    }
}

inline UserRole stringToUserRole(const std::string& role) {
    if (role == "Student") return UserRole::Student;
    if (role == "Instructor") return UserRole::Instructor;
    if (role == "Staff") return UserRole::Staff;
    if (role == "Administrator") return UserRole::Administrator;

    return UserRole::Student;
}

inline int userRoleToPriority(UserRole role) {
    switch (role) {
        case UserRole::Administrator: return 4;
        case UserRole::Instructor: return 3;
        case UserRole::Staff: return 2;
        case UserRole::Student: return 1;
        default: return 0;
    }
}

#endif