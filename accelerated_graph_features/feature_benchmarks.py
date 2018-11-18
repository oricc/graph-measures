import sys
import os

# Leave the path changes here!!!
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

from src.accelerated_graph_features.feature_wrappers import clustering_coefficient, k_core, node_page_rank
from src.accelerated_graph_features.graph_timer import FeatureTimer

import networkx as nx

matching_python_functions = {
    'clustering': [clustering_coefficient, nx.average_clustering],
    'k_core': [k_core, nx.k_core],
    'page_rank': [node_page_rank, nx.pagerank]
}

SEED = 123456
nodes = [50, 100, 300, 500, 1000, 2000, 3000, 4000, 5000]


def benchmark_feature(feature_name, python=False):
    if python:
        feature_type = 'python'
        titles = ['Feature calculation time']
    else:
        feature_type = 'cpp'
        titles = ['Conversion Time', 'Feature calculation time']

    timer = FeatureTimer(feature_name + '_{}_benchmark'.format(feature_type),
                         titles=titles)

    for n in nodes:
        G = nx.erdos_renyi_graph(n, 0.8, seed=SEED)
        run_id = '{}_nodes_and_{}_edges'.format(n, G.size())
        print(run_id)
        timer.start(run_id)
        if python:
            matching_python_functions[feature_name][1](G)
        else:
            matching_python_functions[feature_name][0](G, timer=timer)

        if python:
            timer.stop()


def run_all_feature_tests():
    for feature_name in matching_python_functions.keys():
        benchmark_feature(feature_name, python=False)
        benchmark_feature(feature_name, python=True)


if __name__ == '__main__':
    run_all_feature_tests()
