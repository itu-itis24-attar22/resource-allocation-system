#ifndef TEACHINGASSISTANT_H
#define TEACHINGASSISTANT_H

#include "User.h"

class TeachingAssistant : public User {
private:
    std::string assistantType;

public:
    TeachingAssistant(int userId,
                      const std::string& name,
                      const std::string& email = "",
                      const std::string& status = "active",
                      const std::string& assistantType = "",
                      const std::string& primaryUnitName = "",
                      std::vector<std::string> assignedRoleNames = {});

    std::string getAssistantType() const;
    std::string getRoleName() const override;
    int getPriority() const override;
    bool canRequestSpaceType(const std::string& spaceType) const override;
    bool canSubmitRequestType(const std::string& requestType) const override;
};

#endif
