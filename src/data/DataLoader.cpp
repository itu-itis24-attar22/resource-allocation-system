#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include "../models/Classroom.h"
#include "../models/Laboratory.h"
#include "../models/MeetingRoom.h"
#include "../models/UserFactory.h"

namespace {
    using HeaderIndex = std::unordered_map<std::string, size_t>;

    std::string removeUtf8Bom(const std::string& text) {
        if (text.size() >= 3 &&
            static_cast<unsigned char>(text[0]) == 0xEF &&
            static_cast<unsigned char>(text[1]) == 0xBB &&
            static_cast<unsigned char>(text[2]) == 0xBF) {
            return text.substr(3);
        }

        return text;
    }

    std::string trim(const std::string& text) {
        const std::string whitespace = " \t\r\n";
        const size_t first = text.find_first_not_of(whitespace);
        if (first == std::string::npos) {
            return "";
        }

        const size_t last = text.find_last_not_of(whitespace);
        return removeUtf8Bom(text.substr(first, last - first + 1));
    }

    std::vector<std::string> splitCsvLine(const std::string& line) {
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(trim(token));
        }

        return tokens;
    }

    std::string optionalColumn(const std::vector<std::string>& columns,
                               size_t index,
                               const std::string& defaultValue = "") {
        if (index >= columns.size() || columns[index].empty()) {
            return defaultValue;
        }

        return columns[index];
    }

    HeaderIndex buildHeaderIndex(const std::vector<std::string>& headers) {
        HeaderIndex index;

        for (size_t i = 0; i < headers.size(); ++i) {
            index[headers[i]] = i;
        }

        return index;
    }

    std::string optionalColumnByName(const std::vector<std::string>& columns,
                                     const HeaderIndex& headerIndex,
                                     const std::string& columnName,
                                     size_t fallbackIndex,
                                     const std::string& defaultValue = "") {
        const auto it = headerIndex.find(columnName);
        if (it != headerIndex.end()) {
            return optionalColumn(columns, it->second, defaultValue);
        }

        return optionalColumn(columns, fallbackIndex, defaultValue);
    }

    std::vector<std::string> splitRoleNames(const std::string& text) {
        std::vector<std::string> roles;
        std::stringstream ss(text);
        std::string role;

        while (std::getline(ss, role, '|')) {
            role = trim(role);
            if (!role.empty()) {
                roles.push_back(role);
            }
        }

        return roles;
    }

    std::string firstCsvToken(const std::string& line) {
        std::stringstream ss(line);
        std::string token;
        std::getline(ss, token, ',');
        return trim(token);
    }

    bool isUsersHeader(const std::string& line) {
        return firstCsvToken(line) == "userId";
    }

    bool isSpacesHeader(const std::string& line) {
        return firstCsvToken(line) == "spaceId";
    }

    bool isUserBusySlotsHeader(const std::string& line) {
        return firstCsvToken(line) == "userId";
    }

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

    bool tryParseTimeOfDay(const std::string& text, int& minutesFromMidnight) {
        const std::string trimmed = trim(text);
        const size_t colonPosition = trimmed.find(':');

        if (colonPosition == std::string::npos) {
            int hour = 0;
            if (!tryParseInt(trimmed, hour)) {
                return false;
            }

            if (hour < 0 || hour > 24) {
                return false;
            }

            minutesFromMidnight = hour * 60;
            return true;
        }

        const std::string hourText = trimmed.substr(0, colonPosition);
        const std::string minuteText = trimmed.substr(colonPosition + 1);
        int hour = 0;
        int minute = 0;

        if (!tryParseInt(hourText, hour) || !tryParseInt(minuteText, minute)) {
            return false;
        }

        if (hour < 0 || hour > 24 || minute < 0 || minute > 59) {
            return false;
        }

        if (hour == 24 && minute != 0) {
            return false;
        }

        minutesFromMidnight = hour * 60 + minute;
        return true;
    }

}

