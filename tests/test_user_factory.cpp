#include "test_framework.h"
#include "../src/models/Administrator.h"
#include "../src/models/Instructor.h"
#include "../src/models/Staff.h"
#include "../src/models/Student.h"
#include "../src/models/TeachingAssistant.h"
#include "../src/models/UserFactory.h"

TEST_CASE("UserFactory creates supported polymorphic user types") {
    UserProfileData profile;
    profile.email = "person@example.edu";
    profile.status = "active";
    profile.primaryUnitName = "Engineering";

    User* student = UserFactory::createUser(1, "Student", "Student", profile);
    User* instructor = UserFactory::createUser(2, "Instructor", "Instructor", profile);
    User* assistant = UserFactory::createUser(3, "Assistant", "TeachingAssistant", profile);
    User* staff = UserFactory::createUser(4, "Staff", "Staff", profile);
    User* administrator = UserFactory::createUser(5, "Admin", "Administrator", profile);

    REQUIRE(student);
    REQUIRE(instructor);
    REQUIRE(assistant);
    REQUIRE(staff);
    REQUIRE(administrator);

    CHECK(dynamic_cast<Student*>(student) != nullptr);
    CHECK(dynamic_cast<Instructor*>(instructor) != nullptr);
    CHECK(dynamic_cast<TeachingAssistant*>(assistant) != nullptr);
    CHECK(dynamic_cast<Staff*>(staff) != nullptr);
    CHECK(dynamic_cast<Administrator*>(administrator) != nullptr);
    CHECK_EQ(student->getEmail(), std::string("person@example.edu"));
    CHECK_EQ(student->getPrimaryUnitName(), std::string("Engineering"));

    delete student;
    delete instructor;
    delete assistant;
    delete staff;
    delete administrator;
}

TEST_CASE("UserFactory handles unknown role safely") {
    User* unknown = UserFactory::createUser(99, "Unknown", "Dean");

    CHECK(unknown == nullptr);
}

TEST_CASE("User priorities match current polymorphic model") {
    Student student(1, "Student");
    TeachingAssistant assistant(2, "Assistant");
    Staff staff(3, "Staff");
    Instructor instructor(4, "Instructor");
    Administrator administrator(5, "Admin");

    CHECK_EQ(student.getPriority(), 1);
    CHECK_EQ(assistant.getPriority(), 2);
    CHECK_EQ(staff.getPriority(), 3);
    CHECK_EQ(instructor.getPriority(), 4);
    CHECK_EQ(administrator.getPriority(), 5);
}

TEST_CASE("User space permissions are enforced by subclasses") {
    Student student(1, "Student");
    Staff staff(2, "Staff");
    Instructor instructor(3, "Instructor");
    Administrator administrator(4, "Admin");

    CHECK(!student.canRequestSpaceType("Laboratory"));
    CHECK(instructor.canRequestSpaceType("Laboratory"));
    CHECK(!staff.canRequestSpaceType("Classroom"));
    CHECK(staff.canRequestSpaceType("MeetingRoom"));
    CHECK(administrator.canRequestSpaceType("Classroom"));
    CHECK(administrator.canRequestSpaceType("Laboratory"));
    CHECK(administrator.canRequestSpaceType("MeetingRoom"));
}

TEST_CASE("User request-type permissions are enforced by subclasses") {
    Student student(1, "Student");
    Instructor instructor(2, "Instructor");
    Administrator administrator(3, "Admin");

    CHECK(student.canSubmitRequestType("OneTime"));
    CHECK(student.canSubmitRequestType("CommitteeMeeting"));
    CHECK(!student.canSubmitRequestType("Exam"));
    CHECK(instructor.canSubmitRequestType("Exam"));
    CHECK(administrator.canSubmitRequestType("Exam"));
}
