from pathlib import Path
import csv
import subprocess

from flask import Flask, redirect, render_template, request, url_for


PROJECT_ROOT = Path(__file__).resolve().parent.parent
DATA_DIR = PROJECT_ROOT / "data"

app = Flask(__name__)

EXECUTABLE_NAMES = [
    "allocation_system.exe",
    "allocation_system_test.exe",
    "allocation_system",
]

SUPPORTED_STRATEGIES = [
    {"value": "greedy", "label": "Greedy"},
    {"value": "priority", "label": "Priority"},
    {"value": "multi_room_exam_greedy", "label": "Multi-Room Exam Greedy"},
    {"value": "multi_room_exam_best_fit", "label": "Multi-Room Exam Best-Fit"},
]

REQUEST_HEADERS = [
    "requestId",
    "requestType",
    "userId",
    "spaceId",
    "participantCount",
    "requiredFeature",
    "requiredBuilding",
    "timeData",
    "title",
    "purpose",
    "courseCode",
    "courseName",
    "examType",
    "canSplitAcrossRooms",
]

EXAM_TYPES = ["Midterm", "Final", "Quiz", "General"]
FEATURE_OPTIONS = ["None", "Projector", "Whiteboard", "Computers"]
PURPOSE_OPTIONS = ["Meeting", "Lecture", "Lab", "Exam", "General", "Other"]
REQUEST_TYPE_OPTIONS = ["OneTime", "Recurring", "Exam"]
USER_ROLE_OPTIONS = [
    "All",
    "Student",
    "TeachingAssistant",
    "Instructor",
    "Staff",
    "Administrator",
]

DAY_NAMES = {
    1: "Monday",
    2: "Tuesday",
    3: "Wednesday",
    4: "Thursday",
    5: "Friday",
    6: "Saturday",
    7: "Sunday",
}


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


def write_config_strategy(strategy_name):
    config_path = DATA_DIR / "config.txt"
    config_path.write_text(
        f"allocation_strategy={strategy_name}\n",
        encoding="utf-8",
    )


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


def get_page_message():
    status = request.args.get("status", "")
    text = request.args.get("message", "")

    if status in {"success", "error"} and text:
        return {
            "status": status,
            "text": text,
        }

    return None


def get_request_headers():
    requests_path = DATA_DIR / "requests.csv"

    if not requests_path.exists() or requests_path.stat().st_size == 0:
        return REQUEST_HEADERS

    with requests_path.open("r", encoding="utf-8-sig", newline="") as file:
        reader = csv.reader(file)
        try:
            headers = next(reader)
        except StopIteration:
            return REQUEST_HEADERS

    return headers or REQUEST_HEADERS


def get_next_request_id():
    table = read_csv_table("requests.csv")
    max_request_id = 0

    for row in table["rows"]:
        try:
            request_id = int((row.get("requestId") or "").strip())
        except ValueError:
            continue

        max_request_id = max(max_request_id, request_id)

    return max_request_id + 1


def get_request_row(request_id):
    request_id = str(request_id)

    for row in read_csv_table("requests.csv")["rows"]:
        if (row.get("requestId") or "").strip() == request_id:
            return row

    return None


def get_building_options(spaces):
    buildings = {
        (space.get("building") or "").strip()
        for space in spaces
        if (space.get("building") or "").strip()
    }
    return ["None"] + sorted(buildings)


def normalize_optional_constraint(value):
    stripped = value.strip()
    return stripped if stripped else "None"


def display_value(value, fallback="N/A"):
    stripped = (value or "").strip()
    if not stripped or stripped == "None":
        return fallback
    return stripped


