#ifndef FACULTY_H
#define FACULTY_H

#include "OrganizationUnit.h"

class Faculty : public OrganizationUnit {
public:
    Faculty(int unitId, const std::string& name);
    std::string getType() const override;
};

#endif
