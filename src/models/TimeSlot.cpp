#include "TimeSlot.h"

TimeSlot::TimeSlot(int day, int startHour, int endHour)
    : day(day), startHour(startHour), endHour(endHour) {
    if (!isValid(day, startHour, endHour)) {
        throw std::invalid_argument("Invalid time slot");
    }
}

int TimeSlot::getDay() const {
    return day;
}

int TimeSlot::getStartHour() const {
    return startHour;
}

int TimeSlot::getEndHour() const {
    return endHour;
}

bool TimeSlot::isValid(int day, int startHour, int endHour) {
    if (day < 1 || day > 7) return false;
    if (startHour < 0 || startHour > 23) return false;
    if (endHour < 0 || endHour > 23) return false;
    return startHour < endHour;
}

bool TimeSlot::overlapsWith(const TimeSlot& other) const {
    if (day != other.getDay()) {
        return false;
    }

    return (startHour < other.getEndHour()) &&
           (endHour > other.getStartHour());
}