def build_user_specific_details(user):
    role = (user.get("role") or "").strip()
    detail_fields = []

    if role == "Student":
        detail_fields = [
            ("Student No", user.get("studentNo")),
            ("Program", user.get("program")),
            ("Year", user.get("yearLevel")),
        ]
    elif role == "TeachingAssistant":
        detail_fields = [
            ("Assistant Type", user.get("assistantType")),
            ("Office", user.get("officeRoom")),
        ]
    elif role == "Instructor":
        detail_fields = [
            ("Title", user.get("title")),
            ("Office", user.get("officeRoom")),
        ]
    elif role == "Staff":
        detail_fields = [
            ("Job Title", user.get("jobTitle")),
        ]
    elif role == "Administrator":
        detail_fields = [
            ("Admin Level", user.get("adminLevel")),
        ]

    details = [
        f"{label}: {value.strip()}"
        for label, value in detail_fields
        if (value or "").strip()
    ]

    return "; ".join(details) if details else "N/A"


def build_user_display_rows(rows):
    return [
        {
            "userId": display_value(row.get("userId")),
            "name": display_value(row.get("name")),
            "role": display_value(row.get("role")),
            "email": display_value(row.get("email")),
            "status": display_value(row.get("status")),
            "primaryUnit": display_value(row.get("primaryUnit")),
            "assignedRoles": display_value(row.get("assignedRoles")),
            "details": build_user_specific_details(row),
        }
        for row in rows
    ]


def format_day_name(value):
    try:
        day = int((value or "").strip())
    except ValueError:
        return display_value(value)

    return DAY_NAMES.get(day, display_value(value))


def format_time_part(value):
    text = (value or "").strip()
    if not text:
        return "N/A"

    if ":" not in text:
        try:
            hour = int(text)
        except ValueError:
            return text

        if 0 <= hour <= 24:
            return f"{hour:02d}:00"
        return text

    parts = text.split(":")
    if len(parts) != 2:
        return text

    try:
        hour = int(parts[0])
        minute = int(parts[1])
    except ValueError:
        return text

    if hour < 0 or hour > 24 or minute < 0 or minute > 59:
        return text

    if hour == 24 and minute != 0:
        return text

    return f"{hour:02d}:{minute:02d}"


def format_time_segment(segment):
    parts = [part.strip() for part in segment.split("-")]
    if len(parts) != 3:
        return None

    try:
        day = int(parts[0])
    except ValueError:
        return None

    day_name = DAY_NAMES.get(day)
    if not day_name:
        return None

    start_time = format_time_part(parts[1])
    end_time = format_time_part(parts[2])

    if start_time == "N/A" or end_time == "N/A":
        return None

    return f"{day_name} {start_time}\u2013{end_time}"


def format_time_data(time_data):
    raw_value = (time_data or "").strip()
    if not raw_value:
        return "N/A"

    formatted_segments = []

    for segment in raw_value.split(";"):
        stripped_segment = segment.strip()
        if not stripped_segment:
            return raw_value

        formatted_segment = format_time_segment(stripped_segment)
        if formatted_segment is None:
            return raw_value

        formatted_segments.append(formatted_segment)

    return "; ".join(formatted_segments)


def format_slot_display(day, start_time, end_time):
    day_name = format_day_name(day)
    start_display = format_time_part(start_time)
    end_display = format_time_part(end_time)
    return f"{day_name} {start_display}\u2013{end_display}"


def safe_int(value):
    try:
        return int((value or "").strip())
    except ValueError:
        return None


def parse_positive_int(form_data, field_name, label, errors):
    raw_value = form_data.get(field_name, "").strip()

    try:
        value = int(raw_value)
    except ValueError:
        errors.append(f"{label} must be a number.")
        return None

    if value <= 0:
        errors.append(f"{label} must be greater than zero.")
        return None

    return value


def parse_int_field(form_data, field_name, label, errors):
    raw_value = form_data.get(field_name, "").strip()

    try:
        return int(raw_value)
    except ValueError:
        errors.append(f"{label} must be a number.")
        return None


