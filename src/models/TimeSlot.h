#ifndef TIMESLOT_H
#define TIMESLOT_H

class TimeSlot {
private:
    int startHour;
    int endHour;

public:
    TimeSlot(int startHour, int endHour);

    int getStartHour() const;
    int getEndHour() const;

    bool overlapsWith(const TimeSlot& other) const;
};

#endif