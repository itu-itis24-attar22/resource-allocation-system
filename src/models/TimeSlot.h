#ifndef TIMESLOT_H
#define TIMESLOT_H

#include <stdexcept>

class TimeSlot {
private:
    int day;        // 1 = Monday, ..., 7 = Sunday
    int startHour;
    int endHour;

public:
    TimeSlot(int day, int startHour, int endHour);

    int getDay() const;
    int getStartHour() const;
    int getEndHour() const;

    static bool isValid(int day, int startHour, int endHour);

    bool overlapsWith(const TimeSlot& other) const;
};

#endif