def parse_clock_time(form_data, time_field, hour_field, label, errors):
    raw_value = form_data.get(time_field, "").strip()
    if not raw_value:
        raw_value = form_data.get(hour_field, "").strip()

    if not raw_value:
        errors.append(f"{label} is required.")
        return None, ""

    if ":" in raw_value:
        parts = raw_value.split(":")
        if len(parts) != 2:
            errors.append(f"{label} must use HH:MM format.")
            return None, raw_value

        try:
            hour = int(parts[0])
            minute = int(parts[1])
        except ValueError:
            errors.append(f"{label} must use numeric HH:MM values.")
            return None, raw_value

        if hour < 0 or hour > 24 or minute < 0 or minute > 59:
            errors.append(f"{label} must be between 00:00 and 24:00.")
            return None, raw_value

        if hour == 24 and minute != 0:
            errors.append(f"{label} cannot be later than 24:00.")
            return None, raw_value

        return hour * 60 + minute, f"{hour:02d}:{minute:02d}"

    try:
        hour = int(raw_value)
    except ValueError:
        errors.append(f"{label} must be an hour or HH:MM value.")
        return None, raw_value

    if hour < 0 or hour > 24:
        errors.append(f"{label} hour must be between 0 and 24.")
        return None, raw_value

    return hour * 60, f"{hour:02d}:00"


def parse_time_fields(form_data, errors):
    day = parse_int_field(form_data, "day", "Day", errors)
    start_minutes, start_time = parse_clock_time(
        form_data, "startTime", "startHour", "Start time", errors
    )
    end_minutes, end_time = parse_clock_time(
        form_data, "endTime", "endHour", "End time", errors
    )

    if day is not None and not 1 <= day <= 7:
        errors.append("Day must be between 1 and 7.")

    if start_minutes is not None and start_minutes >= 24 * 60:
        errors.append("Start time must be earlier than 24:00.")

    if start_minutes is not None and end_minutes is not None and start_minutes >= end_minutes:
        errors.append("Start time must be earlier than end time.")

    return day, start_time, end_time


def validate_exam_form(form_data, users, spaces):
    errors = []
    user_ids = {(user.get("userId") or "").strip() for user in users}
    space_ids = {(space.get("spaceId") or "").strip() for space in spaces}

    user_id = form_data.get("userId", "").strip()
    space_id = form_data.get("spaceId", "").strip()
    participant_count = parse_positive_int(
        form_data, "participantCount", "Participant count", errors
    )
    day, start_time, end_time = parse_time_fields(form_data, errors)
    title = form_data.get("title", "").strip()
    course_code = form_data.get("courseCode", "").strip()
    course_name = form_data.get("courseName", "").strip()
    exam_type = form_data.get("examType", "").strip()
    can_split = form_data.get("canSplitAcrossRooms", "").strip()

    if user_id not in user_ids:
        errors.append("Selected requester does not exist.")

    if space_id not in space_ids:
        errors.append("Selected reference space does not exist.")

    if not title:
        errors.append("Title is required.")

    if not course_code:
        errors.append("Course code is required.")

    if not course_name:
        errors.append("Course name is required.")

    if exam_type not in EXAM_TYPES:
        errors.append("Exam type must be Midterm, Final, Quiz, or General.")

    if can_split not in {"true", "false"}:
        errors.append("Can split across rooms must be true or false.")

    if errors:
        return None, errors

    row = {
        "requestId": str(get_next_request_id()),
        "requestType": "Exam",
        "userId": user_id,
        "spaceId": space_id,
        "participantCount": str(participant_count),
        "requiredFeature": normalize_optional_constraint(
            form_data.get("requiredFeature", "")
        ),
        "requiredBuilding": normalize_optional_constraint(
            form_data.get("requiredBuilding", "")
        ),
        "timeData": f"{day}-{start_time}-{end_time}",
        "title": title,
        "purpose": "Exam",
        "courseCode": course_code,
        "courseName": course_name,
        "examType": exam_type,
        "canSplitAcrossRooms": can_split,
    }

    return row, []


