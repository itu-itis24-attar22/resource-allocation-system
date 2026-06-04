#include "test_framework.h"
#include "test_helpers.h"

#include "../src/data/SummaryWriter.h"
#include "../src/models/Classroom.h"
#include "../src/models/CommitteeMeetingRequest.h"
#include "../src/models/ExamRequest.h"
#include "../src/models/Instructor.h"
#include "../src/models/MeetingRoom.h"
#include "../src/models/OneTimeRequest.h"
#include "../src/models/Student.h"
#include "../src/models/UserBusySlot.h"
#include "../src/services/AllocationService.h"
#include "../src/services/MeetingTimeSuggestionService.h"
#include "../src/services/ResourceAllocationSession.h"

#include <chrono>
#include <cstdio>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace {
    bool sameSlot(const TimeSlot& left, const TimeSlot& right) {
        return left.getDay() == right.getDay() &&
               left.getStartMinutes() == right.getStartMinutes() &&
               left.getEndMinutes() == right.getEndMinutes();
    }

    bool hasAllocationForRequestSpaceSlot(const std::vector<Allocation>& allocations,
                                          int requestId,
                                          int spaceId,
                                          const TimeSlot& slot) {
        for (const Allocation& allocation : allocations) {
            if (allocation.getRequestId() == requestId &&
                allocation.getSpace() &&
                allocation.getSpace()->getId() == spaceId &&
                sameSlot(allocation.getTimeSlot(), slot)) {
                return true;
            }
        }

        return false;
    }

    int totalAssignedForRequest(const std::vector<Allocation>& allocations,
                                int requestId) {
        int total = 0;
        for (const Allocation& allocation : allocations) {
            if (allocation.getRequestId() == requestId) {
                total += allocation.getAssignedParticipants();
            }
        }

        return total;
    }

    int totalAssignedForSpaceAt(const std::vector<Allocation>& allocations,
                                int spaceId,
                                const TimeSlot& slot) {
        int total = 0;
        for (const Allocation& allocation : allocations) {
            if (allocation.getSpace() &&
                allocation.getSpace()->getId() == spaceId &&
                allocation.getTimeSlot().overlapsWith(slot)) {
                total += allocation.getAssignedParticipants();
            }
        }

        return total;
    }

    bool allOverlappingSpaceLoadsRespectCapacity(
        const std::vector<Allocation>& allocations
    ) {
        for (const Allocation& allocation : allocations) {
            const Space* space = allocation.getSpace();
            if (!space) {
                return false;
            }

            const int assignedAtSameTime =
                totalAssignedForSpaceAt(allocations,
                                        space->getId(),
                                        allocation.getTimeSlot());

            if (assignedAtSameTime > space->getCapacity()) {
                return false;
            }
        }

        return true;
    }

    std::map<int, std::vector<TimeSlot>> buildCommitteeParticipantSchedule(
        const std::vector<Request*>& requests,
        const std::vector<Allocation>& allocations
    ) {
        std::map<int, std::vector<TimeSlot>> scheduleByUser;

        for (const Request* request : requests) {
            const CommitteeMeetingRequest* committee =
                dynamic_cast<const CommitteeMeetingRequest*>(request);

            if (!committee || committee->getStatus() != RequestStatus::Approved) {
                continue;
            }

            for (const Allocation& allocation : allocations) {
                if (allocation.getRequestId() != committee->getId()) {
                    continue;
                }

                for (int participantId : committee->getRequiredParticipantIds()) {
                    scheduleByUser[participantId].push_back(allocation.getTimeSlot());
                }
            }
        }

        return scheduleByUser;
    }

    bool participantHasSlot(
        const std::map<int, std::vector<TimeSlot>>& scheduleByUser,
        int userId,
        const TimeSlot& slot
    ) {
        const auto found = scheduleByUser.find(userId);
        if (found == scheduleByUser.end()) {
            return false;
        }

        for (const TimeSlot& scheduledSlot : found->second) {
            if (sameSlot(scheduledSlot, slot)) {
                return true;
            }
        }

        return false;
    }

    void writeCsv(const std::string& path, const std::string& contents) {
        std::ofstream file(path);
        file << contents;
    }

    bool fileContains(const std::string& path, const std::string& text) {
        std::ifstream file(path);
        if (!file.is_open()) {
            return false;
        }

        const std::string contents((std::istreambuf_iterator<char>(file)),
                                   std::istreambuf_iterator<char>());
        return contents.find(text) != std::string::npos;
    }
}

