#include "ExamRequest.h"

namespace {
    std::string normalizeCourseCode(const std::string& value) {
        return value.empty() ? "UNKNOWN" : value;
    }

    std::string normalizeCourseName(const std::string& value) {
        return value.empty() ? "Unknown Course" : value;
    }

    std::string normalizeExamType(const std::string& value) {
        return value.empty() ? "General" : value;
    }
}

ExamRequest::ExamRequest(int requestId, User* requester, Space* requestedSpace,
                         const TimeSlot& examTimeSlot, int participantCount,
                         const std::string& title,
                         const std::string& purpose,
                         const std::string& requiredFeature,
                         const std::string& requiredBuilding,
                         const std::string& courseCode,
                         const std::string& courseName,
                         const std::string& examType,
                         bool canSplitAcrossRooms)
    : Request(requestId, requester, requestedSpace, participantCount,
              title, purpose, requiredFeature, requiredBuilding),
      courseCode(normalizeCourseCode(courseCode)),
      courseName(normalizeCourseName(courseName)),
      examType(normalizeExamType(examType)),
      canSplitAcrossRooms(canSplitAcrossRooms),
      examTimeSlot(examTimeSlot) {}

std::string ExamRequest::getCourseCode() const {
    return courseCode;
}

std::string ExamRequest::getCourseName() const {
    return courseName;
}

std::string ExamRequest::getExamType() const {
    return examType;
}

bool ExamRequest::getCanSplitAcrossRooms() const {
    return canSplitAcrossRooms;
}

TimeSlot ExamRequest::getExamTimeSlot() const {
    return examTimeSlot;
}

bool ExamRequest::isRecurring() const {
    return false;
}

std::string ExamRequest::getRequestType() const {
    return "Exam";
}
