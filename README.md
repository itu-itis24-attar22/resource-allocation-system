# Resource Allocation System (Iterative Prototype)

This repository contains an iterative C++ prototype for a university resource allocation system.

The project focuses on gradually developing a flexible allocation system for resources such as classrooms, laboratories, and meeting rooms. The implementation follows an iterative software design approach, where each iteration introduces one or more new concepts into the prototype.

## Current Prototype Features

The current prototype supports:

- one-time requests
- recurring requests
- invalid request capture for malformed or unresolved request rows
- abstract request base class with inheritance
- allocation of multiple space types:
  - Classroom
  - Laboratory
  - MeetingRoom
- rule-based request evaluation
- centralized rule evaluation using:
  - `RuleEngine`
  - `RuleEngineFacade`
- request priority values
- structured user roles:
  - Student
  - Instructor
  - Staff
  - Administrator
- role-based authorization using `UserRoleRule`
- external CSV-based loading of:
  - users
  - spaces
  - requests
- CSV export of:
  - allocations
  - request results
- request lifecycle history logging:
  - created
  - evaluated
  - approved or rejected
  - exported
- allocation strategy support using:
  - `IAllocationStrategy`
  - `GreedyAllocationStrategy`
- rejection reasons for failed requests

## Implemented Rules

The prototype currently evaluates requests using the following rules:

- `CapacityRule`
- `FeatureRule`
- `StatusRule`
- `LocationRule`
- `UserRoleRule`
- `AvailabilityRule`

## Repository Structure

- `docs/srs/` : Software Requirements Specification
- `docs/analysis/` : Initial domain analysis and domain model
- `docs/iterations/` : Iteration reports
- `src/` : C++ prototype implementation
  - `src/models/` : domain models such as requests, spaces, users, allocations, and time slots
  - `src/rules/` : rule engine, rule facade, and individual request rules
  - `src/services/` : allocation service coordination
  - `src/strategies/` : allocation strategy interface and greedy strategy
  - `src/data/` : CSV loading and export components
  - `src/utils/` : console output helpers
- `data/` : CSV input/output files and adversarial test datasets
- `tests/` : unit testing files
- `external/` : third-party single-header libraries

## Data Files

The prototype currently uses CSV files in the `data/` folder:

- `users.csv`
- `spaces.csv`
- `requests.csv`
- `allocations.csv` (generated output)
- `request_results.csv` (generated output)

## Testing and Robustness Report

A formal testing summary is available in [docs/testing_and_robustness_report.pdf](docs/testing_and_robustness_report.pdf).

The report covers normal workflow verification, malformed and adversarial input testing, regression results, bugs found and fixed, and the final robustness status of the allocation system.

## Iterative Development Summary

- Iterations 1-4: basic one-time request support and initial allocation rules
- Iterations 5-6: recurring requests and day-based time slots
- Iterations 7-10: feature, status, location, and rejection-reason support
- Iteration 11: CSV-based data loading
- Iteration 12: abstract `Request` base class and request polymorphism
- Iteration 13: centralized `RuleEngine` and `RuleEngineFacade`
- Iteration 14: structured user roles and role-based authorization
- Iteration 15: request priority support integrated with the request model
- Iteration 16: `DataController` introduced with CSV-based request loading and result exporting
- Iteration 17: request lifecycle history logging added and exported in request results
- Iteration 18: allocation strategy pattern introduced with `IAllocationStrategy` and default `GreedyAllocationStrategy`

## How to Run

Example compilation command:

```bash
g++ -std=c++17 src/main.cpp src/utils/ConsolePrinter.cpp src/data/DataLoader.cpp src/data/DataController.cpp src/data/AllocationWriter.cpp src/data/RequestResultWriter.cpp src/models/User.cpp src/models/Space.cpp src/models/Classroom.cpp src/models/Laboratory.cpp src/models/MeetingRoom.cpp src/models/TimeSlot.cpp src/models/Request.cpp src/models/OneTimeRequest.cpp src/models/RecurringRequest.cpp src/models/InvalidRequest.cpp src/models/Allocation.cpp src/rules/AvailabilityRule.cpp src/rules/CapacityRule.cpp src/rules/FeatureRule.cpp src/rules/StatusRule.cpp src/rules/LocationRule.cpp src/rules/UserRoleRule.cpp src/rules/RuleEvaluationResult.cpp src/rules/RuleEngine.cpp src/rules/RuleEngineFacade.cpp src/strategies/GreedyAllocationStrategy.cpp src/services/AllocationService.cpp -o allocation_system
```

Run:

```bash
./allocation_system
```

The program loads `users.csv`, `spaces.csv`, and `requests.csv`, processes all requests, prints request outcomes to the console, and exports results to `allocations.csv` and `request_results.csv`.

## Notes

This repository is an iterative academic prototype. The current implementation focuses on architecture, extensibility, and rule-driven evaluation rather than production-level persistence or user interface support.

The allocation flow is now designed so that strategy-based algorithms can be extended in later iterations without redesigning the rest of the system.

Future work can build on the current structure by introducing stronger optimization approaches, such as priority-aware scheduling and best-fit resource selection.
