#include "DataController.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <cctype>
#include "DataLoader.h"
#include "AllocationWriter.h"
#include "../models/RequestFactory.h"
#include "../models/CommitteeMeetingRequest.h"
#include "RequestResultWriter.h"

namespace {
    std::string trim(const std::string& value) {
        const std::string whitespace = " \t\r\n";
        const size_t start = value.find_first_not_of(whitespace);
        if (start == std::string::npos) {
            return "";
        }

        const size_t end = value.find_last_not_of(whitespace);
        return value.substr(start, end - start + 1);
    }

    std::string firstCsvToken(const std::string& line) {
        std::stringstream ss(line);
        std::string token;
        std::getline(ss, token, ',');
        return trim(token);
    }

    bool isRequestsHeader(const std::string& line) {
        return firstCsvToken(line) == "requestId";
    }

    std::string normalizeStrategyName(const std::string& strategyName) {
        std::string normalized;

        for (char ch : strategyName) {
            normalized += static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }

        return normalized;
    }

    std::string normalizeOptionalField(const std::string& value) {
        if (value == "None") return "";
        return value;
    }

    std::string normalizeMetadataField(const std::string& value) {
        std::string normalized = trim(value);
        if (normalized == "None") return "";
        return normalized;
    }

    bool parseOptionalBoolField(const std::string& value) {
        std::string normalized = normalizeMetadataField(value);

        for (char& ch : normalized) {
            ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        }

        return normalized == "true" || normalized == "1" || normalized == "yes";
    }

    struct OptionalRequestFields {
        std::string title;
        std::string purpose;
        std::string courseCode;
        std::string courseName;
        std::string examType;
        bool canSplitAcrossRooms = false;
    };

