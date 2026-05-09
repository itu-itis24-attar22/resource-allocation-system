#ifndef STAFF_H
#define STAFF_H

#include "User.h"

class Staff : public User {
private:
    std::string jobTitle;

public:
    Staff(int userId,
          const std::string& name,
          const std::string& email = "",
          const std::string& status = "active",
          const std::string& jobTitle = "",
          const std::string& primaryUnitName = "",
          std::vector<std::string> assignedRoleNames = {});

    std::string getJobTitle() const;
    std::string getRoleName() const override;
    int getPriority() const override;
    bool canRequestSpaceType(const std::string& spaceType) const override;
    bool canSubmitRequestType(const std::string& requestType) const override;
};

#endif
