#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

class User {
protected:
    int userId;
    std::string name;
    std::string email;
    std::string status;
    std::string primaryUnitName;
    std::vector<std::string> assignedRoleNames;

public:
    User(int userId,
         const std::string& name,
         const std::string& email = "",
         const std::string& status = "active",
         const std::string& primaryUnitName = "",
         std::vector<std::string> assignedRoleNames = {});
    virtual ~User() = default;

    int getId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getStatus() const;
    std::string getPrimaryUnitName() const;
    std::vector<std::string> getAssignedRoleNames() const;

    virtual std::string getRoleName() const = 0;
    virtual int getPriority() const = 0;
    virtual bool canRequestSpaceType(const std::string& spaceType) const = 0;
    virtual bool canSubmitRequestType(const std::string& requestType) const = 0;
};

#endif
