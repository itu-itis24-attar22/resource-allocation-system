#include "test_framework.h"
#include "../src/models/SpaceFactory.h"

#include <memory>

TEST_CASE("SpaceFactory creates classroom spaces") {
    std::unique_ptr<Space> space(
        SpaceFactory::createSpace(101, "B201", "Classroom", 40,
                                  true, true, false, true, "Engineering")
    );

    CHECK(space != nullptr);
    CHECK_EQ(space->getId(), 101);
    CHECK_EQ(space->getName(), std::string("B201"));
    CHECK_EQ(space->getType(), std::string("Classroom"));
    CHECK_EQ(space->getCapacity(), 40);
    CHECK(space->getHasProjector());
    CHECK(space->getHasWhiteboard());
    CHECK(!space->getHasComputers());
    CHECK(space->getIsAvailable());
    CHECK_EQ(space->getBuilding(), std::string("Engineering"));
}

TEST_CASE("SpaceFactory creates laboratory spaces") {
    std::unique_ptr<Space> space(
        SpaceFactory::createSpace(201, "L101", "Laboratory", 25,
                                  false, false, true, true, "LabBuilding")
    );

    CHECK(space != nullptr);
    CHECK_EQ(space->getId(), 201);
    CHECK_EQ(space->getName(), std::string("L101"));
    CHECK_EQ(space->getType(), std::string("Laboratory"));
    CHECK_EQ(space->getCapacity(), 25);
    CHECK(!space->getHasProjector());
    CHECK(!space->getHasWhiteboard());
    CHECK(space->getHasComputers());
    CHECK(space->getIsAvailable());
    CHECK_EQ(space->getBuilding(), std::string("LabBuilding"));
}

TEST_CASE("SpaceFactory creates meeting room spaces") {
    std::unique_ptr<Space> space(
        SpaceFactory::createSpace(301, "M301", "MeetingRoom", 10,
                                  true, false, false, true, "AdminBuilding")
    );

    CHECK(space != nullptr);
    CHECK_EQ(space->getId(), 301);
    CHECK_EQ(space->getName(), std::string("M301"));
    CHECK_EQ(space->getType(), std::string("MeetingRoom"));
    CHECK_EQ(space->getCapacity(), 10);
    CHECK(space->getHasProjector());
    CHECK(!space->getHasWhiteboard());
    CHECK(!space->getHasComputers());
    CHECK(space->getIsAvailable());
    CHECK_EQ(space->getBuilding(), std::string("AdminBuilding"));
}

TEST_CASE("SpaceFactory returns null for unknown space types") {
    std::unique_ptr<Space> space(
        SpaceFactory::createSpace(999, "A1", "Auditorium", 100,
                                  true, true, false, true, "Main")
    );

    CHECK(space == nullptr);
}