def validate_one_time_form(form_data, users, spaces):
    errors = []
    user_ids = {(user.get("userId") or "").strip() for user in users}
    space_ids = {(space.get("spaceId") or "").strip() for space in spaces}

    user_id = form_data.get("userId", "").strip()
    space_id = form_data.get("spaceId", "").strip()
    participant_count = parse_positive_int(
        form_data, "participantCount", "Participant count", errors
    )
    day, start_time, end_time = parse_time_fields(form_data, errors)
    title = form_data.get("title", "").strip()
    purpose = form_data.get("purpose", "").strip()

    if user_id not in user_ids:
        errors.append("Selected requester does not exist.")

    if space_id not in space_ids:
        errors.append("Selected space does not exist.")

    if not title:
        errors.append("Title is required.")

    if not purpose:
        errors.append("Purpose is required.")
    elif purpose not in PURPOSE_OPTIONS:
        errors.append("Purpose must be Meeting, Lecture, Lab, General, or Other.")

    if errors:
        return None, errors

    row = {
        "requestId": str(get_next_request_id()),
        "requestType": "OneTime",
        "userId": user_id,
        "spaceId": space_id,
        "participantCount": str(participant_count),
        "requiredFeature": normalize_optional_constraint(
            form_data.get("requiredFeature", "")
        ),
        "requiredBuilding": normalize_optional_constraint(
            form_data.get("requiredBuilding", "")
        ),
        "timeData": f"{day}-{start_time}-{end_time}",
        "title": title,
        "purpose": purpose,
        "courseCode": "",
        "courseName": "",
        "examType": "",
        "canSplitAcrossRooms": "false",
    }

    return row, []


def validate_request_form(form_data, users, spaces):
    errors = []
    user_ids = {(user.get("userId") or "").strip() for user in users}
    space_ids = {(space.get("spaceId") or "").strip() for space in spaces}

    request_type = form_data.get("requestType", "").strip()
    user_id = form_data.get("userId", "").strip()
    space_id = form_data.get("spaceId", "").strip()
    participant_count = parse_positive_int(
        form_data, "participantCount", "Participant count", errors
    )
    title = form_data.get("title", "").strip()
    purpose = form_data.get("purpose", "").strip()
    time_data = ""

    if request_type not in REQUEST_TYPE_OPTIONS:
        errors.append("Request type must be OneTime, Recurring, or Exam.")

    if user_id not in user_ids:
        errors.append("Selected requester does not exist.")

    if space_id not in space_ids:
        errors.append("Selected space does not exist.")

    if not title:
        errors.append("Title is required.")

    if not purpose:
        errors.append("Purpose is required.")

    if request_type in {"OneTime", "Exam"}:
        day, start_time, end_time = parse_time_fields(form_data, errors)
        if day is not None and start_time and end_time:
            time_data = f"{day}-{start_time}-{end_time}"
    elif request_type == "Recurring":
        time_data = form_data.get("timeData", "").strip()
        if not time_data:
            errors.append("Recurring time data is required.")

    course_code = ""
    course_name = ""
    exam_type = ""
    can_split = "false"

    if request_type == "Exam":
        course_code = form_data.get("courseCode", "").strip()
        course_name = form_data.get("courseName", "").strip()
        exam_type = form_data.get("examType", "").strip()
        can_split = form_data.get("canSplitAcrossRooms", "").strip()

        if not course_code:
            errors.append("Course code is required for exam requests.")

        if not course_name:
            errors.append("Course name is required for exam requests.")

        if exam_type not in EXAM_TYPES:
            errors.append("Exam type must be Midterm, Final, Quiz, or General.")

        if can_split not in {"true", "false"}:
            errors.append("Can split across rooms must be true or false.")

    if errors:
        return None, errors

    row = {
        "requestId": str(get_next_request_id()),
        "requestType": request_type,
        "userId": user_id,
        "spaceId": space_id,
        "participantCount": str(participant_count),
        "requiredFeature": normalize_optional_constraint(
            form_data.get("requiredFeature", "")
        ),
        "requiredBuilding": normalize_optional_constraint(
            form_data.get("requiredBuilding", "")
        ),
        "timeData": time_data,
        "title": title,
        "purpose": purpose,
        "courseCode": course_code,
        "courseName": course_name,
        "examType": exam_type,
        "canSplitAcrossRooms": can_split,
    }

    return row, []


