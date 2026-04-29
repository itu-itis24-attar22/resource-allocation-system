#ifndef STAFF_H
#define STAFF_H

#include "User.h"

class Staff : public User {
public:
    Staff(int userId, const std::string& name);

    std::string getRoleName() const override;
    int getPriority() const override;
    bool canRequestSpaceType(const std::string& spaceType) const override;
};

#endif
