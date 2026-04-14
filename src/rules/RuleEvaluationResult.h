#ifndef RULEEVALUATIONRESULT_H
#define RULEEVALUATIONRESULT_H

#include <string>

class RuleEvaluationResult {
private:
    bool passed;
    std::string failureReason;

public:
    RuleEvaluationResult(bool passed = true, const std::string& failureReason = "");

    bool isPassed() const;
    std::string getFailureReason() const;
};

#endif