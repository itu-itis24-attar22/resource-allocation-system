#ifndef REQUESTRESULTWRITER_H
#define REQUESTRESULTWRITER_H

#include <string>
#include <vector>
#include "../models/Request.h"

class RequestResultWriter {
public:
    static void writeRequestResults(const std::string& filename,
                                    const std::vector<Request*>& requests);
};

#endif