def append_request_row(row):
    requests_path = DATA_DIR / "requests.csv"
    headers = get_request_headers()
    write_header = not requests_path.exists() or requests_path.stat().st_size == 0
    needs_leading_newline = False

    if requests_path.exists() and requests_path.stat().st_size > 0:
        with requests_path.open("rb") as file:
            file.seek(-1, 2)
            needs_leading_newline = file.read(1) not in {b"\n", b"\r"}

    with requests_path.open("a", encoding="utf-8", newline="") as file:
        if needs_leading_newline:
            file.write("\n")

        writer = csv.DictWriter(file, fieldnames=headers, extrasaction="ignore")

        if write_header:
            writer.writeheader()

        writer.writerow({header: row.get(header, "") for header in headers})


def is_exam_request(row):
    request_type = (row.get("requestType") or "").strip()
    purpose = (row.get("purpose") or "").strip()
    return request_type == "Exam" or purpose == "Exam"


def index_by_field(rows, field_name):
    indexed = {}

    for row in rows:
        key = (row.get(field_name) or "").strip()
        if key:
            indexed[key] = row

    return indexed


def group_by_field(rows, field_name):
    grouped = {}

    for row in rows:
        key = (row.get(field_name) or "").strip()
        if key:
            grouped.setdefault(key, []).append(row)

    return grouped


def get_room_capacity(space):
    if not space:
        return None
    return safe_int(space.get("capacity"))


def format_time_display(value):
    return format_time_part(value)


def build_assigned_room(allocation, spaces_by_id):
    space_id = (allocation.get("spaceId") or "").strip()
    space = spaces_by_id.get(space_id, {})
    capacity = get_room_capacity(space)

    return {
        "space_id": space_id,
        "name": display_value(allocation.get("spaceName") or space.get("name")),
        "type": display_value(allocation.get("spaceType") or space.get("type")),
        "building": display_value(allocation.get("building") or space.get("building")),
        "capacity": capacity,
        "capacity_display": capacity if capacity is not None else "N/A",
        "assigned": safe_int(allocation.get("assignedParticipants")) or 0,
        "time": format_slot_display(
            allocation.get("day"),
            allocation.get("startHour"),
            allocation.get("endHour"),
        ),
    }


