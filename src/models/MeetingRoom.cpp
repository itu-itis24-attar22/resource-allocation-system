#include "MeetingRoom.h"

MeetingRoom::MeetingRoom(int id, const std::string& name, int capacity,
                         bool hasProjector, bool hasWhiteboard, bool hasComputers,
                         bool isAvailable)
    : Space(id, name, capacity, hasProjector, hasWhiteboard, hasComputers, isAvailable) {}

std::string MeetingRoom::getType() const {
    return "MeetingRoom";
}