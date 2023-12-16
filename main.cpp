#include <iostream>
#include <list>
#include "Graph.h"

using std::list;

list<Mask> Bron_Kerbosch(const Graph& G, Mask R, Mask P, Mask X) {
  // finds all max cliques which include all the vertices in R, some of the vertices in P, and none of the vertices in X
  if (P.empty() and X.empty()) {
    // std::cout << int(R) << ' ';
    return {R};
  }

  list<Mask> cliques;
  for (auto v : V(P)) {
    Mask N_v = N(G, v);
    Mask mask_v = create_mask(v);
    auto new_cliques = Bron_Kerbosch(G, R + mask_v, P and N_v, X and N_v);
    cliques.insert(cliques.end(), new_cliques.begin(), new_cliques.end());
    P = P - mask_v;
    X = X + mask_v;
  }

  return cliques;
}

void Lawler(const Graph& G, const auto& max_cliques) {
  for (auto K : max_cliques) {
    Graph H = G - K;
    auto bipart = bipartite(H, G.N);
    if (bipart.first) {
      std::cout << "3-colorable\n";
      auto appropriate_ind_set = V(K);
      auto G_V = V(G.mask);
      std::cout << "Vertices: \t";
      for (vertex v : G_V) {
        std::cout << v << ' ';
      }
      std::cout << '\n';
      std::cout << "Colors:   \t";
      for (vertex v : G_V) {
        if (100 > v and v >= 10) {
          std::cout << ' ';
        }
        if (appropriate_ind_set.contains(v)) {
          std::cout << 2 << ' ';
        } else {
          std::cout << bipart.second[v] + 2 << ' ';
        }
      }
      return;
    }
  }
  std::cout << "NOT 3-colorable";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  int N, M;
  std::cin >> N >> M;
  adj_list Neigh(N+1);

  int v, u;
  for (int i = 0; i < M; ++i) {
    std::cin >> v >> u;
    Neigh[v].insert(u);
    Neigh[u].insert(v);
  }

  Graph G = Graph(std::move(Neigh), N, M);
  auto max_cliques = Bron_Kerbosch(invert(G), Mask(0), Mask((1 << N) - 1), Mask(0));
  Lawler(G, max_cliques);
}
