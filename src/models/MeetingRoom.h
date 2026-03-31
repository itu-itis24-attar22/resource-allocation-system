#ifndef MEETINGROOM_H
#define MEETINGROOM_H

#include "Space.h"

class MeetingRoom : public Space {
public:
    MeetingRoom(int id, const std::string& name, int capacity);
    std::string getType() const override;
};

#endif