#!/usr/bin/env python3
###########################################################################
#
# Copyright 2026 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################
"""
TizenRT License Manager
Manage and verify Samsung Electronics Apache 2.0 license headers in source files.
"""

import argparse
import fnmatch
import os
import re
import subprocess
import sys
from datetime import datetime
from enum import Enum
from typing import Generator, List, NamedTuple, Optional, Tuple


class CommentStyle(Enum):
    C = "c"          # /* ... */
    HASH = "hash"    # # ...
    BATCH = "batch"  # REM ...


class Status(Enum):
    OK = "OK"
    YEAR_MISMATCH = "YEAR_MISMATCH"
    MISSING = "MISSING"
    OTHER_LICENSE = "OTHER_LICENSE"


class Action(Enum):
    CHECK = "check"
    ADD = "add"
    FIX = "fix"


class FileResult(NamedTuple):
    file_path: str
    status: Status
    detected_year: Optional[int]
    expected_year: int
    modified: bool
    message: str


EXT_MAP = {
    # C/C++ family
    ".c": CommentStyle.C,
    ".h": CommentStyle.C,
    ".cpp": CommentStyle.C,
    ".cxx": CommentStyle.C,
    ".cc": CommentStyle.C,
    ".hpp": CommentStyle.C,
    ".hxx": CommentStyle.C,
    # Script / Build family
    ".sh": CommentStyle.HASH,
    ".bash": CommentStyle.HASH,
    ".py": CommentStyle.HASH,
    ".mk": CommentStyle.HASH,
    ".cmake": CommentStyle.HASH,
    # Batch family
    ".bat": CommentStyle.BATCH,
    ".cmd": CommentStyle.BATCH,
}

EXACT_NAME_MAP = {
    "Makefile": CommentStyle.HASH,
    "Make.defs": CommentStyle.HASH,
    "Kconfig": CommentStyle.HASH,
    "CMakeLists.txt": CommentStyle.HASH,
}

DEFAULT_EXCLUDES = [
    ".git",
    "build/output",
    "*.swp",
    "*~",
    "*.pyc",
    "__pycache__",
]

SAMSUNG_COPYRIGHT_REGEX = re.compile(
    r"Copyright\s+(?:\(c\)\s*)?(\d{4})(?:-\d{4})?\s+Samsung\s+Electronics",
    re.IGNORECASE
)

APACHE_LICENSE_KEYWORD = "Licensed under the Apache License, Version 2.0"

OTHER_LICENSE_INDICATORS = [
    "copyright (c)",
    "copyright (c",
    "all rights reserved",
    "redistribution and use in source and binary forms",
    "gpl",
    "bsd",
    "mit license",
    "apache license",
]

# Similarity threshold for tracing file moves (renames) in git history.
# 100% only traces pure `git mv` moves, but many real moves also include small
# edits (e.g. R099). A moderate threshold still traces such moves while
# avoiding false matches between small boilerplate/skeleton files.
RENAME_SIMILARITY_THRESHOLD = 80


def get_comment_style(file_name_or_path: str) -> Optional[CommentStyle]:
    base_name = os.path.basename(file_name_or_path)
    if base_name in EXACT_NAME_MAP:
        return EXACT_NAME_MAP[base_name]

    _, ext = os.path.splitext(base_name)
    return EXT_MAP.get(ext.lower())


def generate_license_header(style: CommentStyle, year: int) -> str:
    if style == CommentStyle.C:
        return f"""/****************************************************************************
 *
 * Copyright {year} Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/"""
    elif style == CommentStyle.HASH:
        return f"""###########################################################################
#
# Copyright {year} Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################"""
    elif style == CommentStyle.BATCH:
        return f"""REM
REM
REM Copyright {year} Samsung Electronics All Rights Reserved.
REM
REM Licensed under the Apache License, Version 2.0 (the "License");
REM you may not use this file except in compliance with the License.
REM You may obtain a copy of the License at
REM
REM http://www.apache.org/licenses/LICENSE-2.0
REM
REM Unless required by applicable law or agreed to in writing,
REM software distributed under the License is distributed on an
REM "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
REM either express or implied. See the License for the specific
REM language governing permissions and limitations under the License.
REM
REM"""
    raise ValueError(f"Unsupported comment style: {style}")


