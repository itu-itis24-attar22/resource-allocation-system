#ifndef TIMESLOT_H
#define TIMESLOT_H

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

    bool overlapsWith(const TimeSlot& other) const;
};

#endif