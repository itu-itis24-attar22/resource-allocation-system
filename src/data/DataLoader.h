#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>
#include <vector>
#include "../models/User.h"
#include "../models/Space.h"

class DataLoader {
public:
    static std::vector<User*> loadUsers(const std::string& filename);
    static std::vector<Space*> loadSpaces(const std::string& filename);
};

#endif
