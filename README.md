Implementation of __Lawler's__ 3-colorability test
1. Use [Bron–Kerbosch](https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm) algorithm for finding all maximal independant sets in graph
2. Check the induced subgraph on each complementary set of vertices for bipartiteness (just coloring them one by one)
3. If there exist 2-colorable complementary for max ind set K, then initial graph is 3-colorable:
   just use one color for all the vertices in K in and remain coloring of complementary 


Asymptotic: 
```math
O^*(3^{n/3}), \ \ \ \ where\ \ \ \ \sqrt[3]{3} \approx 1,44225
```
