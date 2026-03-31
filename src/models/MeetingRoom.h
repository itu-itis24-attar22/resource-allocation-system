#ifndef MEETINGROOM_H
#define MEETINGROOM_H

#include "Space.h"

class MeetingRoom : public Space {
public:
    MeetingRoom(int id, const std::string& name, int capacity,
                bool hasProjector = false,
                bool hasWhiteboard = false,
                bool hasComputers = false);

    std::string getType() const override;
};

#endif