    void parseOptionalRequestFields(std::stringstream& ss, OptionalRequestFields& fields) {
        std::string token;

        if (std::getline(ss, token, ',')) {
            fields.title = normalizeMetadataField(token);
        }

        if (std::getline(ss, token, ',')) {
            fields.purpose = normalizeMetadataField(token);
        }

        if (std::getline(ss, token, ',')) {
            fields.courseCode = normalizeMetadataField(token);
        }

        if (std::getline(ss, token, ',')) {
            fields.courseName = normalizeMetadataField(token);
        }

        if (std::getline(ss, token, ',')) {
            fields.examType = normalizeMetadataField(token);
        }

        if (std::getline(ss, token)) {
            fields.canSplitAcrossRooms = parseOptionalBoolField(token);
        }
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

    void printRejectedRequestWarning(int requestId, const std::string& reason) {
        if (requestId >= 0) {
            std::cerr << "Warning: Request " << requestId << " rejected: " << reason << ".\n";
        } else {
            std::cerr << "Warning: Rejected request: " << reason << ".\n";
        }
    }

    User* findUserById(const std::vector<User*>& users, int userId) {
        for (User* user : users) {
            if (user->getId() == userId) {
                return user;
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

    std::vector<Request*> loadRequestsFromCsv(const std::string& filename,
                                              const std::vector<User*>& users,
                                              const std::vector<Space*>& spaces) {
        std::vector<Request*> requests;
        std::set<int> seenRequestIds;
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error: Could not open requests file: " << filename << "\n";
            return requests;
        }

        std::string line;
        int lineNumber = 0;
        while (std::getline(file, line)) {
            lineNumber++;
            if (line.empty()) continue;
            if (lineNumber == 1 && isRequestsHeader(line)) continue;

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
            OptionalRequestFields optionalFields;

            if (!std::getline(ss, token, ',') || !tryParseInt(token, requestId)) {
                requests.push_back(RequestFactory::createInvalidRequest(
                    -1, "Unknown", nullptr, nullptr, 0, "", "", "", "", "", "Malformed input"
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
                std::getline(ss, timeData, ',');
                timeData = trim(timeData);

                parseOptionalRequestFields(ss, optionalFields);

                requests.push_back(RequestFactory::createInvalidRequest(
                    requestId,
                    requestType.empty() ? "Unknown" : requestType,
                    findUserById(users, userId),
                    findSpaceById(spaces, spaceId),
                    participantCount,
                    optionalFields.title,
                    optionalFields.purpose,
                    requiredFeature,
                    requiredBuilding,
                    timeData,
                    "Duplicate request ID",
                    optionalFields.courseCode,
                    optionalFields.courseName,
                    optionalFields.examType,
                    optionalFields.canSplitAcrossRooms
                ));
                printRejectedRequestWarning(requestId, "Duplicate request ID");
                continue;
            }

            seenRequestIds.insert(requestId);

            if (!std::getline(ss, requestType, ',')) {
                requests.push_back(RequestFactory::createInvalidRequest(
                    requestId, "Unknown", nullptr, nullptr, 0, "", "", "", "", "", "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }

            if (!std::getline(ss, token, ',') || !tryParseInt(token, userId)) {
                requests.push_back(RequestFactory::createInvalidRequest(
                    requestId, requestType, nullptr, nullptr, 0, "", "", "", "", "", "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }

            if (!std::getline(ss, token, ',') || !tryParseInt(token, spaceId)) {
                requests.push_back(RequestFactory::createInvalidRequest(
                    requestId, requestType, findUserById(users, userId), nullptr, 0, "", "", "", "", "", "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }

            if (!std::getline(ss, token, ',') || !tryParseInt(token, participantCount)) {
                std::getline(ss, requiredFeature, ',');
                requiredFeature = normalizeOptionalField(requiredFeature);
                std::getline(ss, requiredBuilding, ',');
                requiredBuilding = normalizeOptionalField(requiredBuilding);
                std::getline(ss, timeData, ',');
                timeData = trim(timeData);

                parseOptionalRequestFields(ss, optionalFields);

                requests.push_back(RequestFactory::createInvalidRequest(
                    requestId, requestType, findUserById(users, userId), findSpaceById(spaces, spaceId),
                    0, optionalFields.title, optionalFields.purpose, requiredFeature, requiredBuilding, timeData,
                    "Malformed input", optionalFields.courseCode, optionalFields.courseName,
                    optionalFields.examType, optionalFields.canSplitAcrossRooms
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }

            if (!std::getline(ss, requiredFeature, ',')) {
                requests.push_back(RequestFactory::createInvalidRequest(
                    requestId, requestType, findUserById(users, userId), findSpaceById(spaces, spaceId),
                    participantCount, "", "", "", "", "", "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }
            requiredFeature = normalizeOptionalField(requiredFeature);

            if (!std::getline(ss, requiredBuilding, ',')) {
                requests.push_back(RequestFactory::createInvalidRequest(
                    requestId, requestType, findUserById(users, userId), findSpaceById(spaces, spaceId),
                    participantCount, "", "", requiredFeature, "", "", "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }
            requiredBuilding = normalizeOptionalField(requiredBuilding);

            if (!std::getline(ss, timeData, ',')) {
                requests.push_back(RequestFactory::createInvalidRequest(
                    requestId, requestType, findUserById(users, userId), findSpaceById(spaces, spaceId),
                    participantCount, "", "", requiredFeature, requiredBuilding, "", "Malformed input"
                ));
                printRejectedRequestWarning(requestId, "Malformed input");
                continue;
            }
            timeData = trim(timeData);

            parseOptionalRequestFields(ss, optionalFields);

            User* user = findUserById(users, userId);
            Space* space = findSpaceById(spaces, spaceId);

            if (!user) {
                requests.push_back(RequestFactory::createInvalidRequest(
                    requestId, requestType, nullptr, space, participantCount,
                    optionalFields.title,
                    optionalFields.purpose,
                    requiredFeature, requiredBuilding, timeData,
                    "Invalid user reference", optionalFields.courseCode, optionalFields.courseName,
                    optionalFields.examType, optionalFields.canSplitAcrossRooms
                ));
                printRejectedRequestWarning(requestId, "Invalid user reference");
                continue;
            }

            if (!space) {
                requests.push_back(RequestFactory::createInvalidRequest(
                    requestId, requestType, user, nullptr, participantCount,
                    optionalFields.title,
                    optionalFields.purpose,
                    requiredFeature, requiredBuilding, timeData,
                    "Invalid space reference", optionalFields.courseCode, optionalFields.courseName,
                    optionalFields.examType, optionalFields.canSplitAcrossRooms
                ));
                printRejectedRequestWarning(requestId, "Invalid space reference");
                continue;
            }

            Request* request = RequestFactory::createRequest(
                requestId,
                requestType,
                user,
                space,
                participantCount,
                optionalFields.title,
                optionalFields.purpose,
                requiredFeature,
                requiredBuilding,
                timeData,
                optionalFields.courseCode,
                optionalFields.courseName,
                optionalFields.examType,
                optionalFields.canSplitAcrossRooms
            );
            requests.push_back(request);

            if (request->getStatus() == RequestStatus::Rejected) {
                printRejectedRequestWarning(requestId, request->getRejectionReason());
            }
        }

        return requests;
    }

    void attachParticipantsToCommitteeRequests(
        std::vector<Request*>& requests,
        const std::vector<RequestParticipant>& requestParticipants
    ) {
        for (Request* request : requests) {
            CommitteeMeetingRequest* committeeRequest =
                dynamic_cast<CommitteeMeetingRequest*>(request);

            if (!committeeRequest) {
                continue;
            }

            int attachedCount = 0;
            for (const RequestParticipant& participant : requestParticipants) {
                if (participant.getRequestId() == committeeRequest->getId()) {
                    committeeRequest->addRequiredParticipant(
                        participant.getUserId(),
                        participant.getParticipantRole()
                    );
                    attachedCount++;
                }
            }

            if (attachedCount == 0) {
                committeeRequest->addHistoryEvent("No request participants loaded for committee meeting");
            } else {
                committeeRequest->addHistoryEvent(
                    "Loaded " + std::to_string(attachedCount)
                    + " required committee participant(s)"
                );
            }
        }
    }
}

std::string DataController::loadAllocationStrategyName(const std::string& configFile) const {
    std::ifstream file(configFile);

    if (!file.is_open()) {
        std::cerr << "Warning: allocation strategy config missing. Falling back to greedy.\n";
        return "greedy";
    }

    std::string line;
    while (std::getline(file, line)) {
        std::string trimmedLine = trim(line);

        if (trimmedLine.empty() || trimmedLine[0] == '#') {
            continue;
        }

        const size_t equalsPos = trimmedLine.find('=');
        if (equalsPos == std::string::npos) {
            continue;
        }

        const std::string key = trim(trimmedLine.substr(0, equalsPos));
        const std::string value = trim(trimmedLine.substr(equalsPos + 1));

        if (key == "allocation_strategy") {
            if (!value.empty()) {
                const std::string normalizedValue = normalizeStrategyName(value);

                if (normalizedValue == "greedy" ||
                    normalizedValue == "priority" ||
                    normalizedValue == "multi_room_exam_greedy" ||
                    normalizedValue == "multi_room_exam_best_fit" ||
                    normalizedValue == "shared_room_exam_best_fit") {
                    return normalizedValue;
                }

                std::cerr << "Warning: unknown allocation strategy. Falling back to greedy.\n";
                return "greedy";
            }

            std::cerr << "Warning: allocation strategy config missing. Falling back to greedy.\n";
            return "greedy";
        }
    }

    std::cerr << "Warning: allocation strategy config missing. Falling back to greedy.\n";
    return "greedy";
}

SystemData DataController::loadAllData(const std::string& usersFile,
                                       const std::string& spacesFile,
                                       const std::string& requestsFile) const {
    SystemData data;
    data.users = DataLoader::loadUsers(usersFile);
    data.spaces = DataLoader::loadSpaces(spacesFile);
    data.requests = loadRequestsFromCsv(requestsFile, data.users, data.spaces);
    data.userBusySlots = DataLoader::loadUserBusySlots("data/user_busy_slots.csv");
    data.requestParticipants = DataLoader::loadRequestParticipants("data/request_participants.csv");
    attachParticipantsToCommitteeRequests(data.requests, data.requestParticipants);
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

    for (User* user : data.users) {
        delete user;
    }
    data.users.clear();
}

void DataController::exportRequestResults(const std::string& resultsFile,
                                          const std::vector<Request*>& requests) const {
    RequestResultWriter::writeRequestResults(resultsFile, requests);
}
