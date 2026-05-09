#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "OrganizationUnit.h"

class Department : public OrganizationUnit {
public:
    Department(int unitId, const std::string& name);
    std::string getType() const override;
};

#endif
