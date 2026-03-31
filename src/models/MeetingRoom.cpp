#include "MeetingRoom.h"

MeetingRoom::MeetingRoom(int id, const std::string& name, int capacity,
                         bool hasProjector, bool hasWhiteboard, bool hasComputers)
    : Space(id, name, capacity, hasProjector, hasWhiteboard, hasComputers) {}

std::string MeetingRoom::getType() const {
    return "MeetingRoom";
}