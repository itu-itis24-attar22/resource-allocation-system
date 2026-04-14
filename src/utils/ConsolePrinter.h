#ifndef CONSOLEPRINTER_H
#define CONSOLEPRINTER_H

#include <string>
#include "../models/OneTimeRequest.h"
#include "../models/RecurringRequest.h"
#include "../models/Status.h"

std::string requestStatusToString(RequestStatus status);
std::string dayToString(int day);
std::string availabilityToString(bool availability);

void printOneTimeResult(
    const std::string& label,
    const OneTimeRequest& request,
    bool result,
    const std::string& explanation
);

void printRecurringResult(
    const std::string& label,
    const RecurringRequest& request,
    bool result,
    const std::string& explanation
);

#endif