#include "TimeSlot.h"
#include <iomanip>
#include <sstream>

TimeSlot::TimeSlot(int day, int startHour, int endHour)
    : TimeSlot(day, startHour * 60, endHour * 60, true) {}

TimeSlot::TimeSlot(int day, int startMinutes, int endMinutes, bool minuteBased)
    : day(day), startMinutes(startMinutes), endMinutes(endMinutes) {
    (void)minuteBased;

    if (!isValid(day, startMinutes, endMinutes)) {
        throw std::invalid_argument("Invalid time slot");
    }
}

TimeSlot TimeSlot::fromMinutes(int day, int startMinutes, int endMinutes) {
    return TimeSlot(day, startMinutes, endMinutes, true);
}

int TimeSlot::getDay() const {
    return day;
}

int TimeSlot::getStartMinutes() const {
    return startMinutes;
}

int TimeSlot::getEndMinutes() const {
    return endMinutes;
}

int TimeSlot::getStartHour() const {
    return startMinutes / 60;
}

int TimeSlot::getEndHour() const {
    return endMinutes / 60;
}

int TimeSlot::getStartMinute() const {
    return startMinutes % 60;
}

int TimeSlot::getEndMinute() const {
    return endMinutes % 60;
}

std::string TimeSlot::getStartTimeString() const {
    std::ostringstream output;
    output << std::setfill('0') << std::setw(2) << getStartHour()
           << ":" << std::setw(2) << getStartMinute();
    return output.str();
}

std::string TimeSlot::getEndTimeString() const {
    std::ostringstream output;
    output << std::setfill('0') << std::setw(2) << getEndHour()
           << ":" << std::setw(2) << getEndMinute();
    return output.str();
}

bool TimeSlot::isValid(int day, int startMinutes, int endMinutes) {
    if (day < 1 || day > 7) return false;
    if (startMinutes < 0 || startMinutes >= 24 * 60) return false;
    if (endMinutes <= 0 || endMinutes > 24 * 60) return false;
    return startMinutes < endMinutes;
}

bool TimeSlot::overlapsWith(const TimeSlot& other) const {
    if (day != other.getDay()) {
        return false;
    }

    return startMinutes < other.getEndMinutes() &&
           other.getStartMinutes() < endMinutes;
}
