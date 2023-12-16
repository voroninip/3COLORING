#include <unordered_set>
#include <unordered_map>
#include <cmath>
#include <queue>
#include <vector>
#include "Mask.h"

using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::queue;
using vertex = int;
using adj_list = unordered_map<vertex, unordered_set<vertex>>; // adjacency list of graph

struct Graph {
  int N;
  int M;
  adj_list Neigh;
  Mask mask;

  Graph(adj_list&& Neigh, int N, int M, Mask mask): Neigh(std::move(Neigh)), N(N), M(M), mask(mask) {}
  Graph(const adj_list& Neigh, int N, int M, Mask mask): Neigh(Neigh), N(N), M(M), mask(mask) {}
  Graph(adj_list&& Neigh, int N, int M): Neigh(std::move(Neigh)), N(N), M(M), mask((1 << N) - 1) {}
  Graph(const adj_list& Neigh, int N, int M): Neigh(Neigh), N(N), M(M), mask((1 << N) - 1) {}
};


int count_edges(const adj_list& Neigh) {
  int M = 0;
  for (const auto& pair : Neigh) {
    M += pair.second.size();
  }
  return M/2;
}

Mask N(const Graph& G, vertex v) {
  Mask neighbours = Mask(0);
  for (int u : G.Neigh.at(v)) {
    neighbours += create_mask(u);
  }
  return neighbours;
}

std::unordered_set<int> V(Mask A) {
  std::unordered_set<int> vertices;
  for (int pos = 0; pos <= ceil(log2(int(A))); ++pos) {
    if ((int(A) >> pos) & 1) {
      vertices.insert(pos + 1);
    }
  }
  return vertices;
}

int card(Mask A) {
  return __builtin_popcount(int(A));
}

Graph operator-(const Graph& G, Mask I) {
  Mask H_mask = G.mask - I;
  Graph H = Graph({}, card(H_mask), 0, H_mask);
  // H is subgraph induces by vertices in V(G)\I
  // now we correct the Neigh for H
  auto G_V = V(G.mask);
  auto H_V = V(H.mask);
  for (vertex v : G_V) {
    if (!H_V.contains(v)) {
      continue;
    } else {
      H.Neigh[v] = V(create_mask(G.Neigh.at(v)) and H_mask);
    }
  }
  H.M = count_edges(H.Neigh);
  return H;
}

Graph invert(const Graph& G) { // inverted graph (edges <-> gaps)
  int remaining_edges = (G.N * (G.N - 1) / 2) - G.M;
  Graph H = Graph({}, G.N, remaining_edges);
  for (vertex v : V(G.mask)) {
    auto it = G.Neigh.find(v);
    Mask v_neighbours = Mask(0);
    if (it != G.Neigh.end()) {
      v_neighbours = create_mask(it->second);
    }
    H.Neigh[v] = V(G.mask - v_neighbours - create_mask(v));
  }
  return H;
}

std::pair<bool, vector<int>> bipartite(const Graph& H, int N) { // coloring in two colors : 1 and (-1)
  vector<int> color(N+1, 0); // 0 stands for not colored yet
  queue<vertex> ordering;

  for (vertex v : V(H.mask)) {
    ordering.push(v);
    if (color[v] != 0) {
      continue;
    }
    color[v] = 1;
    while (!ordering.empty()) {
      vertex u = ordering.front();
      ordering.pop();
      for (const auto& w : H.Neigh.at(u)) {
        if (color[w] == color[u]) {
          return {false, color};
        }
        if (color[w] != 0) {
          continue;
        }
        color[w] = -color[u];
        ordering.push(w);
      }
    }
  }
  return {true, color};
}