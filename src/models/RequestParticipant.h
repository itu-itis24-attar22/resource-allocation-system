#ifndef REQUESTPARTICIPANT_H
#define REQUESTPARTICIPANT_H

#include <string>

class RequestParticipant {
private:
    int requestId;
    int userId;
    std::string participantRole;

public:
    RequestParticipant(int requestId, int userId, const std::string& participantRole);

    int getRequestId() const;
    int getUserId() const;
    std::string getParticipantRole() const;
};

#endif
