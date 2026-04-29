#ifndef INSTRUCTOR_H
#define INSTRUCTOR_H

#include "User.h"

class Instructor : public User {
public:
    Instructor(int userId, const std::string& name);

    std::string getRoleName() const override;
    int getPriority() const override;
    bool canRequestSpaceType(const std::string& spaceType) const override;
};

#endif
