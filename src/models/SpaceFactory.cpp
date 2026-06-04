#include "SpaceFactory.h"
#include "Classroom.h"
#include "Laboratory.h"
#include "MeetingRoom.h"

Space* SpaceFactory::createSpace(int id,
                                 const std::string& name,
                                 const std::string& type,
                                 int capacity,
                                 bool hasProjector,
                                 bool hasWhiteboard,
                                 bool hasComputers,
                                 bool isAvailable,
                                 const std::string& building) {
    if (type == "Classroom") {
        return new Classroom(id, name, capacity,
                             hasProjector, hasWhiteboard, hasComputers,
                             isAvailable, building);
    }

    if (type == "Laboratory") {
        return new Laboratory(id, name, capacity,
                              hasProjector, hasWhiteboard, hasComputers,
                              isAvailable, building);
    }

    if (type == "MeetingRoom") {
        return new MeetingRoom(id, name, capacity,
                               hasProjector, hasWhiteboard, hasComputers,
                               isAvailable, building);
    }

    return nullptr;
}
