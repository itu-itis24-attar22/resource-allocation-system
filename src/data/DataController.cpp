#include "DataController.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "DataLoader.h"
#include "AllocationWriter.h"
#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"
#include "DataController.h"
#include "RequestResultWriter.h"

namespace {
    std::string normalizeOptionalField(const std::string& value) {
        if (value == "None") return "";
        return value;
    }

    const User* findUserById(const std::vector<User>& users, int userId) {
        for (const User& user : users) {
            if (user.getId() == userId) {
                return &user;
            }
        }
        return nullptr;
    }

    Space* findSpaceById(const std::vector<Space*>& spaces, int spaceId) {
        for (Space* space : spaces) {
            if (space->getId() == spaceId) {
                return space;
            }
        }
        return nullptr;
    }

    TimeSlot parseSingleTimeSlot(const std::string& text) {
        std::stringstream ss(text);
        std::string token;

        int day, startHour, endHour;

        std::getline(ss, token, '-');
        day = std::stoi(token);

        std::getline(ss, token, '-');
        startHour = std::stoi(token);

        std::getline(ss, token, '-');
        endHour = std::stoi(token);

        return TimeSlot(day, startHour, endHour);
    }

    std::vector<TimeSlot> parseRecurringTimeSlots(const std::string& text) {
        std::vector<TimeSlot> slots;
        std::stringstream ss(text);
        std::string part;

        while (std::getline(ss, part, ';')) {
            if (!part.empty()) {
                slots.push_back(parseSingleTimeSlot(part));
            }
        }

        return slots;
    }

    std::vector<Request*> loadRequestsFromCsv(const std::string& filename,
                                              const std::vector<User>& users,
                                              const std::vector<Space*>& spaces) {
        std::vector<Request*> requests;
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error: Could not open requests file: " << filename << "\n";
            return requests;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string token;

            int requestId;
            std::string requestType;
            int userId;
            int spaceId;
            int participantCount;
            std::string requiredFeature;
            std::string requiredBuilding;
            std::string timeData;

            std::getline(ss, token, ',');
            requestId = std::stoi(token);

            std::getline(ss, requestType, ',');

            std::getline(ss, token, ',');
            userId = std::stoi(token);

            std::getline(ss, token, ',');
            spaceId = std::stoi(token);

            std::getline(ss, token, ',');
            participantCount = std::stoi(token);

            std::getline(ss, requiredFeature, ',');
            requiredFeature = normalizeOptionalField(requiredFeature);

            std::getline(ss, requiredBuilding, ',');
            requiredBuilding = normalizeOptionalField(requiredBuilding);

            std::getline(ss, timeData);

            const User* user = findUserById(users, userId);
            Space* space = findSpaceById(spaces, spaceId);

            if (!user || !space) {
                std::cerr << "Warning: Skipping request " << requestId
                          << "missing user or space reference.\n";
                continue;
            }

            if (requestType == "OneTime") {
                TimeSlot slot = parseSingleTimeSlot(timeData);
                requests.push_back(new OneTimeRequest(
                    requestId, *user, space, slot,
                    participantCount, requiredFeature, requiredBuilding
                ));
            } else if (requestType == "Recurring") {
                std::vector<TimeSlot> slots = parseRecurringTimeSlots(timeData);
                requests.push_back(new RecurringRequest(
                    requestId, *user, space, slots,
                    participantCount, requiredFeature, requiredBuilding
                ));
            } else {
                std::cerr << "Warning: Unknown request type for request " << requestId << "\n";
            }
        }

        return requests;
    }
}

SystemData DataController::loadAllData(const std::string& usersFile,
                                       const std::string& spacesFile,
                                       const std::string& requestsFile) const {
    SystemData data;
    data.users = DataLoader::loadUsers(usersFile);
    data.spaces = DataLoader::loadSpaces(spacesFile);
    data.requests = loadRequestsFromCsv(requestsFile, data.users, data.spaces);
    return data;
}

void DataController::exportAllocations(const std::string& allocationsFile,
                                       const std::vector<Allocation>& allocations) const {
    AllocationWriter::writeAllocations(allocationsFile, allocations);
}

void DataController::cleanupData(SystemData& data) const {
    for (Request* request : data.requests) {
        delete request;
    }
    data.requests.clear();

    for (Space* space : data.spaces) {
        delete space;
    }
    data.spaces.clear();
}

void DataController::exportRequestResults(const std::string& resultsFile,
                                          const std::vector<Request*>& requests) const {
    RequestResultWriter::writeRequestResults(resultsFile, requests);
}