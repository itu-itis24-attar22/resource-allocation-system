#include "RuleEvaluationResult.h"

RuleEvaluationResult::RuleEvaluationResult(bool passed, const std::string& failureReason)
    : passed(passed), failureReason(failureReason) {}

bool RuleEvaluationResult::isPassed() const {
    return passed;
}

std::string RuleEvaluationResult::getFailureReason() const {
    return failureReason;
}