std::vector<User*> DataLoader::loadUsers(const std::string& filename) {
    std::vector<User*> users;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open users file: " << filename << "\n";
        return users;
    }

    std::string line;
    int lineNumber = 0;
    HeaderIndex userHeaderIndex;
    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue;
        if (lineNumber == 1 && isUsersHeader(line)) {
            userHeaderIndex = buildHeaderIndex(splitCsvLine(line));
            continue;
        }

        const std::vector<std::string> columns = splitCsvLine(line);

        if (columns.size() < 3) {
            std::cerr << "Warning: Skipping malformed user row " << lineNumber << ".\n";
            continue;
        }

        std::string idToken = columns[0];
        std::string name = columns[1];
        std::string roleString = columns[2];
        int id = 0;

        if (!tryParseInt(idToken, id)) {
            std::cerr << "Warning: Skipping malformed user row " << lineNumber << ".\n";
            continue;
        }

        UserProfileData profile;
        profile.email = optionalColumnByName(columns, userHeaderIndex, "email", 3);
        profile.status = optionalColumnByName(columns, userHeaderIndex, "status", 4, "active");
        profile.primaryUnitName = optionalColumnByName(columns, userHeaderIndex, "primaryUnit", 13);
        profile.assignedRoleNames = splitRoleNames(
            optionalColumnByName(columns, userHeaderIndex, "assignedRoles", 14)
        );
        profile.studentNo = optionalColumnByName(columns, userHeaderIndex, "studentNo", 5);
        profile.program = optionalColumnByName(columns, userHeaderIndex, "program", 6);
        tryParseInt(
            optionalColumnByName(columns, userHeaderIndex, "yearLevel", 7, "0"),
            profile.yearLevel
        );
        profile.title = optionalColumnByName(columns, userHeaderIndex, "title", 8);
        profile.officeRoom = optionalColumnByName(columns, userHeaderIndex, "officeRoom", 9);
        profile.assistantType = optionalColumnByName(columns, userHeaderIndex, "assistantType", 10);
        profile.jobTitle = optionalColumnByName(columns, userHeaderIndex, "jobTitle", 11);
        profile.adminLevel = optionalColumnByName(columns, userHeaderIndex, "adminLevel", 12);

        User* user = UserFactory::createUser(id, name, roleString, profile);
        if (!user) {
            std::cerr << "Warning: Skipping malformed user row " << lineNumber << ".\n";
            continue;
        }

        users.push_back(user);
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
        if (lineNumber == 1 && isSpacesHeader(line)) continue;

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

std::vector<UserBusySlot> DataLoader::loadUserBusySlots(const std::string& filename) {
    std::vector<UserBusySlot> busySlots;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Warning: Could not open user busy slots file: " << filename
                  << ". Continuing with no busy slots.\n";
        return busySlots;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        if (line.empty()) continue;
        if (lineNumber == 1 && isUserBusySlotsHeader(line)) continue;

        const std::vector<std::string> columns = splitCsvLine(line);

        if (columns.size() < 5) {
            std::cerr << "Warning: Skipping malformed user busy slot row "
                      << lineNumber << ".\n";
            continue;
        }

        int userId = 0;
        int day = 0;
        int startMinutes = 0;
        int endMinutes = 0;

        if (!tryParseInt(columns[0], userId) ||
            !tryParseInt(columns[1], day) ||
            !tryParseTimeOfDay(columns[2], startMinutes) ||
            !tryParseTimeOfDay(columns[3], endMinutes)) {
            std::cerr << "Warning: Skipping malformed user busy slot row "
                      << lineNumber << ".\n";
            continue;
        }

        try {
            TimeSlot slot = TimeSlot::fromMinutes(day, startMinutes, endMinutes);
            busySlots.emplace_back(userId, slot, columns[4]);
        } catch (...) {
            std::cerr << "Warning: Skipping invalid user busy slot row "
                      << lineNumber << ".\n";
        }
    }

    return busySlots;
}
