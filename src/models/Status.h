#ifndef STATUS_H
#define STATUS_H

enum class RequestStatus {
    Pending,
    Approved,
    Rejected
};

enum class AllocationStatus {
    Active,
    Cancelled
};

#endif