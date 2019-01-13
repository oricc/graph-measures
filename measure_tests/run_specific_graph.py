import networkx as nx
from loggers import PrintLogger
import numpy as np
from features_algorithms.vertices.motifs import MotifsNodeCalculator
import os
import matplotlib.pyplot as plt

logger = PrintLogger("MyLogger")

PREFIX = 'specific_graphs'


def load_graph(path):
    g = nx.read_gpickle(open(os.path.join(PREFIX, path), 'rb'))
    # nodes = list({21, 36, 13, 43, 21, 14, 45, 35, 21, 14, 48, 44, 21, 14, 48, 41, 21, 12, 18, 41})
    # nodes = [21, 14, 48, 41, 44, 45, 35, 21, 12, 18, 41]
    # nodes = [21, 36, 13, 43, 21, 14, 48, 41, 44]
    nodes = list({21, 14, 48, 41, 44, 45, 35, 21, 12, 18, 41, 21, 36, 13, 43})
    return nx.subgraph(g, nodes)


def run_feature(gnx):
    feature = MotifsNodeCalculator(gnx, level=4, logger=logger)
    feature.build()

    mx = feature.to_matrix(mtype=np.matrix, should_zscore=False)
    return mx


def draw_graph(gnx: nx.Graph):
    pos = nx.layout.spring_layout(gnx)
    nx.draw_networkx_nodes(gnx, pos)
    if gnx.is_directed():
        nx.draw_networkx_edges(gnx, pos, arrowstyle='->', arrowsize=30)
    else:
        nx.draw_networkx_edges(gnx, pos)

    nx.draw_networkx_labels(gnx, pos, font_size=10, font_family='sans-serif')

    plt.axis('off')
    plt.show()


def main():
    path = 'n_50_p_0.5_size_0'
    g = load_graph(path)
    # draw_graph(g)
    res = run_feature(g)
    pass


if __name__ == '__main__':
    main()
