from feature_wrapper_decorator import FeatureWrapper
from test_python_converter import create_graph


@FeatureWrapper
def example_feature(graph, **kargs):
    # example_feature is a C++ function exposed to python
    from src. _features import example_feature

    print(graph['indices'])
    print(graph['neighbors'])

    # Here 0 is the default value for the argument
    example_arg = kargs.get('example_arg', 0)
    res = example_feature(graph, example_arg)
    # Any post-processing goes here
    return res


if __name__ == '__main__':
    example_feature(create_graph())