#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "User.h"

class Administrator : public User {
private:
    std::string adminLevel;

public:
    Administrator(int userId,
                  const std::string& name,
                  const std::string& email = "",
                  const std::string& status = "active",
                  const std::string& adminLevel = "",
                  const std::string& primaryUnitName = "",
                  std::vector<std::string> assignedRoleNames = {});

    std::string getAdminLevel() const;
    std::string getRoleName() const override;
    int getPriority() const override;
    bool canRequestSpaceType(const std::string& spaceType) const override;
    bool canSubmitRequestType(const std::string& requestType) const override;
};

#endif
