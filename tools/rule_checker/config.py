import os

# TizenRT
TIZENRT_ROOT = os.path.abspath(
    os.path.join(os.path.dirname(__file__), '..', '..'))

TIZENRT_ROOT_TOOLS = os.path.join(TIZENRT_ROOT, 'tools')

# Result file name for ci
CI_LINT_FILE_NAME = 'ci_lint.txt'

# Common list
EXCLUDE_PATH_LIST = []
