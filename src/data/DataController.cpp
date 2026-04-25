#include "DataController.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include "DataLoader.h"
#include "AllocationWriter.h"
#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"
#include "../models/InvalidRequest.h"
#include "DataController.h"
#include "RequestResultWriter.h"

namespace {
    class PlaceholderSpace : public Space {
    public:
        PlaceholderSpace()
            : Space(-1, "UnknownSpace", 0, false, false, false, false, "Unknown") {}

        std::string getType() const override {
            return "Unknown";
        }
    };

    User& placeholderUser() {
        static User user(-1, "UnknownUser", UserRole::Student);
        return user;
    }

    Space* placeholderSpace() {
        static PlaceholderSpace space;
        return &space;
    }

    std::string normalizeOptionalField(const std::string& value) {
        if (value == "None") return "";
        return value;
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

    InvalidRequest* createRejectedRequest(int requestId,
                                          const std::string& requestType,
                                          const User* user,
                                          Space* space,
                                          int participantCount,
                                          const std::string& requiredFeature,
                                          const std::string& requiredBuilding,
                                          const std::string& timeData,
                                          const std::string& reason) {
        InvalidRequest* request = new InvalidRequest(
            requestId,
            user ? *user : placeholderUser(),
            space ? space : placeholderSpace(),
            participantCount,
            requestType.empty() ? "Unknown" : requestType,
            timeData,
            requiredFeature,
            requiredBuilding
        );
        request->addHistoryEvent("evaluated");
        request->markRejected(reason);
        return request;
    }

    void printRejectedRequestWarning(int requestId, const std::string& reason) {
        if (requestId >= 0) {
            std::cerr << "Warning: Request " << requestId << " rejected: " << reason << ".\n";
        } else {
            std::cerr << "Warning: Rejected request: " << reason << ".\n";
        }
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

    bool tryParseSingleTimeSlot(const std::string& text,
                                TimeSlot& slot,
                                std::string& errorReason) {
        std::stringstream ss(text);
        std::string token;

        int day, startHour, endHour;

        if (!std::getline(ss, token, '-') || !tryParseInt(token, day)) {
            errorReason = "Malformed input";
            return false;
        }

        if (!std::getline(ss, token, '-') || !tryParseInt(token, startHour)) {
            errorReason = "Malformed input";
            return false;
        }

        if (!std::getline(ss, token, '-') || !tryParseInt(token, endHour)) {
            errorReason = "Malformed input";
            return false;
        }

        try {
            slot = TimeSlot(day, startHour, endHour);
            return true;
        } catch (...) {
            errorReason = "Invalid time slot";
            return false;
        }
    }

    bool tryParseRecurringTimeSlots(const std::string& text,
                                    std::vector<TimeSlot>& slots,
                                    std::string& errorReason) {
        std::stringstream ss(text);
        std::string part;

        while (std::getline(ss, part, ';')) {
            if (!part.empty()) {
                TimeSlot slot(1, 0, 1);
                if (!tryParseSingleTimeSlot(part, slot, errorReason)) {
                    return false;
                }
                slots.push_back(slot);
            }
        }

        if (slots.empty()) {
            errorReason = "Malformed input";
            return false;
        }

        return true;
    }

    std::vector<Request*> loadRequestsFromCsv(const std::string& filename,
                                              const std::vector<User>& users,
                                              const std::vector<Space*>& spaces) {
        std::vector<Request*> requests;
        std::set<int> seenRequestIds;
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

            int requestId = -1;
            std::string requestType;
            int userId = -1;
            int spaceId = -1;
            int participantCount = 0;
            std::string requiredFeature;
            std::string requiredBuilding;
            std::string timeData;

            if (!std::getline(ss, token, ',') || !tryParseInt(token, requestId)) {
                requests.push_back(createRejectedRequest(
                    -1, "Unknown", nullptr, nullptr, 0, "", "", "", "Malformed input"
                ));
                printRejectedRequestWarning(-1, "Malformed input");
                continue;
            }

            if (seenRequestIds.find(requestId) != seenRequestIds.end()) {
                std::getline(ss, requestType, ',');
                std::getline(ss, token, ',');
                userId = tryParseInt(token, userId) ? userId : -1;
                std::getline(ss, token, ',');
                spaceId = tryParseInt(token, spaceId) ? spaceId : -1;
                std::getline(ss, token, ',');
                participantCount = tryParseInt(token, participantCount) ? participantCount : 0;
                std::getline(ss, requiredFeature, ',');
                requiredFeature = normalizeOptionalField(requiredFeature);
                std::getline(ss, requiredBuilding, ',');
                requiredBuilding = normalizeOptionalField(requiredBuilding);
                std::getline(ss, timeData);

                requests.push_back(createRejectedRequest(
                    requestId,
                    requestType.empty() ? "Unknown" : requestType,
                    findUserById(users, userId),
                    findSpaceById(spaces, spaceId),
                    participantCount,
                    requiredFeature,
                    requiredBuilding,
                    timeData,
                    "Duplicate request ID"
                ));
                printRejectedRequestWarning(requestId, "Duplicate request ID");
                continue;
            }

            seenRequestIds.insert(requestId);

            if (!std::getline(ss, requestType, ',')) {
                requests.push_back(createRejectedRequest(
                    requestId, "Unknown", nullptr, nullptr, 0, "", "", "", "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }

            if (!std::getline(ss, token, ',') || !tryParseInt(token, userId)) {
                requests.push_back(createRejectedRequest(
                    requestId, requestType, nullptr, nullptr, 0, "", "", "", "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }

            if (!std::getline(ss, token, ',') || !tryParseInt(token, spaceId)) {
                requests.push_back(createRejectedRequest(
                    requestId, requestType, findUserById(users, userId), nullptr, 0, "", "", "", "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }

            if (!std::getline(ss, token, ',') || !tryParseInt(token, participantCount)) {
                requests.push_back(createRejectedRequest(
                    requestId, requestType, findUserById(users, userId), findSpaceById(spaces, spaceId),
                    0, "", "", "", "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }

            if (!std::getline(ss, requiredFeature, ',')) {
                requests.push_back(createRejectedRequest(
                    requestId, requestType, findUserById(users, userId), findSpaceById(spaces, spaceId),
                    participantCount, "", "", "", "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }
            requiredFeature = normalizeOptionalField(requiredFeature);

            if (!std::getline(ss, requiredBuilding, ',')) {
                requests.push_back(createRejectedRequest(
                    requestId, requestType, findUserById(users, userId), findSpaceById(spaces, spaceId),
                    participantCount, requiredFeature, "", "", "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }
            requiredBuilding = normalizeOptionalField(requiredBuilding);

            if (!std::getline(ss, timeData)) {
                requests.push_back(createRejectedRequest(
                    requestId, requestType, findUserById(users, userId), findSpaceById(spaces, spaceId),
                    participantCount, requiredFeature, requiredBuilding, "", "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }

            const User* user = findUserById(users, userId);
            Space* space = findSpaceById(spaces, spaceId);

            if (!user) {
                requests.push_back(createRejectedRequest(
                    requestId, requestType, nullptr, space, participantCount,
                    requiredFeature, requiredBuilding, timeData, "Invalid user reference"
                ));
                printRejectedRequestWarning(requestId, "Invalid user reference");
                continue;
            }

            if (!space) {
                requests.push_back(createRejectedRequest(
                    requestId, requestType, user, nullptr, participantCount,
                    requiredFeature, requiredBuilding, timeData, "Invalid space reference"
                ));
                printRejectedRequestWarning(requestId, "Invalid space reference");
                continue;
            }

            if (requestType == "OneTime") {
                TimeSlot slot(1, 0, 1);
                std::string errorReason;
                if (!tryParseSingleTimeSlot(timeData, slot, errorReason)) {
                    requests.push_back(createRejectedRequest(
                        requestId, requestType, user, space, participantCount,
                        requiredFeature, requiredBuilding, timeData, errorReason
                    ));
                    printRejectedRequestWarning(requestId, errorReason);
                    continue;
                }
                requests.push_back(new OneTimeRequest(
                    requestId, *user, space, slot,
                    participantCount, requiredFeature, requiredBuilding
                ));
            } else if (requestType == "Recurring") {
                std::vector<TimeSlot> slots;
                std::string errorReason;
                if (!tryParseRecurringTimeSlots(timeData, slots, errorReason)) {
                    requests.push_back(createRejectedRequest(
                        requestId, requestType, user, space, participantCount,
                        requiredFeature, requiredBuilding, timeData, errorReason
                    ));
                    printRejectedRequestWarning(requestId, errorReason);
                    continue;
                }
                requests.push_back(new RecurringRequest(
                    requestId, *user, space, slots,
                    participantCount, requiredFeature, requiredBuilding
                ));
            } else {
                requests.push_back(createRejectedRequest(
                    requestId, requestType, user, space, participantCount,
                    requiredFeature, requiredBuilding, timeData, "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
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
