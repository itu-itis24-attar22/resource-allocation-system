from pathlib import Path
import csv

from flask import Flask, render_template


PROJECT_ROOT = Path(__file__).resolve().parent.parent
DATA_DIR = PROJECT_ROOT / "data"

app = Flask(__name__)


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
    stats = [
        {"label": "Users", "value": len(read_csv_table("users.csv")["rows"])},
        {"label": "Spaces", "value": len(read_csv_table("spaces.csv")["rows"])},
        {"label": "Requests", "value": len(read_csv_table("requests.csv")["rows"])},
        {"label": "Allocations", "value": count_optional_file("allocations.csv")},
        {"label": "Request Results", "value": count_optional_file("request_results.csv")},
    ]

    return render_template(
        "index.html",
        strategy=read_config_strategy(),
        stats=stats,
    )


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
