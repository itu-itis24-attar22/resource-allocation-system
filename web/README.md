# Resource Allocation Dashboard

This folder contains a small Flask dashboard for the C++ Resource Allocation System.

The web dashboard does not run allocation logic or modify the C++ backend. It displays data files from the project `data/` folder, can update the selected strategy in `config.txt`, can append new request rows to `requests.csv`, and can run the compiled backend executable for demo purposes.

## Install Flask

```bash
pip install flask
```

## Compile The C++ Backend

From PowerShell in the project root:

```powershell
$sources = Get-ChildItem -Recurse -Path src -Filter *.cpp | ForEach-Object { $_.FullName }
g++ -std=c++17 $sources -o allocation_system
```

The dashboard looks for one of these executables in the project root:

- `allocation_system.exe`
- `allocation_system_test.exe`
- `allocation_system`

## Run Flask From The Project Root

```bash
python web/app.py
```

## Run From The Web Folder

```bash
cd web
python app.py
```

## Open The Dashboard

Open this address in a browser:

```text
http://127.0.0.1:5000
```

Click `Run Allocation` on the home page to run the compiled C++ backend executable. The web app uses a fixed executable lookup, runs it from the project root, and then displays the updated `allocations.csv` and `request_results.csv` files.

## Select A Strategy

On the dashboard home page:

1. Choose a strategy from the dropdown.
2. Click `Save Strategy`.
3. Click `Run Allocation`.

Saving a strategy updates `data/config.txt` with:

```text
allocation_strategy=<selected_strategy>
```

The Flask app does not implement strategy logic. It only updates the config file. The C++ backend reads `config.txt` and uses `AllocationStrategyFactory` when the allocation executable runs.

## Add A Request

Use the `Add Request` page from the navigation bar.

The form lets the user choose:

- `OneTime`
- `Recurring`
- `Exam`

It collects the common request fields first, then asks for the time format needed by the selected type. Exam-specific fields are used only when the selected request type is `Exam`.

When submitted, Flask appends one row to `data/requests.csv` using this format:

```text
requestId,requestType,userId,spaceId,participantCount,requiredFeature,requiredBuilding,timeData,title,purpose,courseCode,courseName,examType,canSplitAcrossRooms
```

For `OneTime` rows:

- `requestType` is `OneTime`.
- `timeData` is written as `day-HH:MM-HH:MM`.
- exam-specific fields are empty.
- `canSplitAcrossRooms` is `false`.

For `Recurring` rows:

- `requestType` is `Recurring`.
- `timeData` is entered directly, for example `2-9-10;4-9-10` or `2-09:00-10:30;4-13:15-14:00`.
- exam-specific fields are empty.
- `canSplitAcrossRooms` is `false`.

For `Exam` rows:

- `requestType` is `Exam`.
- `timeData` is written as `day-HH:MM-HH:MM`.
- course code, course name, exam type, and split support are written from the exam fields.

For all new rows:

- blank feature or building constraints are written as `None`.
- `requestId` is generated as the current maximum numeric request ID plus one.

The form only performs basic input validation, such as valid user ID, valid space ID, positive participant count, title, purpose, and the required time fields. Backend validation, request construction, permissions, rule checking, and allocation still happen in the C++ program.

After adding a request, the dashboard shows a confirmation page. The request is saved immediately, but it remains pending until allocation is run. From the confirmation page, the user can:

- add another request,
- view the raw requests table,
- run allocation immediately.

If `Run Allocation Now` is clicked, Flask runs the compiled C++ backend and redirects to `Allocation Summary` with the newly added request highlighted.

## View Allocation Summary

Use the `Allocation Summary` page to see all request types grouped by request ID.

The page reads existing CSV files only:

- `requests.csv` identifies requests.
- `request_results.csv` provides approval or rejection status.
- `allocations.csv` provides room assignments and assigned participants.
- `spaces.csv` provides readable room names, capacity, type, and building.
- `users.csv` provides requester names.

One-time requests show their assigned space when approved. Recurring requests show each allocation occurrence. Exam requests show course details and grouped room assignments, including total assigned participants and unused capacity when room capacities are available.

If allocation outputs are missing, the page shows a friendly message asking the user to run allocation first.

## Pages

- `/` shows the selected strategy and quick data counts.
- `/users` shows `data/users.csv`.
- `/spaces` shows `data/spaces.csv`.
- `/requests` shows `data/requests.csv`.
- `/add-request` adds a new `OneTime`, `Recurring`, or `Exam` request row to `data/requests.csv`.
- `/allocation-summary` groups all allocation results by request.
- `/add-one-time` redirects to `/add-request?type=OneTime`.
- `/add-exam` redirects to `/add-request?type=Exam`.
- `/exam-summary` redirects to `/allocation-summary`.
- `/allocations` shows `data/allocations.csv` if it exists.
- `/results` shows `data/request_results.csv` if it exists.

If `allocations.csv` or `request_results.csv` is missing, the dashboard shows a friendly message instead of crashing.