def get_creation_year(file_path: str, fallback_year: Optional[int] = None) -> int:
    if fallback_year is not None:
        return fallback_year

    # Try git log to find file creation year
    abs_path = os.path.abspath(file_path)
    file_dir = os.path.dirname(abs_path)
    rename_opt = f"--find-renames={RENAME_SIMILARITY_THRESHOLD}%"

    # 1. First, check when the file was originally added (--diff-filter=A).
    # --follow traces the history back through renames/moves so that the
    # creation year is the ORIGINAL file's year, not the year it was moved.
    # --find-renames limits tracing to sufficiently similar renames to avoid
    # falsely tracing small boilerplate/skeleton files to unrelated older files.
    try:
        cmd_add = ["git", "log", "--follow", rename_opt, "--diff-filter=A",
                   "--format=%ad", "--date=format:%Y", "--", abs_path]
        res = subprocess.run(cmd_add, cwd=file_dir, capture_output=True, text=True, check=False)
        if res.returncode == 0 and res.stdout.strip():
            years = [y.strip() for y in res.stdout.strip().splitlines() if y.strip().isdigit()]
            if years:
                return int(years[-1])
    except Exception:
        pass

    # 2. If not found (e.g. files committed in the initial root commit), check oldest commit on this path
    try:
        cmd_log = ["git", "log", "--follow", rename_opt,
                   "--format=%ad", "--date=format:%Y", "--", abs_path]
        res = subprocess.run(cmd_log, cwd=file_dir, capture_output=True, text=True, check=False)
        if res.returncode == 0 and res.stdout.strip():
            years = [y.strip() for y in res.stdout.strip().splitlines() if y.strip().isdigit()]
            if years:
                return int(years[-1])
    except Exception:
        pass

    # 3. Fallback to file creation/modification time
    try:
        stat = os.stat(abs_path)
        mtime = getattr(stat, "st_birthtime", None) or stat.st_mtime
        return datetime.fromtimestamp(mtime).year
    except Exception:
        return datetime.now().year


class CheckResult(NamedTuple):
    status: Status
    detected_year: Optional[int]
    expected_year: int
    message: str


def check_content_license(content: str, style: CommentStyle, expected_year: int) -> CheckResult:
    # Scan the top part of the file (first 4000 chars / ~60 lines)
    header_portion = content[:4000]

    match = SAMSUNG_COPYRIGHT_REGEX.search(header_portion)
    if match:
        detected_year = int(match.group(1))
        if detected_year != expected_year:
            return CheckResult(
                Status.YEAR_MISMATCH,
                detected_year,
                expected_year,
                f"Year mismatch: found {detected_year}, expected {expected_year}"
            )
        return CheckResult(
            Status.OK,
            detected_year,
            expected_year,
            "Valid Samsung Electronics license"
        )

    # If Samsung license is missing, check if third-party license is present for information
    lower_header = header_portion.lower()
    has_other = any(ind in lower_header for ind in OTHER_LICENSE_INDICATORS)
    msg = "Samsung Electronics license missing (third-party license present)" if has_other else "Samsung Electronics license missing"

    return CheckResult(
        Status.MISSING,
        None,
        expected_year,
        msg
    )


def is_comment_start(text: str, style: CommentStyle) -> bool:
    stripped = text.lstrip()
    if style == CommentStyle.C:
        return stripped.startswith(("/*", "//"))
    elif style == CommentStyle.HASH:
        return stripped.startswith("#")
    elif style == CommentStyle.BATCH:
        return stripped.lower().startswith("rem")
    return False


def add_license_to_content(content: str, style: CommentStyle, year: int) -> Tuple[str, bool]:
    header = generate_license_header(style, year)
    lines = content.splitlines(keepends=True)

    if not lines:
        return header + "\n", True

    insert_idx = 0
    # Preserve Shebang
    if lines[0].startswith("#!"):
        insert_idx = 1
        # Also check for encoding declaration on line 2
        if len(lines) > 1 and re.match(r"^#.*coding[:=]\s*([-\w.]+)", lines[1]):
            insert_idx = 2
    elif lines[0].strip().lower().startswith(("@echo", "echo off")):
        insert_idx = 1

    suffix = "".join(lines[insert_idx:]).lstrip("\r\n")

    # If following content starts with a comment (e.g. existing third-party license or block comment),
    # join directly with a single newline (no blank line) matching TizenRT codebase style.
    # Otherwise, separate license header from code with a blank line.
    sep = "\n" if (suffix and is_comment_start(suffix, style)) else "\n\n"

    if insert_idx == 0:
        new_content = header + sep + suffix
        return new_content, True
    else:
        prefix = "".join(lines[:insert_idx])
        if not prefix.endswith("\n"):
            prefix += "\n"
        new_content = prefix + header + sep + suffix
        return new_content, True


def fix_license_content(content: str, style: CommentStyle, expected_year: int) -> Tuple[str, bool]:
    res = check_content_license(content, style, expected_year)
    if res.status == Status.OK:
        return content, False

    if res.status == Status.YEAR_MISMATCH:
        # Replace the year in Samsung copyright line
        def replace_year(match):
            original = match.group(0)
            return re.sub(r"\d{4}(-\d{4})?", str(expected_year), original, count=1)

        new_content, count = SAMSUNG_COPYRIGHT_REGEX.subn(replace_year, content, count=1)
        return new_content, (count > 0)

    if res.status == Status.MISSING:
        return add_license_to_content(content, style, expected_year)

    return content, False


