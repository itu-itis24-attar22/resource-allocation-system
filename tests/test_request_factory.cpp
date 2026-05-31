#include "test_framework.h"
#include "test_helpers.h"
#include "../src/models/Classroom.h"
#include "../src/models/CommitteeMeetingRequest.h"
#include "../src/models/ExamRequest.h"
#include "../src/models/InvalidRequest.h"
#include "../src/models/OneTimeRequest.h"
#include "../src/models/RecurringRequest.h"
#include "../src/models/RequestFactory.h"
#include "../src/models/Student.h"

TEST_CASE("RequestFactory creates supported request subtypes") {
    Student student(1, "Student");
    Classroom room(101, "B201", 40, true, true, false, true, "Engineering");

    Request* oneTime = RequestFactory::createRequest(
        1, "OneTime", &student, &room, 20,
        "Project Meeting", "Meeting", "Projector", "Engineering", "1-10-12"
    );
    Request* recurring = RequestFactory::createRequest(
        2, "Recurring", &student, &room, 20,
        "Weekly Meeting", "Meeting", "Projector", "Engineering",
        "1-10-12;3-10:30-11:30"
    );
    Request* committee = RequestFactory::createRequest(
        3, "CommitteeMeeting", &student, &room, 5,
        "Defense", "Presentation", "Projector", "Engineering", "5-13:00-14:00"
    );

    CHECK(dynamic_cast<OneTimeRequest*>(oneTime) != nullptr);
    CHECK(dynamic_cast<RecurringRequest*>(recurring) != nullptr);
    CHECK(dynamic_cast<CommitteeMeetingRequest*>(committee) != nullptr);
    CHECK_EQ(oneTime->getTitle(), std::string("Project Meeting"));
    CHECK_EQ(oneTime->getPurpose(), std::string("Meeting"));

    RecurringRequest* recurringRequest = dynamic_cast<RecurringRequest*>(recurring);
    REQUIRE(recurringRequest);
    CHECK_EQ(recurringRequest->getRequestedTimeSlots().size(), static_cast<size_t>(2));
    CHECK_EQ(recurringRequest->getRequestedTimeSlots()[1].getStartTimeString(), std::string("10:30"));

    delete oneTime;
    delete recurring;
    delete committee;
}

TEST_CASE("RequestFactory creates ExamRequest with exam metadata") {
    Student student(1, "Student");
    Classroom room(101, "B201", 40, true, true, false, true, "Engineering");

    Request* request = RequestFactory::createRequest(
        4, "Exam", &student, &room, 30,
        "Midterm", "Exam", "Whiteboard", "Engineering", "2-09:00-11:00",
        "CSE101", "Intro", "Midterm", true
    );
    ExamRequest* exam = dynamic_cast<ExamRequest*>(request);

    REQUIRE(exam);
    CHECK_EQ(exam->getCourseCode(), std::string("CSE101"));
    CHECK_EQ(exam->getCourseName(), std::string("Intro"));
    CHECK_EQ(exam->getExamType(), std::string("Midterm"));
    CHECK(exam->getCanSplitAcrossRooms());
    CHECK_EQ(exam->getExamTimeSlot().getStartTimeString(), std::string("09:00"));

    delete request;
}

TEST_CASE("RequestFactory creates InvalidRequest for bad request types and malformed times") {
    Student student(1, "Student");
    Classroom room(101, "B201", 40, true, true, false, true, "Engineering");

    Request* unknown = RequestFactory::createRequest(
        5, "Magic", &student, &room, 10,
        "Unknown", "Test", "", "", "1-10-11"
    );
    Request* malformedOneTime = RequestFactory::createRequest(
        6, "OneTime", &student, &room, 10,
        "Bad", "Test", "", "", "Monday 10"
    );
    Request* malformedRecurring = RequestFactory::createRequest(
        7, "Recurring", &student, &room, 10,
        "Bad Recurring", "Test", "", "", "1-10-11;bad"
    );

    CHECK(dynamic_cast<InvalidRequest*>(unknown) != nullptr);
    CHECK(dynamic_cast<InvalidRequest*>(malformedOneTime) != nullptr);
    CHECK(dynamic_cast<InvalidRequest*>(malformedRecurring) != nullptr);
    CHECK(unknown->getStatus() == RequestStatus::Rejected);
    CHECK(malformedOneTime->getStatus() == RequestStatus::Rejected);
    CHECK(malformedRecurring->getStatus() == RequestStatus::Rejected);

    delete unknown;
    delete malformedOneTime;
    delete malformedRecurring;
}

TEST_CASE("Request stores default metadata and lifecycle history") {
    Student student(1, "Student");
    Classroom room(101, "B201", 40, true, true, false, true, "Engineering");
    OneTimeRequest request(8, &student, &room, TimeSlot(1, 10, 11),
                           10, "", "", "", "");

    CHECK_EQ(request.getTitle(), std::string("Untitled Request"));
    CHECK_EQ(request.getPurpose(), std::string("General"));
    CHECK(historyContains(request, "created"));

    request.addHistoryEvent("custom event");
    request.markApproved();

    CHECK(request.getStatus() == RequestStatus::Approved);
    CHECK(historyContains(request, "custom event"));
    CHECK(historyContains(request, "approved"));

    request.markRejected("manual rejection");
    CHECK(request.getStatus() == RequestStatus::Rejected);
    CHECK_EQ(request.getRejectionReason(), std::string("manual rejection"));
    CHECK(historyContains(request, "rejected: manual rejection"));
}
