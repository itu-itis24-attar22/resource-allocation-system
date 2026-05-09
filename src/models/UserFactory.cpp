#include "UserFactory.h"
#include "Student.h"
#include "Instructor.h"
#include "Staff.h"
#include "Administrator.h"
#include "TeachingAssistant.h"

User* UserFactory::createUser(int userId, const std::string& name, const std::string& roleName) {
    return createUser(userId, name, roleName, UserProfileData{});
}

User* UserFactory::createUser(int userId,
                              const std::string& name,
                              const std::string& roleName,
                              const UserProfileData& profile) {
    if (roleName == "Student") {
        return new Student(userId,
                           name,
                           profile.email,
                           profile.status,
                           profile.studentNo,
                           profile.program,
                           profile.yearLevel,
                           profile.primaryUnitName,
                           profile.assignedRoleNames);
    }

    if (roleName == "Instructor") {
        return new Instructor(userId,
                              name,
                              profile.email,
                              profile.status,
                              profile.title,
                              profile.officeRoom,
                              profile.primaryUnitName,
                              profile.assignedRoleNames);
    }

    if (roleName == "TeachingAssistant") {
        return new TeachingAssistant(userId,
                                     name,
                                     profile.email,
                                     profile.status,
                                     profile.assistantType,
                                     profile.primaryUnitName,
                                     profile.assignedRoleNames);
    }

    if (roleName == "Staff") {
        return new Staff(userId,
                         name,
                         profile.email,
                         profile.status,
                         profile.jobTitle,
                         profile.primaryUnitName,
                         profile.assignedRoleNames);
    }

    if (roleName == "Administrator") {
        return new Administrator(userId,
                                 name,
                                 profile.email,
                                 profile.status,
                                 profile.adminLevel,
                                 profile.primaryUnitName,
                                 profile.assignedRoleNames);
    }

    return nullptr;
}
