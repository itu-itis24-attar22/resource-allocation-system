#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "User.h"

class Administrator : public User {
public:
    Administrator(int userId, const std::string& name);

    std::string getRoleName() const override;
    int getPriority() const override;
    bool canRequestSpaceType(const std::string& spaceType) const override;
    bool canSubmitRequestType(const std::string& requestType) const override;
};

#endif
