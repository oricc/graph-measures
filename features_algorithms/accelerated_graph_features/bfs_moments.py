from features_infra.feature_calculators import NodeFeatureCalculator, FeatureMeta
from features_algorithms.accelerated_graph_features.__c_kernel import bfs_moments


class BfsMomentsCalculator(NodeFeatureCalculator):
    def is_relevant(self):
        return True

    def _calculate(self, include: set):
        self._features = bfs_moments(self._gnx)


feature_entry = {
    "bfs_moments": FeatureMeta(BfsMomentsCalculator, {"bfs"}),
}
