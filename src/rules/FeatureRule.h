#ifndef FEATURERULE_H
#define FEATURERULE_H

#include "IRequestRule.h"

class FeatureRule : public IRequestRule {
public:
    RuleEvaluationResult evaluate(Request& request) const override;
};

#endif