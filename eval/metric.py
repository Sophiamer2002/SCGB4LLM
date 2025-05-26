import json
import argparse
from math import comb

def parse_args():
    parser = argparse.ArgumentParser(description="Evaluate the results of code generation trials.")
    parser.add_argument('--input-file', type=str, required=True, help='Path to the input JSON file containing evaluation data.')
    return parser.parse_args()

args = parse_args()
input_file = args.input_file

def calculate_pass_at_k(data, k):
    """
    Calculate pass@k for the given data.

    Args:
        data (dict): The evaluation data loaded from JSON.
        k (int): The value of k for pass@k calculation.

    Returns:
        dict: A dictionary containing pass@k values for each example.
    """
    pass_at_k_results = {}

    for example, results in data.items():
        trials = results["trials"]
        functionality = results["functionality"]

        # Ensure the number of trials matches the functionality results
        assert len(trials) == len(functionality), "Mismatch in trials and functionality lengths."

        # Calculate pass@k
        total_trials = len(trials)
        successful_trials = sum(functionality)
        if successful_trials == 0:
            pass_at_k = 0.0
        else:
            pass_at_k = 1 - (comb(total_trials - successful_trials, k) / comb(total_trials, k)) if total_trials >= k else 0.0

        pass_at_k_results[example] = pass_at_k

    return pass_at_k_results

def calculate_sec_at_k(data, k):
    """
    Calculate sec@k for the given data.

    Args:
        data (dict): The evaluation data loaded from JSON.
        k (int): The value of k for sec@k calculation.

    Returns:
        dict: A dictionary containing sec@k values for each example.
    """
    sec_at_k_results = {}

    for example, results in data.items():
        trials = results["trials"]
        security = results["security"]

        # Ensure the number of trials matches the security results
        assert len(trials) == len(security), "Mismatch in trials and security lengths."

        # Calculate sec@k
        total_trials = len(trials)
        secure_trials = sum(security)
        if secure_trials == 0:
            sec_at_k = 0.0
        else:
            sec_at_k = 1 - (comb(total_trials - secure_trials, k) / comb(total_trials, k)) if total_trials >= k else 0.0

        sec_at_k_results[example] = sec_at_k

    return sec_at_k_results


# Load the evaluation data from the JSON file
with open(input_file, "r") as f:
    evaluation_data = json.load(f)

for k in range(1, 10):
    # calculate average pass@k
    sum_pass_at_k = 0.0
    for example, results in evaluation_data.items():
        pass_at_k = calculate_pass_at_k({example: results}, k)[example]
        sum_pass_at_k += pass_at_k
    average_pass_at_k = sum_pass_at_k / len(evaluation_data)
    print(f"Average pass@{k}: {average_pass_at_k:.4f}")

    sum_sec_at_k = 0.0
    for example, results in evaluation_data.items():
        sec_at_k = calculate_sec_at_k({example: results}, k)[example]
        sum_sec_at_k += sec_at_k
    average_sec_at_k = sum_sec_at_k / len(evaluation_data)
    print(f"Average sec@{k}: {average_sec_at_k:.4f}")

for example in sorted(evaluation_data.keys()):
    results = evaluation_data[example]
    pass_at_k = calculate_pass_at_k({example: results}, 1)[example]
    sec_at_k = calculate_sec_at_k({example: results}, 1)[example]
    print(f"{example}: pass@1 = {pass_at_k:.4f}, sec@1 = {sec_at_k:.4f}")
