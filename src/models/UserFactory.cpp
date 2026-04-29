#include "UserFactory.h"
#include "Student.h"
#include "Instructor.h"
#include "Staff.h"
#include "Administrator.h"

User* UserFactory::createUser(int userId, const std::string& name, const std::string& roleName) {
    if (roleName == "Student") {
        return new Student(userId, name);
    }

    if (roleName == "Instructor") {
        return new Instructor(userId, name);
    }

    if (roleName == "Staff") {
        return new Staff(userId, name);
    }

    if (roleName == "Administrator") {
        return new Administrator(userId, name);
    }

    return nullptr;
}
