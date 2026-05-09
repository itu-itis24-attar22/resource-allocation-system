#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"

class Student : public User {
private:
    std::string studentNo;
    std::string program;
    int yearLevel;

public:
    Student(int userId,
            const std::string& name,
            const std::string& email = "",
            const std::string& status = "active",
            const std::string& studentNo = "",
            const std::string& program = "",
            int yearLevel = 0,
            const std::string& primaryUnitName = "",
            std::vector<std::string> assignedRoleNames = {});

    std::string getStudentNo() const;
    std::string getProgram() const;
    int getYearLevel() const;
    std::string getRoleName() const override;
    int getPriority() const override;
    bool canRequestSpaceType(const std::string& spaceType) const override;
    bool canSubmitRequestType(const std::string& requestType) const override;
};

#endif
