import sys
import os
# Leave the path changes here!!!
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))
sys.path.append(os.path.join(os.path.dirname(__file__), '..', '..'))

from src.accelerated_graph_features.feature_wrapper_decorator import FeatureWrapper



@FeatureWrapper
def example_feature(graph, **kargs):
    # example_feature is a C++ function exposed to python
    import src.accelerated_graph_features._features as feat

    print(graph['indices'])
    print(graph['neighbors'])

    # Here 0 is the default value for the argument
    example_arg = kargs.get('example_arg', 0)
    res = feat.example_feature(graph)
    # Any post-processing goes here
    return res

