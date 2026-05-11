# Resource Allocation System (Iterative Prototype)

This repository contains an iterative C++ prototype for a university resource allocation system, with a small Flask web dashboard for demonstration.

The project focuses on gradually developing a flexible allocation system for resources such as classrooms, laboratories, and meeting rooms. The implementation follows an iterative software design approach, where each iteration introduces one or more new concepts into the prototype.

## Current Prototype Features

The current prototype supports:

- abstract `Request` base class with inheritance
- `OneTimeRequest`, `RecurringRequest`, `ExamRequest`, and `InvalidRequest`
- request creation through `RequestFactory`
- invalid request capture for malformed or unresolved request rows
- allocation of multiple space types:
  - Classroom
  - Laboratory
  - MeetingRoom
- rule-based request evaluation
- centralized rule evaluation using:
  - `RuleEngine`
  - `RuleEngineFacade`
- request priority values
- user polymorphism:
  - Student
  - Instructor
  - Staff
  - Administrator
- user creation through `UserFactory`
- space-type authorization using `UserRoleRule`
- request-type authorization using `RequestTypeRule`
- external CSV-based loading of:
  - users
  - spaces
  - requests
- centralized data loading and exporting through `DataController`
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
  - `PriorityAllocationStrategy`
  - `MultiRoomExamGreedyStrategy`
  - `MultiRoomExamBestFitStrategy`
  - `SharedRoomExamBestFitStrategy`
- strategy selection through `AllocationStrategyFactory` and `data/config.txt`
- `assignedParticipants` stored in each `Allocation`
- minute-based `TimeSlot` support with backward compatibility for older hour-only data
- Flask web dashboard for demo usage
- web-based Add Request page
- web-based Allocation Summary page
- rejection reasons for failed requests

## Implemented Rules

The prototype currently evaluates requests using the following rules:

- `CapacityRule`
- `FeatureRule`
- `StatusRule`
- `LocationRule`
- `UserRoleRule`
- `RequestTypeRule`
- `AvailabilityRule`

`UserRoleRule` checks whether a user can request the selected space type. `RequestTypeRule` checks whether a user can submit the selected request type, such as an exam request.

## Repository Structure

- `docs/srs/` : Software Requirements Specification
- `docs/analysis/` : Initial domain analysis and domain model
- `docs/iterations/` : Iteration reports
- `src/` : C++ prototype implementation
  - `src/models/` : domain models, including requests, factories, users, spaces, allocations, and time slots
  - `src/rules/` : rule engine, rule facade, and individual request rules
  - `src/services/` : allocation service coordination
  - `src/strategies/` : allocation strategy interface, greedy strategy, priority strategy, and multi-room exam strategies
  - `src/data/` : CSV loading and export components
  - `src/utils/` : console output helpers
- `web/` : Flask dashboard for viewing data, adding requests, running the backend, and reviewing allocation summaries
- `data/` : CSV input/output files and configuration
- `tests/` : unit testing files
- `external/` : third-party single-header libraries

## Data Files

The prototype currently uses CSV files in the `data/` folder:

- `users.csv`
- `spaces.csv`
- `requests.csv`
- `config.txt`
- `allocations.csv` (generated output)
- `request_results.csv` (generated output)

The selected allocation strategy is configured in `data/config.txt`, for example:

```text
allocation_strategy=shared_room_exam_best_fit
```

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
- Iteration 19: explicit greedy batch processing
- Iteration 20: `AllocationStrategyFactory` and config-based strategy selection
- Iteration 21: `PriorityAllocationStrategy`
- Iteration 22: user polymorphism and `UserFactory`
- Iteration 23: `RequestFactory`
- Iteration 24: request title and purpose metadata
- Iteration 25: `ExamRequest`
- Iteration 26: `RequestTypeRule`
- Iteration 27: `assignedParticipants` and strategy access to the full space pool
- Iteration 28: `MultiRoomExamGreedyStrategy`
- Iteration 29: `MultiRoomExamBestFitStrategy`
- Iteration 30: minute-based `TimeSlot` support
- Iteration 31: extended user and organization model
- Iteration 32: `SharedRoomExamBestFitStrategy` for exam-to-exam same-room capacity sharing

## How to Compile and Run Backend

From PowerShell in the project root:

```powershell
$sources = Get-ChildItem -Recurse -Path src -Filter *.cpp | ForEach-Object { $_.FullName }
g++ -std=c++17 $sources -o allocation_system
```

Run:

```bash
./allocation_system
```

On Windows:

```powershell
.\allocation_system.exe
```

The backend loads CSV files, reads `data/config.txt`, processes requests with the selected strategy, prints request outcomes, and exports `allocations.csv` and `request_results.csv`.

## How to Run Web Dashboard

Install Flask:

```bash
pip install flask
```

Run:

```bash
python web/app.py
```

Open:

```text
http://127.0.0.1:5000
```

The web dashboard does not implement allocation logic. It reads and writes CSV files for demo usage and runs the compiled C++ backend. The C++ backend remains responsible for rules, strategies, and allocation.

## Demo Workflow

1. Compile the backend.
2. Run the Flask dashboard.
3. Select an allocation strategy.
4. Add or review requests.
5. Click `Run Allocation`.
6. Open `Allocation Summary`.

## Notes

This repository is an iterative academic prototype. The current implementation focuses on architecture, extensibility, and rule-driven evaluation rather than production-level persistence or user interface support.

The allocation flow is now designed so that strategy-based algorithms can be extended in later iterations without redesigning the rest of the system.

Future work can build on the current structure with a priority + best-fit strategy, global optimization across simultaneous exams, stronger database or API integration, and a richer web interface.
