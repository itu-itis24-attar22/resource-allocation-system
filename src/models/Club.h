#ifndef CLUB_H
#define CLUB_H

#include "OrganizationUnit.h"

class Club : public OrganizationUnit {
public:
    Club(int unitId, const std::string& name);
    std::string getType() const override;
};

#endif