def process_file(
    file_path: str,
    action: Action,
    year: Optional[int] = None,
    dry_run: bool = False
) -> FileResult:
    abs_path = os.path.abspath(file_path)
    style = get_comment_style(abs_path)
    if not style:
        return FileResult(
            file_path,
            Status.MISSING,
            None,
            datetime.now().year,
            False,
            "Unsupported file type"
        )

    expected_year = get_creation_year(abs_path, fallback_year=year)

    try:
        with open(abs_path, "r", encoding="utf-8", errors="replace") as f:
            content = f.read()
    except Exception as e:
        return FileResult(
            file_path,
            Status.MISSING,
            None,
            expected_year,
            False,
            f"Error reading file: {e}"
        )

    check_res = check_content_license(content, style, expected_year)
    modified = False

    if action == Action.CHECK:
        return FileResult(
            file_path,
            check_res.status,
            check_res.detected_year,
            expected_year,
            False,
            check_res.message
        )

    if action == Action.ADD:
        if check_res.status == Status.MISSING:
            new_content, changed = add_license_to_content(content, style, expected_year)
            if changed:
                modified = True
                if not dry_run:
                    with open(abs_path, "w", encoding="utf-8") as f:
                        f.write(new_content)
            return FileResult(
                file_path,
                check_res.status,
                check_res.detected_year,
                expected_year,
                modified,
                "License added" if not dry_run else "License would be added (dry-run)"
            )
        else:
            return FileResult(
                file_path,
                check_res.status,
                check_res.detected_year,
                expected_year,
                False,
                f"Skipped add: status is {check_res.status.value}"
            )

    if action == Action.FIX:
        if check_res.status in (Status.YEAR_MISMATCH, Status.MISSING):
            new_content, changed = fix_license_content(content, style, expected_year)
            if changed:
                modified = True
                if not dry_run:
                    with open(abs_path, "w", encoding="utf-8") as f:
                        f.write(new_content)
            return FileResult(
                file_path,
                check_res.status,
                check_res.detected_year,
                expected_year,
                modified,
                "License fixed" if not dry_run else "License would be fixed (dry-run)"
            )
        else:
            return FileResult(
                file_path,
                check_res.status,
                check_res.detected_year,
                expected_year,
                False,
                f"No fix needed: status is {check_res.status.value}"
            )

    return FileResult(file_path, check_res.status, check_res.detected_year, expected_year, False, "")


def is_excluded(path: str, exclude_patterns: List[str]) -> bool:
    normalized = path.replace("\\", "/")
    parts = normalized.split("/")
    for pat in exclude_patterns:
        pat_clean = pat.rstrip("/")
        if any(fnmatch.fnmatch(part, pat_clean) for part in parts):
            return True
        if fnmatch.fnmatch(normalized, pat_clean) or fnmatch.fnmatch(os.path.basename(normalized), pat_clean):
            return True
    return False


def find_target_files(
    paths: List[str],
    exclude_patterns: Optional[List[str]] = None
) -> Generator[str, None, None]:
    excludes = list(DEFAULT_EXCLUDES)
    if exclude_patterns:
        excludes.extend(exclude_patterns)

    for p in paths:
        abs_p = os.path.abspath(p)
        if not os.path.exists(abs_p):
            continue

        if os.path.isfile(abs_p):
            if not is_excluded(abs_p, excludes) and get_comment_style(abs_p) is not None:
                yield p
        elif os.path.isdir(abs_p):
            for root, dirs, files in os.walk(abs_p):
                # Prune excluded directories
                dirs[:] = [d for d in dirs if not is_excluded(os.path.join(root, d), excludes)]
                for file_name in files:
                    file_full = os.path.join(root, file_name)
                    if not is_excluded(file_full, excludes) and get_comment_style(file_name) is not None:
                        yield file_full


