#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "../models/Classroom.h"
#include "../models/Laboratory.h"
#include "../models/MeetingRoom.h"
#include "../models/UserRole.h"

std::vector<User> DataLoader::loadUsers(const std::string& filename) {
    std::vector<User> users;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open users file: " << filename << "\n";
        return users;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;

        int id;
        std::string name;
        std::string roleString;

        std::getline(ss, token, ',');
        id = std::stoi(token);

        std::getline(ss, name, ',');
        std::getline(ss, roleString, ',');

        users.emplace_back(id, name, stringToUserRole(roleString));
    }

    return users;
}

std::vector<Space*> DataLoader::loadSpaces(const std::string& filename) {
    std::vector<Space*> spaces;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open spaces file: " << filename << "\n";
        return spaces;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;

        int id;
        std::string type;
        std::string name;
        int capacity;
        bool hasProjector;
        bool hasWhiteboard;
        bool hasComputers;
        bool isAvailable;
        std::string building;

        std::getline(ss, token, ',');
        id = std::stoi(token);

        std::getline(ss, type, ',');
        std::getline(ss, name, ',');

        std::getline(ss, token, ',');
        capacity = std::stoi(token);

        std::getline(ss, token, ',');
        hasProjector = (token == "1");

        std::getline(ss, token, ',');
        hasWhiteboard = (token == "1");

        std::getline(ss, token, ',');
        hasComputers = (token == "1");

        std::getline(ss, token, ',');
        isAvailable = (token == "1");

        std::getline(ss, building, ',');

        if (type == "Classroom") {
            spaces.push_back(new Classroom(id, name, capacity,
                                           hasProjector, hasWhiteboard, hasComputers,
                                           isAvailable, building));
        } else if (type == "Laboratory") {
            spaces.push_back(new Laboratory(id, name, capacity,
                                            hasProjector, hasWhiteboard, hasComputers,
                                            isAvailable, building));
        } else if (type == "MeetingRoom") {
            spaces.push_back(new MeetingRoom(id, name, capacity,
                                             hasProjector, hasWhiteboard, hasComputers,
                                             isAvailable, building));
        }
    }

    return spaces;
}