TEST_CASE("Derived room schedule includes approved allocations and excludes rejected conflicts") {
    Instructor instructor(10, "Dr. Schedule");
    MeetingRoom room(301, "M301", 20, true, true, false, true, "AdminBuilding");
    AllocationService service("greedy");

    OneTimeRequest approved(501, &instructor, &room, TimeSlot(1, 9, 10),
                            12, "Approved Meeting", "Schedule consistency",
                            "Projector", "AdminBuilding");
    OneTimeRequest conflicting(502, &instructor, &room, TimeSlot(1, 9, 10),
                               8, "Rejected Conflict", "Should not appear",
                               "Projector", "AdminBuilding");

    CHECK(service.processRequest(approved));
    CHECK(!service.processRequest(conflicting));

    const std::vector<Allocation>& allocations = service.getAllocations();
    CHECK_EQ(allocations.size(), static_cast<size_t>(1));
    CHECK(approved.getStatus() == RequestStatus::Approved);
    CHECK(conflicting.getStatus() == RequestStatus::Rejected);
    CHECK_EQ(conflicting.getRejectionReason(), std::string("Time slot unavailable"));
    CHECK(hasAllocationForRequestSpaceSlot(allocations, 501, 301, TimeSlot(1, 9, 10)));
    CHECK_EQ(allocationCountForRequest(allocations, 502), 0);
}

TEST_CASE("Committee participant schedule is derived only from approved committee allocations") {
    Student student(1, "Student One");
    Instructor freeProfessor(2, "Dr. Free");
    Instructor busyProfessor(3, "Dr. Busy");
    MeetingRoom room(301, "M301", 12, true, true, false, true, "AdminBuilding");
    std::vector<User*> users{&student, &freeProfessor, &busyProfessor};
    std::vector<UserBusySlot> busySlots{
        UserBusySlot(3, TimeSlot::fromMinutes(1, 10 * 60, 12 * 60), "Lecture")
    };
    AllocationService service("greedy", users, busySlots);

    CommitteeMeetingRequest approved(601, &student, &room, TimeSlot(2, 13, 14),
                                     8, "Approved Thesis Meeting",
                                     "Committee meeting", "Projector",
                                     "AdminBuilding");
    approved.addRequiredParticipant(2, "Supervisor");
    approved.addRequiredParticipant(3, "CommitteeMember");

    CommitteeMeetingRequest rejected(602, &student, &room,
                                     TimeSlot::fromMinutes(1, 10 * 60, 11 * 60),
                                     8, "Busy Professor Meeting",
                                     "Committee meeting", "Projector",
                                     "AdminBuilding");
    rejected.addRequiredParticipant(3, "Supervisor");

    CHECK(service.processRequest(approved));
    CHECK(!service.processRequest(rejected));

    const std::vector<Request*> requests{&approved, &rejected};
    const auto participantSchedule =
        buildCommitteeParticipantSchedule(requests, service.getAllocations());

    CHECK(participantHasSlot(participantSchedule, 2, TimeSlot(2, 13, 14)));
    CHECK(participantHasSlot(participantSchedule, 3, TimeSlot(2, 13, 14)));
    CHECK(!participantHasSlot(participantSchedule, 3, TimeSlot(1, 10, 11)));
    CHECK_EQ(allocationCountForRequest(service.getAllocations(), 601), 1);
    CHECK_EQ(allocationCountForRequest(service.getAllocations(), 602), 0);
    CHECK_CONTAINS(rejected.getRejectionReason(), "Dr. Busy");
    CHECK(historyContains(rejected, "Suggested least-change alternative times"));
}