def parse_args(argv: Optional[List[str]] = None):
    if argv is None:
        argv = sys.argv[1:]

    # Normalize argv to support hybrid subcommand & flag styles
    # Valid subcommands
    subcmds = {"check", "add", "fix"}
    first_subcmd = None
    for arg in argv:
        if arg in subcmds:
            first_subcmd = arg
            break
        if not arg.startswith("-"):
            break

    # If user used flags like -a, -f, -c instead of subcommand
    action_flag = None
    cleaned_argv = []
    for arg in argv:
        if arg in ("-c", "--check"):
            action_flag = "check"
        elif arg in ("-a", "--add"):
            action_flag = "add"
        elif arg in ("-f", "--fix"):
            action_flag = "fix"
        else:
            cleaned_argv.append(arg)

    show_top_help = any(h in argv for h in ("-h", "--help")) and not any(sc in argv for sc in subcmds)
    if not first_subcmd and not show_top_help:
        # User didn't specify a subcommand
        chosen_action = action_flag or "check"
        normalized_argv = [chosen_action] + cleaned_argv
    else:
        normalized_argv = argv

    parser = argparse.ArgumentParser(
        prog="license_manager.py",
        description="TizenRT License Header Management Tool",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Check a single file or directory
  ./tools/license/license_manager.py check os/compression/compress.c
  ./tools/license/license_manager.py check apps/

  # Check using flags
  ./tools/license/license_manager.py -c apps/

  # Add license to files missing it
  ./tools/license/license_manager.py add os/new_file.c
  ./tools/license/license_manager.py add --year 2024 apps/my_app/

  # Fix mismatched years or insert missing licenses
  ./tools/license/license_manager.py fix os/

  # Dry-run check for changes without modifying files
  ./tools/license/license_manager.py fix --dry-run os/
        """
    )

    subparsers = parser.add_subparsers(dest="command", help="Command to execute")

    common_parser = argparse.ArgumentParser(add_help=False)
    common_parser.add_argument("paths", nargs="+", help="Target files or directories (relative or absolute)")
    common_parser.add_argument("-y", "--year", type=int, default=None, help="Force specific creation year")
    common_parser.add_argument("-d", "--dry-run", action="store_true", help="Simulate changes without writing to disk")
    common_parser.add_argument("-e", "--exclude", action="append", default=[], help="Exclude pattern (can be repeated)")
    common_parser.add_argument("-v", "--verbose", action="store_true", help="Show all files including OK")
    common_parser.add_argument("-q", "--quiet", action="store_true", help="Show summary only")

    subparsers.add_parser("check", parents=[common_parser], help="Check license headers")
    subparsers.add_parser("add", parents=[common_parser], help="Add license headers")
    subparsers.add_parser("fix", parents=[common_parser], help="Fix license headers")

    return parser.parse_args(normalized_argv)


def main(argv: Optional[List[str]] = None) -> int:
    if argv is None:
        raw_args = sys.argv[1:]
    else:
        raw_args = argv

    if not raw_args or raw_args in (["-h"], ["--help"]):
        parse_args(["--help"])
        return 0

    try:
        args = parse_args(raw_args)
    except SystemExit as e:
        return e.code

    action_map = {
        "check": Action.CHECK,
        "add": Action.ADD,
        "fix": Action.FIX,
    }
    action = action_map.get(args.command, Action.CHECK)

    if not args.paths:
        print("Error: No target paths specified.", file=sys.stderr)
        return 1

    targets = list(find_target_files(args.paths, args.exclude))
    if not targets:
        if not args.quiet:
            print("No matching target source files found.")
        return 0

    total_count = 0
    ok_count = 0
    mismatch_count = 0
    missing_count = 0
    other_count = 0
    modified_count = 0

    for path in targets:
        res = process_file(path, action=action, year=args.year, dry_run=args.dry_run)
        total_count += 1

        if res.status == Status.OK:
            ok_count += 1
            if args.verbose and not args.quiet:
                print(f"[OK] {path} (Year: {res.detected_year})")
        elif res.status == Status.YEAR_MISMATCH:
            mismatch_count += 1
            if not args.quiet:
                mod_str = " (MODIFIED)" if res.modified else ""
                print(f"[MISMATCH] {path}: found {res.detected_year}, expected {res.expected_year}{mod_str}")
        elif res.status == Status.MISSING:
            missing_count += 1
            if not args.quiet:
                mod_str = " (ADDED)" if res.modified else ""
                print(f"[MISSING] {path}{mod_str}")
        elif res.status == Status.OTHER_LICENSE:
            other_count += 1
            if not args.quiet:
                print(f"[OTHER] {path}: {res.message}")

        if res.modified:
            modified_count += 1

    # Print summary
    if not args.quiet:
        print("\n" + "=" * 50)
        print(f"Action        : {action.value.upper()}{' (DRY-RUN)' if args.dry_run else ''}")
        print(f"Total Files   : {total_count}")
        print(f"OK            : {ok_count}")
        print(f"Year Mismatch : {mismatch_count}")
        print(f"Missing       : {missing_count}")
        print(f"Third-Party   : {other_count}")
        if action in (Action.ADD, Action.FIX):
            print(f"Modified      : {modified_count}")
        print("=" * 50)

    # Return exit code: 1 if errors found in check mode, 0 otherwise
    if action == Action.CHECK:
        if mismatch_count > 0 or missing_count > 0:
            return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())
