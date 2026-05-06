from pathlib import Path
import csv
import subprocess

from flask import Flask, render_template


PROJECT_ROOT = Path(__file__).resolve().parent.parent
DATA_DIR = PROJECT_ROOT / "data"

app = Flask(__name__)

EXECUTABLE_NAMES = [
    "allocation_system.exe",
    "allocation_system_test.exe",
    "allocation_system",
]


def read_config_strategy():
    config_path = DATA_DIR / "config.txt"

    if not config_path.exists():
        return "greedy (default / config missing)"

    with config_path.open("r", encoding="utf-8-sig", newline="") as file:
        for line in file:
            stripped = line.strip()
            if not stripped or stripped.startswith("#"):
                continue

            if "=" not in stripped:
                continue

            key, value = stripped.split("=", 1)
            if key.strip() == "allocation_strategy" and value.strip():
                return value.strip()

    return "greedy (default / config missing)"


def read_csv_table(filename):
    csv_path = DATA_DIR / filename

    if not csv_path.exists():
        return {
            "exists": False,
            "headers": [],
            "rows": [],
            "message": f"{filename} is not available yet.",
        }

    with csv_path.open("r", encoding="utf-8-sig", newline="") as file:
        reader = csv.DictReader(file)
        headers = reader.fieldnames or []
        rows = list(reader)

    return {
        "exists": True,
        "headers": headers,
        "rows": rows,
        "message": "",
    }


def count_optional_file(filename):
    table = read_csv_table(filename)
    if not table["exists"]:
        return "Not available"
    return len(table["rows"])


def truncate_output(text, limit=3000):
    if not text:
        return ""

    if len(text) <= limit:
        return text

    return text[:limit] + "\n... output truncated for dashboard display ..."


def find_backend_executable():
    for executable_name in EXECUTABLE_NAMES:
        executable_path = PROJECT_ROOT / executable_name
        if executable_path.exists() and executable_path.is_file():
            return executable_path

    return None


def build_dashboard_context(run_result=None):
    stats = [
        {"label": "Users", "value": len(read_csv_table("users.csv")["rows"])},
        {"label": "Spaces", "value": len(read_csv_table("spaces.csv")["rows"])},
        {"label": "Requests", "value": len(read_csv_table("requests.csv")["rows"])},
        {"label": "Allocations", "value": count_optional_file("allocations.csv")},
        {"label": "Request Results", "value": count_optional_file("request_results.csv")},
    ]

    return {
        "strategy": read_config_strategy(),
        "stats": stats,
        "run_result": run_result,
    }


def run_backend_allocation():
    executable_path = find_backend_executable()

    if executable_path is None:
        return {
            "status": "error",
            "message": "Backend executable not found. Please compile the C++ project first.",
            "stdout": "",
            "stderr": "",
        }

    try:
        completed = subprocess.run(
            [str(executable_path)],
            text=True,
            cwd=PROJECT_ROOT,
            capture_output=True,
            timeout=30,
        )
    except subprocess.TimeoutExpired as exc:
        return {
            "status": "error",
            "message": "Backend execution timed out after 30 seconds.",
            "stdout": truncate_output(exc.stdout or ""),
            "stderr": truncate_output(exc.stderr or ""),
        }

    if completed.returncode == 0:
        status = "success"
        message = "Allocation backend ran successfully. Results were updated."
    else:
        status = "error"
        message = f"Allocation backend failed with exit code {completed.returncode}."

    return {
        "status": status,
        "message": message,
        "stdout": truncate_output(completed.stdout),
        "stderr": truncate_output(completed.stderr),
    }


def render_csv_page(template_name, title, filename, empty_message):
    table = read_csv_table(filename)
    return render_template(
        template_name,
        title=title,
        table=table,
        empty_message=empty_message,
    )


@app.route("/")
def index():
    return render_template("index.html", **build_dashboard_context())


@app.post("/run-allocation")
def run_allocation():
    run_result = run_backend_allocation()
    return render_template("index.html", **build_dashboard_context(run_result))


@app.route("/users")
def users():
    return render_csv_page("users.html", "Users", "users.csv", "No users were found.")


@app.route("/spaces")
def spaces():
    return render_csv_page("spaces.html", "Spaces", "spaces.csv", "No spaces were found.")


@app.route("/requests")
def requests():
    return render_csv_page("requests.html", "Requests", "requests.csv", "No requests were found.")


@app.route("/allocations")
def allocations():
    return render_csv_page(
        "allocations.html",
        "Allocations",
        "allocations.csv",
        "No allocation results available yet. Run the C++ program first.",
    )


@app.route("/results")
def results():
    return render_csv_page(
        "results.html",
        "Request Results",
        "request_results.csv",
        "No request results available yet. Run the C++ program first.",
    )


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5000, debug=True)