TEST_CASE("Least-change meeting suggestions are valid schedule alternatives") {
    Student student(1, "Student One");
    Instructor professor(2, "Dr. Busy");
    MeetingRoom room(301, "M301", 12, true, true, false, true, "AdminBuilding");
    std::vector<UserBusySlot> busySlots{
        UserBusySlot(2, TimeSlot::fromMinutes(1, 10 * 60, 12 * 60), "Lecture")
    };
    CommitteeMeetingRequest request(603, &student, &room,
                                    TimeSlot::fromMinutes(1, 10 * 60, 11 * 60),
                                    8, "Suggested Meeting",
                                    "Committee meeting", "Projector",
                                    "AdminBuilding");
    request.addRequiredParticipant(2, "Supervisor");
    MeetingTimeSuggestionService suggestionService;

    const std::vector<MeetingTimeSuggestion> suggestions =
        suggestionService.suggestTimes(request, &room, busySlots, {}, 3);

    CHECK_EQ(suggestions.size(), static_cast<size_t>(3));
    CHECK_EQ(suggestions[0].getTimeSlot().getDay(), 1);
    CHECK_EQ(suggestions[0].getTimeSlot().getStartTimeString(), std::string("09:00"));
    CHECK_EQ(suggestions[0].getTimeDistanceMinutes(), 60);

    int previousScore = -1;
    for (const MeetingTimeSuggestion& suggestion : suggestions) {
        const TimeSlot slot = suggestion.getTimeSlot();
        CHECK(slot.getDay() >= 1 && slot.getDay() <= 5);
        CHECK(slot.getStartMinutes() >= 9 * 60);
        CHECK(slot.getEndMinutes() <= 17 * 60);
        CHECK(!busySlots[0].getTimeSlot().overlapsWith(slot));

        if (previousScore >= 0) {
            CHECK(suggestion.getScore() >= previousScore);
        }
        previousScore = suggestion.getScore();
    }
}

TEST_CASE("Multi-room exam schedule preserves total demand and room capacities") {
    Instructor instructor(2, "Dr. Exam");
    Classroom roomA(401, "A401", 40, true, true, false, true, "ExamBuilding");
    Classroom roomB(402, "A402", 35, true, true, false, true, "ExamBuilding");
    Classroom roomC(403, "A403", 20, true, true, false, true, "ExamBuilding");
    std::vector<User*> users{&instructor};
    std::vector<UserBusySlot> busySlots;
    std::vector<Space*> spaces{&roomA, &roomB, &roomC};
    AllocationService service("multi_room_exam_best_fit", users, busySlots);

    ExamRequest exam(701, &instructor, &roomA, TimeSlot(5, 10, 12),
                     70, "Split Exam", "Exam", "Whiteboard",
                     "ExamBuilding", "CS301", "Algorithms",
                     "Final", true);
    std::vector<Request*> requests{&exam};

    service.processRequests(requests, spaces);

    CHECK(exam.getStatus() == RequestStatus::Approved);
    CHECK(allocationCountForRequest(service.getAllocations(), 701) >= 2);
    CHECK_EQ(totalAssignedForRequest(service.getAllocations(), 701), 70);
    CHECK(allOverlappingSpaceLoadsRespectCapacity(service.getAllocations()));

    for (const Allocation& allocation : service.getAllocations()) {
        CHECK_EQ(allocation.getRequestId(), 701);
        CHECK(sameSlot(allocation.getTimeSlot(), TimeSlot(5, 10, 12)));
        CHECK(allocation.getSpace());
        CHECK(allocation.getAssignedParticipants() <= allocation.getSpace()->getCapacity());
    }
}

