from features_infra.feature_calculators import NodeFeatureCalculator, FeatureMeta
from features_algorithms.accelerated_graph_features.__c_kernel import flow


class FlowCalculator(NodeFeatureCalculator):
    """See Y. Rozen & Y. Louzoun article <add-link>"""

    def __init__(self, *args, threshold=0, **kwargs):
        super(FlowCalculator, self).__init__(*args, **kwargs)
        self._threshold = threshold

    def is_relevant(self):
        return self._gnx.is_directed()

    def _calculate(self, include):
        self._features = flow(self._gnx, threshold=self._threshold)


feature_entry = {
    "flow": FeatureMeta(FlowCalculator, {}),
}
