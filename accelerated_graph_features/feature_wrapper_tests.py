import sys
import os

# Leave the path changes here!!!
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

from src.accelerated_graph_features.test_python_converter import create_graph
from src.accelerated_graph_features.feature_wrappers import example_feature


def test_example_feature():
    example_feature(create_graph())


if __name__ == '__main__':
    test_example_feature()