TEST_CASE("Rejected oversized exam creates no schedule allocations") {
    Instructor instructor(2, "Dr. Exam");
    Classroom roomA(401, "A401", 40, true, true, false, true, "ExamBuilding");
    Classroom roomB(402, "A402", 35, true, true, false, true, "ExamBuilding");
    std::vector<User*> users{&instructor};
    std::vector<UserBusySlot> busySlots;
    std::vector<Space*> spaces{&roomA, &roomB};
    AllocationService service("multi_room_exam_best_fit", users, busySlots);

    ExamRequest exam(702, &instructor, &roomA, TimeSlot(5, 10, 12),
                     200, "Oversized Exam", "Exam", "Whiteboard",
                     "ExamBuilding", "CS999", "Impossible Exam",
                     "Final", true);
    std::vector<Request*> requests{&exam};

    service.processRequests(requests, spaces);

    CHECK(exam.getStatus() == RequestStatus::Rejected);
    CHECK_EQ(allocationCountForRequest(service.getAllocations(), 702), 0);
    CHECK(service.getAllocations().empty());
}

TEST_CASE("Shared-room exam schedule never exceeds remaining room capacity") {
    Instructor instructor(2, "Dr. Exam");
    Classroom s401(105, "S401", 100, false, true, false, true, "SharedExamBuilding");
    Classroom b202(102, "B202", 20, false, true, false, true, "SharedExamBuilding");
    std::vector<User*> users{&instructor};
    std::vector<UserBusySlot> busySlots;
    std::vector<Space*> spaces{&s401, &b202};

    ExamRequest request7(7, &instructor, &s401, TimeSlot(5, 10, 12),
                         60, "Shared Room Exam A", "Exam", "Whiteboard",
                         "SharedExamBuilding", "PHY301",
                         "Physics Shared Seating", "Midterm", true);
    ExamRequest request8(8, &instructor, &s401, TimeSlot(5, 10, 12),
                         50, "Shared Room Exam B", "Exam", "Whiteboard",
                         "SharedExamBuilding", "PHY302",
                         "Applied Physics Shared Seating", "Midterm", true);
    std::vector<Request*> requests{&request7, &request8};
    AllocationService service("shared_room_exam_best_fit", users, busySlots);

    service.processRequests(requests, spaces);

    CHECK(request7.getStatus() == RequestStatus::Approved);
    CHECK(request8.getStatus() == RequestStatus::Approved);
    CHECK_EQ(totalAssignedForRequest(service.getAllocations(), 7), 60);
    CHECK_EQ(totalAssignedForRequest(service.getAllocations(), 8), 50);
    CHECK_EQ(totalAssignedForSpaceAt(service.getAllocations(), 105, TimeSlot(5, 10, 12)), 100);
    CHECK_EQ(totalAssignedForSpaceAt(service.getAllocations(), 102, TimeSlot(5, 10, 12)), 10);
    CHECK(allOverlappingSpaceLoadsRespectCapacity(service.getAllocations()));
}

