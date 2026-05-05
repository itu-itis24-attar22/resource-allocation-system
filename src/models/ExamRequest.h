#ifndef EXAMREQUEST_H
#define EXAMREQUEST_H

#include <string>
#include "Request.h"
#include "TimeSlot.h"

class ExamRequest : public Request {
private:
    std::string courseCode;
    std::string courseName;
    std::string examType;
    bool canSplitAcrossRooms;
    TimeSlot examTimeSlot;

public:
    ExamRequest(int requestId, User* requester, Space* requestedSpace,
                const TimeSlot& examTimeSlot, int participantCount,
                const std::string& title = "",
                const std::string& purpose = "",
                const std::string& requiredFeature = "",
                const std::string& requiredBuilding = "",
                const std::string& courseCode = "",
                const std::string& courseName = "",
                const std::string& examType = "",
                bool canSplitAcrossRooms = false);

    std::string getCourseCode() const;
    std::string getCourseName() const;
    std::string getExamType() const;
    bool getCanSplitAcrossRooms() const;
    TimeSlot getExamTimeSlot() const;

    bool isRecurring() const override;
    std::string getRequestType() const override;
};

#endif
