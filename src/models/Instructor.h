#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

#include "User.h"

class Instructor : public User {
private:
    std::string title;
    std::string officeRoom;

public:
    Instructor(int userId,
               const std::string& name,
               const std::string& email = "",
               const std::string& status = "active",
               const std::string& title = "",
               const std::string& officeRoom = "",
               const std::string& primaryUnitName = "",
               std::vector<std::string> assignedRoleNames = {});

    std::string getTitle() const;
    std::string getOfficeRoom() const;
    std::string getRoleName() const override;
    int getPriority() const override;
    bool canRequestSpaceType(const std::string& spaceType) const override;
    bool canSubmitRequestType(const std::string& requestType) const override;
};

#endif
