import os
import subprocess
from config import TIZENRT_ROOT_TOOLS
from config import TIZENRT_ROOT
from common import Result
from utils import execute_return_output
from utils import print_fail, print_green

TIZEN_LINT_TOOL = os.path.join(
    TIZENRT_ROOT_TOOLS, 'rule_checker',
    'checkpatch_tizen.pl')


class Linter:
    def __init__(self):
        pass

    def execute(self, file_list, show_warning):
        warning_count = {'error': 0, 'warning': 0}
        for filepath in file_list:
            if not filepath.endswith('.c') and not filepath.endswith('.h'):
                continue
            each_result = self.execute_lint(filepath, show_warning)
            warning_count['error'] += each_result['error']
            warning_count['warning'] += each_result['warning']
        report = 'error: {}, warning: {}'.format(
            warning_count['error'], warning_count['warning'])
        exitcode = 0
        if warning_count['error'] > 0:
            exitcode = 1
            print_fail('Please Check Coding Rule!')

        return Result(message=report, exitcode=exitcode)

    def execute_lint(self, fullpath, show_warning):
        cmd = '{} {} 2> /dev/null'.format(TIZEN_LINT_TOOL, fullpath)

        lint_result = {'error': 0, 'warning': 0}
        print_green('$ ' + fullpath)
        result = execute_return_output(cmd, is_print_cmd=False)

        for line in result.data.split('\n'):
            if 'ERROR: ' in line:
                print(line.rstrip())
            if show_warning and 'WARNING: ' in line:
                print(line.rstrip())

            if line.startswith('total: '):
                splited = line.split(' ')
                lint_result['error'] = int(splited[1])
                lint_result['warning'] = int(splited[3])
                if lint_result['error'] is not 0 or lint_result['warning'] is not 0:
                    print('{}, {}'.format(line.rstrip(), fullpath))
        return lint_result
