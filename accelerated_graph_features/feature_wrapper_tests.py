import sys
import os

# Leave the path changes here!!!
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

from src.accelerated_graph_features.test_python_converter import create_graph
from src.accelerated_graph_features.feature_wrappers import example_feature, clustering_coefficient, k_core, \
    node_page_rank, bfs_moments, motif


def test_features():
    g = create_graph()
    example_feature(g)
    clustering_coefficient(g)
    k_core(g)
    node_page_rank(g)
    bfs_moments(g)
    m_res = motif(g, level=3)
    print(m_res)

if __name__ == '__main__':
    test_features()
