# Resource Allocation System (Iterative Prototype)

This repository contains an iterative C++ prototype for a university resource allocation system.

The project focuses on gradually developing a flexible allocation system for resources such as classrooms, laboratories, and meeting rooms. The implementation follows an iterative software design approach, where each iteration introduces one or more new concepts into the prototype.

## Current Prototype Features

The current prototype supports:

- one-time requests
- recurring requests
- abstract request base class with inheritance
- allocation of multiple space types:
  - Classroom
  - Laboratory
  - MeetingRoom
- rule-based request evaluation
- centralized rule evaluation using:
  - RuleEngine
  - RuleEngineFacade
- external CSV-based loading of:
  - users
  - spaces
- CSV export of current allocations
- structured user roles:
  - Student
  - Instructor
  - Staff
  - Administrator
- role-based authorization using `UserRoleRule`
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
- `data/` : CSV input/output files
- `tests/` : unit testing files
- `external/` : third-party single-header libraries

## Data Files

The prototype currently uses CSV files in the `data/` folder:

- `users.csv`
- `spaces.csv`
- `allocations.csv` (generated output)

## Build and Run

Example compilation command:

```bash
g++ -std=c++17 src/main.cpp src/utils/ConsolePrinter.cpp src/data/DataLoader.cpp src/models/User.cpp src/models/Space.cpp src/models/Classroom.cpp src/models/Laboratory.cpp src/models/MeetingRoom.cpp src/models/TimeSlot.cpp src/models/Request.cpp src/models/OneTimeRequest.cpp src/models/RecurringRequest.cpp src/models/Allocation.cpp src/rules/AvailabilityRule.cpp src/rules/CapacityRule.cpp src/rules/FeatureRule.cpp src/rules/StatusRule.cpp src/rules/LocationRule.cpp src/rules/UserRoleRule.cpp src/rules/RuleEvaluationResult.cpp src/rules/RuleEngine.cpp src/rules/RuleEngineFacade.cpp src/services/AllocationService.cpp -o allocation_system

Run:
./allocation_system


Iterative Development Summary
Iteration 1–4: basic one-time request support and initial rules
Iteration 5–6: recurring requests and day-based time slots
Iteration 7–10: feature, status, location, and rejection-reason support
Iteration 11: CSV-based input loading
Iteration 12: abstract Request base class
Iteration 13: rule engine and facade
Iteration 14: structured user roles and role-based authorization

Note:
This repository is an iterative academic prototype. The current implementation focuses on architecture, extensibility, and rule-driven evaluation rather than production-level persistence or user interface support.