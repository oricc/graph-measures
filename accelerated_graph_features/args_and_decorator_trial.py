class FeatureWrapperDecorator(object):
    """
    This class is a decorator for the pure python function that are exposed to the user of the accelerated feature package.
    The decorator is responsible for doing the tasks that are common to all wrapper functions:
         - Converting the nx.Graph object to a converted graph dictionary.
    """

    def __init__(self, func):
        self.f = func

    def __call__(self, a, **kwargs):
        print("Hello from decorator")
        # kwargs['more'] = 'hi'
        self.f(self.convert(a), **kwargs)

    def convert(self, a):
        return a + 1


@FeatureWrapperDecorator
def wrapper_example(graph, **kwargs):
    print(graph)
    for k, v in kwargs.items():
        print('karg', str(k), ':', str(v))



@FeatureWrapperDecorator
def f(a, *args, **kwargs):
    print('a:', str(a))
    for arg in args:
        print('arg:', str(arg))
    for k, v in kwargs.items():
        print('karg', str(k), ':', str(v))


# f(1, 2, 3, four=4, five=5)
wrapper_example(1,  three=3)
