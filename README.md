# Secure Code Generation Benchmark for LLM

This repository contains a benchmark for evaluating the secure code generation capabilities of Large Language Models (LLMs). We focus on vulnerabilities in C language, specifically targeting CWE-119 (Improper Restriction of Operations within the Bounds of a Memory Buffer), CWE-190 (Integer Overflow or Wraparound), and CWE-825 (Expired Pointer Dereference).

## Environment Setup

Simply run the following command to set up the environment:

```bash
pip install -r requirements.txt
```

You also need to ensure that `gcc` is installed on your system, and the version is at least 4.8.0. You can check the version by running:

```bash
gcc --version
```


## Run the benchmark

In total, there are three steps:
1. **Generate LLM responses**:
    ```bash
    python eval/generate.py --api_endpoint <api-endpoint> --model <model-name> --save-dir <save-dir-path> --trials <num-trials>
    ```

    The LLM must be hosted on an OpenAI-compatible API endpoint. Refer to the service provider's documentation for the correct model name and API endpoint details. Also **REMEMBER TO SET THE API KEY** in your environment variables as `API_KEY`.

    The default save directory is `./results/YYYYMMDD-HHMMSS`, where `YYYYMMDD-HHMMSS` is the current timestamp. You can specify a different directory using the `--save-dir` option.

    The `--trials` option specifies the number of trials for each code sample. The default is 10, but you can increase it to get more responses from the LLM.

    After running this command, there will be a directory created at `<save-dir-path>`, and a file named `output.json` inside it. This file contains the LLM responses for each code sample in the dataset.


2. **Evaluate the responses**

    ```bash
    python eval/evaluate.py --save-dir <save-dir-path>
    ```

    The `--save-dir` option should point to the directory where the LLM responses were saved in the previous step. The evaluation script will read the responses and generate a report based on the evaluation criteria defined in `eval/evaluate.py`, including the compilation capability, functional correctness and security of the generated code.

    After running this command, a file named `evaluation.json` will be created in the same directory as the LLM responses. This file contains the evaluation results for each code sample, including whether the code compiled successfully, passed functional tests, and whether it contained security vulnerabilities.


3. **Generate the report**

    ```bash
    python eval/metric.py --input-file <save-dir-path>/evaluation.json
    ```

    The `--input-file` option should point to the `evaluation.json` file generated in the previous step. This script will calculate the pass@k and sec@k metrics based on the evaluation results. It will also give pass@1 for each code generation task.


## Dataset Structure

The dataset is well-organized in directory `<root-dir-path>/data` as follows:

```
<root-dir-path>/data
├── <data-sample-id>
│   ├── code.c
│   ├── function.c
│   ├── metadata.json
|   └── security.c
├── <data-sample-id>
│   ├── code.c
│   ├── function.c
│   ├── metadata.json
|   └── security.c
├── <data-sample-id>
...
```

You can add your own code samples in the same format to the `data` directory. Each sample should have a unique `<data-sample-id>` and contain the following files:
- `code.c`: The original code task that may contain vulnerabilities. The area started with `// BEGIN SOLUTION` and ended with `// END SOLUTION` is the code that needs completed.
- `function.c`: functionality test code that checks the correctness of the code.
- `security.c`: security test code that checks the security of the code.
- `metadata.json`: Metadata file containing information about the code sample, such as the task description, expected output, and any other relevant details.

