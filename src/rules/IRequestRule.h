#ifndef IREQUESTRULE_H
#define IREQUESTRULE_H

#include "../models/Request.h"
#include "RuleEvaluationResult.h"

class IRequestRule {
public:
    virtual ~IRequestRule() = default;

    virtual RuleEvaluationResult evaluate(Request& request) const = 0;
};

#endif