#include "TimeSlot.h"

TimeSlot::TimeSlot(int startHour, int endHour)
    : startHour(startHour), endHour(endHour) {}

int TimeSlot::getStartHour() const {
    return startHour;
}

int TimeSlot::getEndHour() const {
    return endHour;
}

bool TimeSlot::overlapsWith(const TimeSlot& other) const {
    return (startHour < other.getEndHour()) && (endHour > other.getStartHour());
}