#ifndef ORGANIZATIONUNIT_H
#define ORGANIZATIONUNIT_H

#include <string>

class OrganizationUnit {
private:
    int unitId;
    std::string name;
    std::string unitType;

public:
    OrganizationUnit(int unitId,
                     const std::string& name,
                     const std::string& unitType);
    virtual ~OrganizationUnit() = default;

    int getId() const;
    std::string getName() const;
    std::string getUnitType() const;
    virtual std::string getType() const;
};

#endif
