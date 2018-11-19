import _features as af
af.test()
test_g = {
	"indices":[0, 3, 5, 7, 10],
	"neighbors": [1, 2, 3, 0, 3, 0, 3, 0, 1, 2]
}

nodes = af.example_feature(test_g)

print(nodes)

af.clustering_coefficient(test_g)

af.k_core(test_g)

af.node_page_rank(test_g,0.1,5)

print(af.bfs_moments(test_g))
