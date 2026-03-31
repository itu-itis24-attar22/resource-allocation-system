#include "MeetingRoom.h"

MeetingRoom::MeetingRoom(int id, const std::string& name, int capacity,
                         bool hasProjector, bool hasWhiteboard, bool hasComputers,
                         bool isAvailable, const std::string& building)
    : Space(id, name, capacity, hasProjector, hasWhiteboard, hasComputers, isAvailable, building) {}

std::string MeetingRoom::getType() const {
    return "MeetingRoom";
}