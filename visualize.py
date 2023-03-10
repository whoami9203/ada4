import matplotlib.pyplot as plt
import networkx as nx

G = nx.MultiDiGraph()

PREFIX = 'testcase'
IN = f'{PREFIX}.in'
OUT = f'{PREFIX}.out'

edges = []

with open(IN, 'r') as f:
    N, M = map(int, f.readline().strip().split())
    # print(N, M)
    G.add_nodes_from([i for i in range(1, N + 1)])
    for _ in range(M):
        u, v, w = map(int, f.readline().strip().split())
        edges.append((u, v))

with open(OUT, 'r') as f:
    opt_ans = int(f.readline().strip())
    if opt_ans == -1:
        print('no sol!')
        exit(0)
    in_path = f.readline().strip()
    weighted_edges = []
    for i in range(M):
        u, v = edges[i]
        w = 5 if in_path[i] == '1' else 1
        weighted_edges.append((u, v, {'weight': w}))
    G.add_edges_from(weighted_edges)

# print(G.edges(data='weight', keys=True))

weights = [
        tt[3]
        for tt in G.edges(data='weight', keys=True)
    ]

subax2 = plt.subplot(111)
pos = nx.nx_agraph.graphviz_layout(G)
nx.draw(G, with_labels=True, pos=pos, width=weights)

plt.show()
