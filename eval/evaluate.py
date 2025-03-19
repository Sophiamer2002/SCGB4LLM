import os
import json
import argparse

from itertools import product

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

parser = argparse.ArgumentParser()
parser.add_argument('--save-dir', required=True)
parser.add_argument('--dataset-dir', default=os.path.join(BASE_DIR, 'data'), help=''''
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
    └── macro.h
''')

args = parser.parse_args()
with open(os.path.join(args.save_dir, 'output.json'), 'r') as f:
    output = json.load(f)
cves = list(output['output'].keys())
trials = output['trials']

artifacts = os.path.join(args.save_dir, 'artifacts')
os.makedirs(artifacts, exist_ok=True)

for cve, trial in product(cves, range(trials)):
    pass
