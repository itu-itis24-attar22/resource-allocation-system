#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    int userId;
    std::string name;

public:
    User(int userId, const std::string& name);
    virtual ~User() = default;

    int getId() const;
    std::string getName() const;

    virtual std::string getRoleName() const = 0;
    virtual int getPriority() const = 0;
    virtual bool canRequestSpaceType(const std::string& spaceType) const = 0;
};

#endif
