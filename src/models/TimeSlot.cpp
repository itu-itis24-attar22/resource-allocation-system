#include "TimeSlot.h"

TimeSlot::TimeSlot(int day, int startHour, int endHour)
    : day(day), startHour(startHour), endHour(endHour) {}

int TimeSlot::getDay() const {
    return day;
}

int TimeSlot::getStartHour() const {
    return startHour;
}

int TimeSlot::getEndHour() const {
    return endHour;
}

bool TimeSlot::overlapsWith(const TimeSlot& other) const {
    if (day != other.getDay()) {
        return false;
    }

    return (startHour < other.getEndHour()) &&
           (endHour > other.getStartHour());
}