TEST_CASE("Structured summaries agree with schedule allocation state") {
    const std::string usersPath = "tests/tmp_schedule_users.csv";
    const std::string spacesPath = "tests/tmp_schedule_spaces.csv";
    const std::string requestsPath = "tests/tmp_schedule_requests.csv";
    const std::string busySlotsPath = "tests/tmp_schedule_busy_slots.csv";
    const std::string participantsPath = "tests/tmp_schedule_participants.csv";
    const std::string allocationsPath = "tests/tmp_schedule_allocations.csv";
    const std::string resultsPath = "tests/tmp_schedule_results.csv";
    const std::string requestSummariesPath = "tests/tmp_schedule_request_summaries.csv";
    const std::string allocationSummariesPath = "tests/tmp_schedule_allocation_summaries.csv";

    writeCsv(usersPath,
             "userId,name,role\n"
             "1,Dr. Scheduler,Instructor\n");
    writeCsv(spacesPath,
             "spaceId,type,name,capacity,hasProjector,hasWhiteboard,hasComputers,isAvailable,building\n"
             "301,MeetingRoom,M301,20,1,1,0,1,AdminBuilding\n");
    writeCsv(requestsPath,
             "requestId,requestType,userId,spaceId,participantCount,requiredFeature,requiredBuilding,timeData,title,purpose,courseCode,courseName,examType,canSplitAcrossRooms\n"
             "801,OneTime,1,301,10,Projector,AdminBuilding,1-09:00-10:00,Approved Schedule Slot,Should appear in schedules,,,,false\n"
             "802,OneTime,1,301,5,Projector,AdminBuilding,1-09:00-10:00,Rejected Schedule Conflict,Should not create allocation,,,,false\n");
    writeCsv(busySlotsPath, "userId,day,startTime,endTime,reason\n");
    writeCsv(participantsPath, "requestId,userId,participantRole\n");

    ResourceAllocationSession session;
    CHECK(session.loadStrategyName("tests/tmp_missing_schedule_config.txt") == "greedy");
    CHECK(session.loadSystemData(usersPath, spacesPath, requestsPath,
                                 busySlotsPath, participantsPath));
    CHECK(session.runAllocation());

    CHECK_EQ(allocationCountForRequest(session.getAllocations(), 801), 1);
    CHECK_EQ(allocationCountForRequest(session.getAllocations(), 802), 0);
    CHECK(session.exportResults(allocationsPath, resultsPath,
                                requestSummariesPath, allocationSummariesPath));

    CHECK_FILE_CONTAINS(requestSummariesPath, "Approved Schedule Slot");
    CHECK_FILE_CONTAINS(requestSummariesPath, "Rejected Schedule Conflict");
    CHECK_FILE_CONTAINS(requestSummariesPath, "Approved");
    CHECK_FILE_CONTAINS(requestSummariesPath, "Rejected");
    CHECK_FILE_CONTAINS(requestSummariesPath, "Time slot unavailable");
    CHECK_FILE_CONTAINS(allocationSummariesPath, "Approved Schedule Slot");
    CHECK(!fileContains(allocationSummariesPath, "Rejected Schedule Conflict"));

    session.cleanup();
    std::remove(usersPath.c_str());
    std::remove(spacesPath.c_str());
    std::remove(requestsPath.c_str());
    std::remove(busySlotsPath.c_str());
    std::remove(participantsPath.c_str());
    std::remove(allocationsPath.c_str());
    std::remove(resultsPath.c_str());
    std::remove(requestSummariesPath.c_str());
    std::remove(allocationSummariesPath.c_str());
}

TEST_CASE("Backend processes a larger request set without timing out or losing schedule entries") {
    Instructor instructor(2, "Dr. Load");
    std::vector<std::unique_ptr<Classroom>> ownedRooms;
    std::vector<Space*> spaces;
    std::vector<std::unique_ptr<OneTimeRequest>> ownedRequests;
    std::vector<Request*> requests;
    std::vector<User*> users{&instructor};
    std::vector<UserBusySlot> busySlots;

    for (int index = 0; index < 20; ++index) {
        ownedRooms.push_back(std::make_unique<Classroom>(
            900 + index,
            "LoadRoom" + std::to_string(index),
            30,
            true,
            true,
            false,
            true,
            "LoadBuilding"
        ));
        spaces.push_back(ownedRooms.back().get());
    }

    for (int index = 0; index < 100; ++index) {
        const int roomIndex = index % 20;
        const int day = (index / 20) + 1;
        ownedRequests.push_back(std::make_unique<OneTimeRequest>(
            9000 + index,
            &instructor,
            spaces[roomIndex],
            TimeSlot(day, 9, 10),
            10,
            "Load Request " + std::to_string(index),
            "Performance smoke",
            "Projector",
            "LoadBuilding"
        ));
        requests.push_back(ownedRequests.back().get());
    }

    AllocationService service("greedy", users, busySlots);
    const auto start = std::chrono::steady_clock::now();
    service.processRequests(requests, spaces);
    const auto end = std::chrono::steady_clock::now();
    const auto elapsedMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    CHECK_EQ(service.getAllocations().size(), static_cast<size_t>(100));
    CHECK(allOverlappingSpaceLoadsRespectCapacity(service.getAllocations()));
    CHECK(elapsedMs < 10000);
}
