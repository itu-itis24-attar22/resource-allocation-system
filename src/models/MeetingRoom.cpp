#include "MeetingRoom.h"

MeetingRoom::MeetingRoom(int id, const std::string& name, int capacity)
    : Space(id, name, capacity) {}

std::string MeetingRoom::getType() const {
    return "MeetingRoom";
}