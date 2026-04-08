import os
import sys
import time
import select
from subprocess import PIPE, Popen


class CustomError(Exception):
    def __init__(self, message: str):
        self.message = f"\033[91;1;4m{message}\033[0m"
        super().__init__(self.message)


PROMPTS = (">> ", ">>", "» ", "»")


def strip_prompt_prefix(line: str) -> str:
    for prompt in PROMPTS:
        if line.startswith(prompt):
            return line[len(prompt):]
    return line


def normalize_output_line(line: str) -> str:
    return strip_prompt_prefix(line)


def line_diff_error_message(output_file: str, line_number: int, line: str, expected_line: str) -> str:
    return (
        f"\033[91mError at line {line_number} in file '{output_file}':\n"
        f"\toutput is \033[90m'{line}'\033[91m but expected: "
        f"\033[90m'{expected_line}'\033[0m"
    )


def line_diff(line: str, expected_line: str, context: dict) -> bool:
    normalized_line = normalize_output_line(line)
    normalized_expected = normalize_output_line(expected_line)

    if normalized_line == normalized_expected:
        return True

    print(
        line_diff_error_message(
            context["output_file"], context["line_number"], line, expected_line
        ),
        file=sys.stderr,
    )
    return False


def check_output(output_file_name: str, expected_output_file_name: str) -> float:
    with open(output_file_name, "r", encoding="utf-8") as output_file:
        output = output_file.read().splitlines()
    with open(expected_output_file_name, "r", encoding="utf-8") as expected_output_file:
        expected_output = expected_output_file.read().splitlines()

    sum_matches = 0

    for index, expected_line in enumerate(expected_output):
        actual_line = output[index] if index < len(output) else ""
        if line_diff(
            actual_line,
            expected_line,
            {"output_file": output_file_name, "line_number": index + 1},
        ):
            sum_matches += 1

    if len(output) != len(expected_output):
        print(
            f"\033[91mLine count mismatch in '{output_file_name}': "
            f"got {len(output)}, expected {len(expected_output)}\033[0m",
            file=sys.stderr,
        )

    if not expected_output:
        return 1.0 if not output else 0.0

    return sum_matches / len(expected_output)


def resolve_executable(executable: str) -> str:
    if os.path.isabs(executable):
        return executable
    return os.path.abspath(executable)


def try_read_optional_prompt(process, timeout: float = 0.2) -> str:
    """
    Try to consume an optional prompt from stdout.
    Accepts:
      - '>> '
      - '>>'
      - '» '
      - '»'
      - no prompt at all
    Returns the consumed prompt, or '' if none was present.
    """
    fd = process.stdout.fileno()
    ready, _, _ = select.select([fd], [], [], timeout)
    if not ready:
        return ""

    data = os.read(fd, 3)
    if not data:
        return ""

    text = data.decode("utf-8", errors="replace")

    for prompt in sorted(PROMPTS, key=len, reverse=True):
        if text.startswith(prompt):
            extra = text[len(prompt):]
            if extra:
                process._checker_buffer = extra + getattr(process, "_checker_buffer", "")
            return prompt

    process._checker_buffer = text + getattr(process, "_checker_buffer", "")
    return ""


def buffered_readline(process, timeout: float = 5.0) -> str:
    deadline = time.time() + timeout
    buffer = getattr(process, "_checker_buffer", "")
    process._checker_buffer = ""

    while True:
        if "\n" in buffer:
            line, rest = buffer.split("\n", 1)
            process._checker_buffer = rest
            return line + "\n"

        if time.time() > deadline:
            if buffer:
                return buffer
            return ""

        fd = process.stdout.fileno()
        remaining = max(0.0, deadline - time.time())
        ready, _, _ = select.select([fd], [], [], remaining)
        if not ready:
            continue

        chunk = os.read(fd, 1024)
        if not chunk:
            if buffer:
                return buffer
            return ""

        buffer += chunk.decode("utf-8", errors="replace")


def run(executable: str, input_file_name: str, output_file_name: str, expected_output_file_name: str) -> float:
    with open(input_file_name, "r", encoding="utf-8") as input_file, open(
        output_file_name, "w", encoding="utf-8"
    ) as output_file:
        process = Popen(
            [resolve_executable(executable)],
            stdout=PIPE,
            stdin=PIPE,
            text=False,
        )
        process._checker_buffer = ""

        for line_number, raw_line in enumerate(input_file, start=1):
            try_read_optional_prompt(process)

            command = raw_line.rstrip("\n")
            process.stdin.write((command + "\n").encode("utf-8"))
            process.stdin.flush()

            if command == "Exit":
                break

            output_line = buffered_readline(process, timeout=5)
            if output_line == "":
                raise CustomError(
                    f"Process terminated before producing output for line {line_number}"
                )

            output_file.write(output_line)

        process.stdin.close()
        process.stdout.close()
        process.wait(timeout=5)

    return check_output(output_file_name, expected_output_file_name)


if __name__ == "__main__":
    if len(sys.argv) != 5:
        print("Usage: python3 checker.py <executable> <input_file> <output_file> <expected_output_file>")
        sys.exit(1)

    grade = run(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
    print(grade)
