#ifndef TIMESLOT_H
#define TIMESLOT_H

#include <stdexcept>
#include <string>

class TimeSlot {
private:
    int day;        // 1 = Monday, ..., 7 = Sunday
    int startMinutes;
    int endMinutes;

    TimeSlot(int day, int startMinutes, int endMinutes, bool minuteBased);

public:
    TimeSlot(int day, int startHour, int endHour);

    static TimeSlot fromMinutes(int day, int startMinutes, int endMinutes);

    int getDay() const;
    int getStartMinutes() const;
    int getEndMinutes() const;
    int getStartHour() const;
    int getEndHour() const;
    int getStartMinute() const;
    int getEndMinute() const;
    std::string getStartTimeString() const;
    std::string getEndTimeString() const;

    static bool isValid(int day, int startMinutes, int endMinutes);

    bool overlapsWith(const TimeSlot& other) const;
};

#endif
