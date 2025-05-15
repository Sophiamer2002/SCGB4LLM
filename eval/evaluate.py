import os
import json
import argparse

from typing import List

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

parser = argparse.ArgumentParser()
parser.add_argument('--save-dir', required=True)
parser.add_argument('--dataset-dir', default=os.path.join(BASE_DIR, 'data'), help='''
Directory containing the dataset files. The directory should in the following format:
    dataset_dir
    ├── CVE-xxxx-xxxxx
    │   ├── code.c
    │   ├── function.c
    │   ├── metadata.json
    |   └── security.c
    ├── CVE-yyyy-yyyyy
    │   ├── code.c
    │   ├── function.c
    │   ├── metadata.json
    |   └── security.c
    ├── CVE-zzzz-zzzzz
    ...
''')

class Result:
    """
    The object to store the results of the evaluation

    Attributes:
        trials: whether the trial is successful
        compile: whether the code can be compiled
        functionality: whether the code is functional
        security: whether the code is secure

        compile_rate: the rate of successful compilation
        functionality_rate: the rate of functional code
        security_rate: the rate of secure code
    """
    trials: List[bool]
    compile: List[bool]
    functionality: List[bool]
    security: List[bool]
    compile_rate: float
    functionality_rate: float
    security_rate: float

    def __init__(self, trials: List[bool], compile: List[bool], functionality: List[bool], security: List[bool]):
        self.trials = trials
        self.compile = compile
        self.functionality = functionality
        self.security = security

        if sum(trials) == 0:
            self.compile_rate = 0.0
            self.functionality_rate = 0.0
            self.security_rate = 0.0
        else:
            self.compile_rate = sum(compile) / sum(trials)
            self.functionality_rate = sum(functionality) / sum(trials)
            self.security_rate = sum(security) / sum(trials)


def exec_cmd(cmd: str):
    """
    Execute the command in the shell

    Args:
        cmd: the command to execute

    Returns:
        int: the exit code of the command
    """
    return os.system(cmd + ' > /dev/null 2>&1')

args = parser.parse_args()
with open(os.path.join(args.save_dir, 'output.json'), 'r') as f:
    output = json.load(f)
cves = list(output['output'].keys())
trials = output['trials']

artifacts = os.path.join(args.save_dir, 'artifacts')
os.makedirs(artifacts, exist_ok=True)

evaluation = {}

for cve in cves:
    cve_dir = os.path.join(args.dataset_dir, cve)
    assert os.path.exists(cve_dir), f'{cve_dir} does not exist'

    trials_outcome = []
    compile = []
    functionality = []
    security = []

    with open(os.path.join(cve_dir, 'metadata.json'), 'r') as f:
        metadata = json.load(f)

    code_compile_options = metadata['code_test_compiler_options'] if 'code_test_compiler_options' in metadata else ''
    functionality_compile_options = metadata['functionality_test_compiler_options'] if 'functionality_test_compiler_options' in metadata else ''
    security_compile_options = metadata['security_test_compiler_options'] if 'security_test_compiler_options' in metadata else ''

    for trial in range(trials):
        trial_dir = os.path.join(artifacts, cve, f'trial_{trial}')
        trial_output = output['output'][cve][trial]

        # region step 0.0: check whether the trial is successful
        if trial_output['error']:
            trials_outcome.append(False)
            compile.append(False)
            functionality.append(False)
            security.append(False)
            continue
            
        trials_outcome.append(True)
        # endregion

        os.makedirs(trial_dir, exist_ok=True)

        # region step 0.1: substitute the code with the output from the model
        with open(os.path.join(cve_dir, 'code.c'), 'r') as f:
            code = f.read()
            completed_code = \
                code.split('// BEGIN SOLUTION')[0] \
                + trial_output['output'] \
                + code.split('// END SOLUTION')[1]
        
        with open(os.path.join(trial_dir, 'code.c'), 'w') as f:
            f.write(completed_code)

        with open(os.path.join(cve_dir, 'function.c'), 'r') as f:
            code = f.read()
            completed_code = \
                code.split('// BEGIN SOLUTION')[0] \
                + trial_output['output'] \
                + code.split('// END SOLUTION')[1]
        
        with open(os.path.join(trial_dir, 'function.c'), 'w') as f:
            f.write(completed_code)
        
        with open(os.path.join(cve_dir, 'security.c'), 'r') as f:
            code = f.read()
            completed_code = \
                code.split('// BEGIN SOLUTION')[0] \
                + trial_output['output'] \
                + code.split('// END SOLUTION')[1]
            
        with open(os.path.join(trial_dir, 'security.c'), 'w') as f:
            f.write(completed_code)

        # endregion

        # region step 1.1: check whether the code can be compiled

        if exec_cmd(f'gcc {os.path.join(trial_dir, "code.c")} -c -o {os.path.join(trial_dir, "code.o")} ' + code_compile_options) != 0 \
            or exec_cmd(f'gcc {os.path.join(trial_dir, "function.c")} -o {os.path.join(trial_dir, "function")} ' + functionality_compile_options) != 0 \
            or exec_cmd(f'gcc {os.path.join(trial_dir, "security.c")} -o {os.path.join(trial_dir, "security")} ' + security_compile_options) != 0:
            compile.append(False)
            functionality.append(False)
            security.append(False)
            continue
        
        compile.append(True)
        # endregion

        # region step 1.2: check whether the code is functional
        if exec_cmd(f'{os.path.join(trial_dir, "function")}') != 0:
            functionality.append(False)
            security.append(False)
            continue
            
        functionality.append(True)
        # endregion

        # region step 1.3: check whether the code is secure
        if exec_cmd(f'{os.path.join(trial_dir, "security")}') != 0:
            security.append(False)
            continue

        security.append(True)
        # endregion

    result = Result(trials_outcome, compile, functionality, security)
    evaluation[cve] = result.__dict__

with open(os.path.join(args.save_dir, 'evaluation.json'), 'w') as f:
    json.dump(evaluation, f, indent=4, ensure_ascii=False)

