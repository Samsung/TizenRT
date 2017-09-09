#!/usr/bin/env python

import sys

from config import TIZENRT_ROOT
from config import CI_LINT_FILE_NAME

from utils import write_result
from utils import find_tizenrt_source_files

from linter import Linter


def run(targets, show_warning, is_ci, is_all):
    targets.append(TIZENRT_ROOT)
    file_list = find_tizenrt_source_files(targets)
    result = Linter().execute(file_list, show_warning)

    if is_ci:
        write_result(CI_LINT_FILE_NAME, result.message)

    print(result.message)
    exit(result.exitcode)


if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument('targets', metavar='Target', nargs='*',
                        help='Directory or files to execute lint')
    parser.add_argument(
        "--ci",
        dest="ci",
        required=False,
        action='store_true',
        help="True, if it is ci build.")
    parser.add_argument(
        "--warning",
        dest="show_warning",
        required=False,
        action='store_true',
        help="True, if you want to show warning.")
    parser.add_argument(
        "--all",
        dest="all",
        required=False,
        action='store_true',
        help="True, if you want to format all of source code in iotivity-rt.")
    args = parser.parse_args()
    run(args.targets, args.show_warning, args.ci, args.all)
