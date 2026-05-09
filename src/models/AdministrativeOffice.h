#ifndef ADMINISTRATIVEOFFICE_H
#define ADMINISTRATIVEOFFICE_H

#include "OrganizationUnit.h"

class AdministrativeOffice : public OrganizationUnit {
public:
    AdministrativeOffice(int unitId, const std::string& name);
    std::string getType() const override;
};

#endif
