#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "../models/Classroom.h"
#include "../models/Laboratory.h"
#include "../models/MeetingRoom.h"
#include "../models/UserRole.h"
#include "RequestResultWriter.h"

namespace {
    bool tryParseInt(const std::string& text, int& value) {
        try {
            size_t parsedLength = 0;
            value = std::stoi(text, &parsedLength);
            return parsedLength == text.size();
        } catch (...) {
            return false;
        }
    }

    bool tryParseFlag(const std::string& text, bool& value) {
        if (text == "0") {
            value = false;
            return true;
        }
        if (text == "1") {
            value = true;
            return true;
        }
        return false;
    }

    bool tryParseUserRole(const std::string& text, UserRole& role) {
        if (text == "Student") {
            role = UserRole::Student;
            return true;
        }
        if (text == "Instructor") {
            role = UserRole::Instructor;
            return true;
        }
        if (text == "Staff") {
            role = UserRole::Staff;
            return true;
        }
        if (text == "Administrator") {
            role = UserRole::Administrator;
            return true;
        }
        return false;
    }
}

std::vector<User> DataLoader::loadUsers(const std::string& filename) {
    std::vector<User> users;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open users file: " << filename << "\n";
        return users;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idToken;
        std::string name;
        std::string roleString;
        int id = 0;
        UserRole role;

        if (!std::getline(ss, idToken, ',') ||
            !std::getline(ss, name, ',') ||
            !std::getline(ss, roleString, ',')) {
            std::cerr << "Warning: Skipping malformed user row " << lineNumber << ".\n";
            continue;
        }

        if (!tryParseInt(idToken, id) || !tryParseUserRole(roleString, role)) {
            std::cerr << "Warning: Skipping malformed user row " << lineNumber << ".\n";
            continue;
        }

        users.emplace_back(id, name, role);
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
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idToken;
        std::string type;
        std::string name;
        std::string capacityToken;
        std::string projectorToken;
        std::string whiteboardToken;
        std::string computersToken;
        std::string availableToken;
        std::string building;
        int id = 0;
        int capacity = 0;
        bool hasProjector = false;
        bool hasWhiteboard = false;
        bool hasComputers = false;
        bool isAvailable = false;

        if (!std::getline(ss, idToken, ',') ||
            !std::getline(ss, type, ',') ||
            !std::getline(ss, name, ',') ||
            !std::getline(ss, capacityToken, ',') ||
            !std::getline(ss, projectorToken, ',') ||
            !std::getline(ss, whiteboardToken, ',') ||
            !std::getline(ss, computersToken, ',') ||
            !std::getline(ss, availableToken, ',') ||
            !std::getline(ss, building, ',')) {
            std::cerr << "Warning: Skipping malformed space row " << lineNumber << ".\n";
            continue;
        }

        if (!tryParseInt(idToken, id) ||
            !tryParseInt(capacityToken, capacity) ||
            !tryParseFlag(projectorToken, hasProjector) ||
            !tryParseFlag(whiteboardToken, hasWhiteboard) ||
            !tryParseFlag(computersToken, hasComputers) ||
            !tryParseFlag(availableToken, isAvailable)) {
            std::cerr << "Warning: Skipping malformed space row " << lineNumber << ".\n";
            continue;
        }

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
        } else {
            std::cerr << "Warning: Skipping malformed space row " << lineNumber << ".\n";
        }
    }

    return spaces;
}
