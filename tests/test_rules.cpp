#include "test_framework.h"
#include "../src/models/Classroom.h"
#include "../src/models/CommitteeMeetingRequest.h"
#include "../src/models/ExamRequest.h"
#include "../src/models/Instructor.h"
#include "../src/models/Laboratory.h"
#include "../src/models/MeetingRoom.h"
#include "../src/models/OneTimeRequest.h"
#include "../src/models/Student.h"
#include "../src/models/UserBusySlot.h"
#include "../src/rules/CapacityRule.h"
#include "../src/rules/FeatureRule.h"
#include "../src/rules/LocationRule.h"
#include "../src/rules/ParticipantAvailabilityRule.h"
#include "../src/rules/RequestTypeRule.h"
#include "../src/rules/StatusRule.h"
#include "../src/rules/UserRoleRule.h"

TEST_CASE("Physical room rules evaluate current Request API") {
    Student student(1, "Student");
    Classroom room(101, "B201", 40, true, true, false, true, "Engineering");
    Classroom smallRoom(102, "B202", 10, false, true, false, true, "Engineering");
    Laboratory maintenanceLab(201, "L101", 25, false, false, true, false, "LabBuilding");

    OneTimeRequest ok(1, &student, &room, TimeSlot(1, 10, 12),
                      30, "Project", "Meeting", "Projector", "Engineering");
    OneTimeRequest tooLarge(2, &student, &smallRoom, TimeSlot(1, 10, 12),
                            20, "Project", "Meeting", "Whiteboard", "Engineering");
    OneTimeRequest missingFeature(3, &student, &smallRoom, TimeSlot(1, 10, 12),
                                  5, "Project", "Meeting", "Projector", "Engineering");
    OneTimeRequest wrongBuilding(4, &student, &room, TimeSlot(1, 10, 12),
                                 5, "Project", "Meeting", "Projector", "AdminBuilding");
    Instructor instructor(2, "Dr. Rule");
    OneTimeRequest unavailableRoom(5, &instructor, &maintenanceLab, TimeSlot(1, 10, 12),
                                   5, "Lab", "Meeting", "Computers", "LabBuilding");

    CHECK(CapacityRule().evaluate(ok).isPassed());
    CHECK(!CapacityRule().evaluate(tooLarge).isPassed());
    CHECK(FeatureRule().evaluate(ok).isPassed());
    CHECK(!FeatureRule().evaluate(missingFeature).isPassed());
    CHECK(StatusRule().evaluate(ok).isPassed());
    CHECK(!StatusRule().evaluate(unavailableRoom).isPassed());
    CHECK(LocationRule().evaluate(ok).isPassed());
    CHECK(!LocationRule().evaluate(wrongBuilding).isPassed());
}

TEST_CASE("RequestTypeRule and UserRoleRule enforce requester permissions") {
    Student student(1, "Student");
    Instructor instructor(2, "Dr. Instructor");
    Laboratory lab(201, "L101", 25, false, false, true, true, "LabBuilding");
    Classroom classroom(101, "B201", 40, true, true, false, true, "Engineering");

    OneTimeRequest studentLab(1, &student, &lab, TimeSlot(1, 10, 12),
                              10, "Lab", "Meeting", "Computers", "LabBuilding");
    OneTimeRequest instructorLab(2, &instructor, &lab, TimeSlot(1, 10, 12),
                                 10, "Lab", "Meeting", "Computers", "LabBuilding");
    ExamRequest studentExam(3, &student, &classroom, TimeSlot(1, 10, 12),
                            30, "Exam", "Exam", "Whiteboard", "Engineering",
                            "CSE101", "Intro", "Midterm", false);

    CHECK(!UserRoleRule().evaluate(studentLab).isPassed());
    CHECK(UserRoleRule().evaluate(instructorLab).isPassed());
    CHECK(!RequestTypeRule().evaluate(studentExam).isPassed());
}

TEST_CASE("ParticipantAvailabilityRule passes non-committee requests immediately") {
    Student student(1, "Student");
    MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
    OneTimeRequest request(1, &student, &room, TimeSlot(1, 10, 11),
                           5, "One-time", "Meeting", "Projector", "AdminBuilding");
    ParticipantAvailabilityRule rule;

    CHECK(rule.evaluate(request).isPassed());
}

TEST_CASE("ParticipantAvailabilityRule rejects missing committee participants") {
    Student student(1, "Student");
    MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
    CommitteeMeetingRequest request(40, &student, &room, TimeSlot(5, 13, 14),
                                    5, "Defense", "Presentation",
                                    "Projector", "AdminBuilding");
    std::vector<User*> users{&student};
    std::vector<UserBusySlot> busySlots;
    ParticipantAvailabilityRule rule(users, busySlots);

    RuleEvaluationResult result = rule.evaluate(request);

    CHECK(!result.isPassed());
    CHECK_EQ(result.getFailureReason(),
             std::string("Committee meeting has no required participants"));
}

TEST_CASE("ParticipantAvailabilityRule rejects unavailable and unknown participants") {
    Student student(1, "Student");
    Instructor professor(2, "Dr. Ali");
    MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
    std::vector<User*> users{&student, &professor};
    std::vector<UserBusySlot> busySlots{
        UserBusySlot(2, TimeSlot::fromMinutes(1, 10 * 60, 12 * 60), "Lecture")
    };
    ParticipantAvailabilityRule rule(users, busySlots);

    CommitteeMeetingRequest busyRequest(41, &student, &room,
                                        TimeSlot::fromMinutes(1, 10 * 60, 11 * 60),
                                        5, "Busy Meeting", "Presentation",
                                        "Projector", "AdminBuilding");
    busyRequest.addRequiredParticipant(2, "Supervisor");

    RuleEvaluationResult busyResult = rule.evaluate(busyRequest);
    CHECK(!busyResult.isPassed());
    CHECK_CONTAINS(busyResult.getFailureReason(), "Dr. Ali");
    CHECK_CONTAINS(busyResult.getFailureReason(), "Monday 10:00-11:00");

    CommitteeMeetingRequest unknownRequest(42, &student, &room, TimeSlot(5, 13, 14),
                                           5, "Unknown", "Presentation",
                                           "Projector", "AdminBuilding");
    unknownRequest.addRequiredParticipant(999, "Supervisor");

    RuleEvaluationResult unknownResult = rule.evaluate(unknownRequest);
    CHECK(!unknownResult.isPassed());
    CHECK_CONTAINS(unknownResult.getFailureReason(), "ID 999");
}

TEST_CASE("ParticipantAvailabilityRule rejects outside working hours through availability service") {
    Student student(1, "Student");
    Instructor professor(2, "Dr. Ali");
    MeetingRoom room(301, "M301", 10, true, false, false, true, "AdminBuilding");
    std::vector<User*> users{&student, &professor};
    std::vector<UserBusySlot> busySlots;
    ParticipantAvailabilityRule rule(users, busySlots);

    CommitteeMeetingRequest request(43, &student, &room,
                                    TimeSlot::fromMinutes(1, 17 * 60, 18 * 60),
                                    5, "Late Meeting", "Presentation",
                                    "Projector", "AdminBuilding");
    request.addRequiredParticipant(2, "Supervisor");

    RuleEvaluationResult result = rule.evaluate(request);

    CHECK(!result.isPassed());
    CHECK_CONTAINS(result.getFailureReason(), "not available");
}
