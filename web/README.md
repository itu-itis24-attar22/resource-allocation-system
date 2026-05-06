# Resource Allocation Dashboard

This folder contains a read-only Flask dashboard for the C++ Resource Allocation System.

The web dashboard does not run allocation logic, edit CSV files, or modify the C++ backend. It only displays the current data files from the project `data/` folder.

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

## Pages

- `/` shows the selected strategy and quick data counts.
- `/users` shows `data/users.csv`.
- `/spaces` shows `data/spaces.csv`.
- `/requests` shows `data/requests.csv`.
- `/allocations` shows `data/allocations.csv` if it exists.
- `/results` shows `data/request_results.csv` if it exists.

If `allocations.csv` or `request_results.csv` is missing, the dashboard shows a friendly message instead of crashing.
