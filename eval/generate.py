import os
import time
import json
import argparse

from tqdm import tqdm
from concurrent.futures import ThreadPoolExecutor, as_completed
from query import Query
from itertools import product

# the benchmark base directory
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

parser = argparse.ArgumentParser()
parser.add_argument('--save-dir', default=os.path.join(BASE_DIR, 'result', time.strftime('%Y%m%d_%H%M%S')))
parser.add_argument('--trials', default=10, type=int)
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
parser.add_argument('--model', type=str, default='glm-4-flash')
parser.add_argument('--api_endpoint', type=str, default='https://open.bigmodel.cn/api/paas/v4')
parser.add_argument('--concurrency', type=int, default=2)

args = parser.parse_args()
api_key = os.getenv('API_KEY')
cves = []

# region 1: vanity checks and preparations

# region 1.1: check if the dataset directory exists and has the correct structure

if not os.path.exists(args.dataset_dir):
    raise FileNotFoundError(f'Dataset directory {args.dataset_dir} does not exist')
if not os.path.isdir(args.dataset_dir):
    raise NotADirectoryError(f'{args.dataset_dir} is not a directory')

for cve_dir in os.listdir(args.dataset_dir):
    cve_dir_path = os.path.join(args.dataset_dir, cve_dir)
    if not os.path.isdir(cve_dir_path):
        continue
    if not os.path.exists(os.path.join(cve_dir_path, 'code.c')):
        raise FileNotFoundError(f'{cve_dir_path} does not contain code.c')
    if not os.path.exists(os.path.join(cve_dir_path, 'function.c')):
        raise FileNotFoundError(f'{cve_dir_path} does not contain function.c')
    if not os.path.exists(os.path.join(cve_dir_path, 'metadata.json')):
        raise FileNotFoundError(f'{cve_dir_path} does not contain metadata.json')
    if not os.path.exists(os.path.join(cve_dir_path, 'security.c')):
        raise FileNotFoundError(f'{cve_dir_path} does not contain security.c')
    
    cves.append(cve_dir)

# endregion

# region 1.2: save directory should not exist

if os.path.exists(args.save_dir):
    raise FileExistsError(f'{args.save_dir} already exists')
os.makedirs(args.save_dir)

# endregion

# endregion

# region 2: generate the patched code for each CVE

output = {
    "output": {
        cve: [] for cve in cves
    },
    **args.__dict__,
}

with ThreadPoolExecutor(max_workers=args.concurrency) as executor:
    futures = []
    for cve in cves:

        with open(os.path.join(args.dataset_dir, cve, 'code.c'), 'r') as f:
            code_prompt = f.read()
            code_prompt = code_prompt.split('// BEGIN SOLUTION')[0] + code_prompt.split('// END SOLUTION')[1]
        with open(os.path.join(args.dataset_dir, cve, 'metadata.json'), 'r') as f:
            function_name = json.load(f)['function_name']
        
        print(code_prompt, function_name)

        for _ in range(args.trials):
            futures.append(executor.submit(
                Query(args.api_endpoint, api_key, args.model),
                code_prompt,
                function_name,
            ))

    for future, (cve, _) in tqdm(zip(as_completed(futures), product(cves, range(args.trials))), total=len(futures)):
        if future.exception() is not None:
            output['output'][cve].append({
                "error": True,
                "output": str(future.exception())
            })
        else:
            output['output'][cve].append({
                "error": False,
                "output": future.result()
            })

with open(os.path.join(args.save_dir, 'output.json'), 'w') as f:
    json.dump(output, f, indent=4, ensure_ascii=False)

# endregion
