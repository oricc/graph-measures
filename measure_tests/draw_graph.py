from measure_tests.run_specific_graph import load_graph, draw_graph

if __name__ == '__main__':
    path = 'undirected_test.pickle'
    # path = 'n_50_p_0.5_size_0'
    g = load_graph(path)
    draw_graph(g)