def build_allocation_summary():
    requests_table = read_csv_table("requests.csv")
    results_table = read_csv_table("request_results.csv")
    allocations_table = read_csv_table("allocations.csv")
    spaces_table = read_csv_table("spaces.csv")
    users_table = read_csv_table("users.csv")

    results_by_request = index_by_field(results_table["rows"], "requestId")
    allocations_by_request = group_by_field(allocations_table["rows"], "requestId")
    spaces_by_id = index_by_field(spaces_table["rows"], "spaceId")
    users_by_id = index_by_field(users_table["rows"], "userId")

    summaries = []

    for request_row in requests_table["rows"]:
        request_id = (request_row.get("requestId") or "").strip()
        result_row = results_by_request.get(request_id, {})
        user = users_by_id.get((request_row.get("userId") or "").strip(), {})
        participant_count = safe_int(request_row.get("participantCount"))
        request_type = display_value(
            result_row.get("requestType") or request_row.get("requestType"),
            "Unknown",
        )
        status = display_value(result_row.get("status"), "Pending")

        assigned_rooms = []
        if status.lower() != "rejected":
            assigned_rooms = [
                build_assigned_room(allocation, spaces_by_id)
                for allocation in allocations_by_request.get(request_id, [])
            ]

        total_assigned = sum(room["assigned"] for room in assigned_rooms)
        capacities = [room["capacity"] for room in assigned_rooms]
        total_capacity = None

        if assigned_rooms and all(capacity is not None for capacity in capacities):
            total_capacity = sum(capacities)

        waste = None
        if total_capacity is not None and participant_count is not None:
            waste = total_capacity - participant_count

        summaries.append(
            {
                "request_id": request_id,
                "request_type": request_type,
                "is_exam": request_type == "Exam" or is_exam_request(request_row),
                "title": display_value(
                    request_row.get("title") or result_row.get("title"),
                    "Untitled Request",
                ),
                "purpose": display_value(
                    request_row.get("purpose") or result_row.get("purpose")
                ),
                "course_code": display_value(
                    request_row.get("courseCode") or result_row.get("courseCode")
                ),
                "course_name": display_value(
                    request_row.get("courseName") or result_row.get("courseName")
                ),
                "exam_type": display_value(
                    request_row.get("examType") or result_row.get("examType")
                ),
                "participant_count": participant_count,
                "participant_display": (
                    participant_count if participant_count is not None else "N/A"
                ),
                "can_split": display_value(
                    request_row.get("canSplitAcrossRooms")
                    or result_row.get("canSplitAcrossRooms"),
                    "false",
                ),
                "time_data": format_time_data(
                    request_row.get("timeData") or result_row.get("timeInfo")
                ),
                "user_id": display_value(request_row.get("userId")),
                "requester_name": display_value(user.get("name")),
                "status": status,
                "status_class": status.lower(),
                "rejection_reason": display_value(
                    result_row.get("rejectionReason"),
                    "",
                ),
                "assigned_rooms": assigned_rooms,
                "total_assigned": total_assigned,
                "assignment_display": (
                    f"{total_assigned} / {participant_count}"
                    if participant_count is not None
                    else str(total_assigned)
                ),
                "total_capacity": (
                    total_capacity if total_capacity is not None else "N/A"
                ),
                "waste": waste if waste is not None else "N/A",
            }
        )

    return {
        "summaries": summaries,
        "strategy": read_config_strategy(),
        "requests_available": requests_table["exists"],
        "results_available": results_table["exists"],
        "allocations_available": allocations_table["exists"],
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


def build_dashboard_context(run_result=None, strategy_message=None):
    stats = [
        {"label": "Users", "value": len(read_csv_table("users.csv")["rows"])},
        {"label": "Spaces", "value": len(read_csv_table("spaces.csv")["rows"])},
        {"label": "Requests", "value": len(read_csv_table("requests.csv")["rows"])},
        {"label": "Allocations", "value": count_optional_file("allocations.csv")},
        {"label": "Request Results", "value": count_optional_file("request_results.csv")},
    ]

    return {
        "strategy": read_config_strategy(),
        "strategies": SUPPORTED_STRATEGIES,
        "stats": stats,
        "run_result": run_result,
        "strategy_message": strategy_message,
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


def format_table_for_display(filename, table):
    display_table = {
        "exists": table["exists"],
        "headers": list(table["headers"]),
        "rows": [],
        "message": table["message"],
    }

    for row in table["rows"]:
        display_row = dict(row)

        if filename == "requests.csv" and "timeData" in display_row:
            display_row["timeData"] = format_time_data(display_row.get("timeData"))

        if filename == "request_results.csv":
            if "timeData" in display_row:
                display_row["timeData"] = format_time_data(display_row.get("timeData"))
            if "timeInfo" in display_row:
                display_row["timeInfo"] = format_time_data(display_row.get("timeInfo"))

        if filename == "allocations.csv":
            if "day" in display_row:
                display_row["day"] = format_day_name(display_row.get("day"))
            if "startHour" in display_row:
                display_row["startHour"] = format_time_part(display_row.get("startHour"))
            if "endHour" in display_row:
                display_row["endHour"] = format_time_part(display_row.get("endHour"))

        display_table["rows"].append(display_row)

    return display_table


def render_csv_page(template_name, title, filename, empty_message):
    table = format_table_for_display(filename, read_csv_table(filename))
    return render_template(
        template_name,
        title=title,
        table=table,
        empty_message=empty_message,
        page_message=get_page_message(),
    )


@app.route("/")
def index():
    strategy_message = None
    message_status = request.args.get("strategy_status", "")
    message_text = request.args.get("strategy_message", "")

    if message_status in {"success", "error"} and message_text:
        strategy_message = {
            "status": message_status,
            "text": message_text,
        }

    return render_template(
        "index.html",
        **build_dashboard_context(strategy_message=strategy_message),
    )


@app.post("/run-allocation")
def run_allocation():
    run_result = run_backend_allocation()
    return render_template("index.html", **build_dashboard_context(run_result))


@app.post("/set-strategy")
def set_strategy():
    selected_strategy = request.form.get("strategy", "").strip()
    strategy_labels = {
        strategy["value"]: strategy["label"] for strategy in SUPPORTED_STRATEGIES
    }

    if selected_strategy not in strategy_labels:
        return redirect(
            url_for(
                "index",
                strategy_status="error",
                strategy_message="Invalid strategy selected. Config was not changed.",
            )
        )

    write_config_strategy(selected_strategy)
    return redirect(
        url_for(
            "index",
            strategy_status="success",
            strategy_message=f"Strategy saved: {strategy_labels[selected_strategy]}",
        )
    )


@app.route("/add-request", methods=["GET", "POST"])
def add_request():
    users_table = read_csv_table("users.csv")
    spaces_table = read_csv_table("spaces.csv")
    users = users_table["rows"]
    spaces = spaces_table["rows"]
    form_data = request.form.to_dict() if request.method == "POST" else {
        "requestType": request.args.get("type", "OneTime"),
    }
    errors = []

    if request.method == "POST":
        row, errors = validate_request_form(form_data, users, spaces)

        if not errors:
            append_request_row(row)
            return redirect(url_for("request_added", request_id=row["requestId"]))

    return render_template(
        "add_request.html",
        users=users,
        spaces=spaces,
        request_types=REQUEST_TYPE_OPTIONS,
        purpose_options=PURPOSE_OPTIONS,
        exam_types=EXAM_TYPES,
        feature_options=FEATURE_OPTIONS,
        building_options=get_building_options(spaces),
        form_data=form_data,
        errors=errors,
    )


@app.route("/request-added/<request_id>")
def request_added(request_id):
    row = get_request_row(request_id)
    display_row = dict(row) if row else None

    if display_row and "timeData" in display_row:
        display_row["timeData"] = format_time_data(display_row.get("timeData"))

    return render_template(
        "request_added.html",
        request_id=request_id,
        request_row=display_row,
    )


@app.post("/run-allocation-now/<request_id>")
def run_allocation_now(request_id):
    run_result = run_backend_allocation()

    return redirect(
        url_for(
            "allocation_summary",
            highlight=request_id,
            run_status=run_result["status"],
            run_message=run_result["message"],
        )
    )


@app.route("/add-exam", methods=["GET", "POST"])
def add_exam():
    return redirect(url_for("add_request", type="Exam"))


@app.route("/add-one-time", methods=["GET", "POST"])
def add_one_time():
    return redirect(url_for("add_request", type="OneTime"))


@app.route("/allocation-summary")
def allocation_summary():
    run_message = None
    run_status = request.args.get("run_status", "")
    message_text = request.args.get("run_message", "")

    if run_status in {"success", "error"} and message_text:
        run_message = {
            "status": run_status,
            "text": message_text,
        }

    return render_template(
        "allocation_summary.html",
        **build_allocation_summary(),
        highlight_id=request.args.get("highlight", ""),
        run_message=run_message,
    )


@app.route("/exam-summary")
def exam_summary():
    return redirect(url_for("allocation_summary"))


@app.route("/users")
def users():
    table = read_csv_table("users.csv")
    selected_role = request.args.get("role", "All").strip() or "All"

    if selected_role not in USER_ROLE_OPTIONS:
        selected_role = "All"

    rows = table["rows"]
    if selected_role != "All":
        rows = [
            row for row in rows
            if (row.get("role") or "").strip() == selected_role
        ]

    return render_template(
        "users.html",
        title="Users",
        table=table,
        users=build_user_display_rows(rows),
        role_options=USER_ROLE_OPTIONS,
        selected_role=selected_role,
        empty_message="No users were found.",
        page_message=get_page_message(),
    )


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
