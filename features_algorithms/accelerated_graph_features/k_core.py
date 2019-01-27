from features_infra.feature_calculators import NodeFeatureCalculator, FeatureMeta
from features_algorithms.accelerated_graph_features.__c_kernel import k_core


class KCoreCalculator(NodeFeatureCalculator):
    def is_relevant(self):
        return True

    def _calculate(self, include: set):
        self._features = k_core(self._gnx)


feature_entry = {
    "k_core": FeatureMeta(KCoreCalculator, {"kc"}),
}
