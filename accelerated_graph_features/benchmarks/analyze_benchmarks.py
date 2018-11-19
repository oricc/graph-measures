import os
import sys

sys.path.append(os.path.abspath('..'))
sys.path.append(os.path.abspath('../..'))

import matplotlib.pyplot as plt
import numpy as np
from src.accelerated_graph_features.utils.data_reader import get_number_data


def plot_benchmark_comparison(feature_name):
    cpp_file = feature_name + '_cpp_benchmark.csv'
    python_file = feature_name + '_python_benchmark.csv'

    cpp_results = get_number_data(cpp_file)
    python_results = get_number_data(python_file)

    cpp_conversion_time = [d['Conversion Time'] / 10 ** 6 for d in cpp_results]
    cpp_feature_time = [d['Feature calculation time'] / 10 ** 6 for d in cpp_results]

    python_feature_time = [d['Feature calculation time'] / 10 ** 6 for d in python_results]

    runs = [d['run id'] for d in python_results]

    N = len(cpp_results)

    X = np.arange(N)
    width = 0.2

    cpp_conversion_bar = plt.bar(X, cpp_conversion_time, width)
    cpp_feature_bar = plt.bar(X, cpp_feature_time, width, bottom=cpp_conversion_time)
    python_feature_bar = plt.bar(X + width, python_feature_time, width)

    plt.ylabel('Time')
    plt.title('Feature Time Comparison for '+feature_name.capitalize())
    plt.xticks(X, runs, rotation=90)
    plt.legend((cpp_conversion_bar[0], cpp_feature_bar[0], python_feature_bar),
               ('C++ Conversion', 'C++ Feature', 'Python Feature'))

    plt.show()


if __name__ == '__main__':

    features = ['clustering','k_core','page_rank']

    for f in features:
        plot_benchmark_comparison(f)
