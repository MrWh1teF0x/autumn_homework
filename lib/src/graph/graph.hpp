#include <map>

#include "vertex.hpp"

template <typename T>
class Graph {
 public:
  Graph(const std::vector<Vertex<T>>& vertices) : vertices(vertices) {}

  Graph(const std::vector<std::pair<T, T>>& edges) {
    std::map<T, std::vector<T>> vertices_to_add;

    for (auto edge : edges) {
      vertices_to_add[edge.first].push_back(edge.second);
      vertices_to_add[edge.second].push_back(edge.first);
    }

    for (auto vertex : vertices_to_add) {
      vertices.push_back(Vertex(vertex.first, vertex.second));
    }
  }

  void PrintAdjList() const {
    for (auto vertex : vertices) {
      vertex.PrintAdjVertices();
    }
  }

  std::vector<Vertex<T>> GetVertices() const { return vertices; }

  size_t GetVerticesCount() const { return vertices.size(); }

  void AddEdge(const T& vert_1, const T& vert_2) {
    if (!ContainsVertex(vert_1) || !ContainsVertex(vert_2))
      throw std::invalid_argument("Vertex not found!");

    for (int i = 0; i < vertices.size(); i++) {
      if (vertices[i].GetVertexId() == vert_1) vertices[i].AddAdjVertex(vert_2);
      if (vertices[i].GetVertexId() == vert_2) vertices[i].AddAdjVertex(vert_1);
    }
  }

  void AddVertex(const Vertex<T>& vertex) {
    for (const T& adj_vertex : vertex.GetAdjVertices()) {
      if (!ContainsVertex(adj_vertex))
        throw std::invalid_argument("Adj vertex not found!");

      for (int i = 0; i < vertices.size(); i++) {
        if (adj_vertex == vertices[i].GetVertexId()) {
          vertices[i].AddAdjVertex(vertex.GetVertexId());
          break;
        }
      }
    }
    vertices.push_back(vertex);
  }

  void AddVertex(const T& vertex) { vertices.push_back(Vertex<T>(vertex)); }

  void DeleteVertex(const T& vertex) {
    if (!ContainsVertex(vertex))
      throw std::invalid_argument("Vertex not found!");

    for (int i = 0; i < vertices.size(); i++) {
      if (vertex == vertices[i].GetVertexId()) {
        vertices.erase(vertices.begin() + i);
        return;
      }
    }
  }

  bool ContainsVertex(const Vertex<T>& vertex) const {
    for (const Vertex<T>& vert : vertices) {
      if (vert.GetVertexId() == vertex.GetVertexId() &&
          vert.GetAdjVerticesCount() == vertex.GetAdjVerticesCount()) {
        auto adj_verts = vertex.GetAdjVertices();
        for (int i = 0; i < adj_verts.size(); i++) {
          if (!vertex.ContainsAdjVertex(adj_verts[i])) return false;
        }
        return true;
      }
    }
    return false;
  }

  bool ContainsVertex(const T& vertex) const {
    for (const Vertex<T>& vert : vertices) {
      if (vert.GetVertexId() == vertex) return true;
    }
    return false;
  }

  bool ContainsEdge(const T& vert_1, const T& vert_2) const {
    for (int i = 0; i < vertices.size(); i++) {
      if (vert_1 == vertices[i].GetVertexId() ||
          vert_2 == vertices[i].GetVertexId()) {
        if (vertices[i].ContainsAdjVertex(vert_1) ||
            vertices[i].ContainsAdjVertex(vert_2))
          return true;
      }
    }
    return false;
  }

 private:
  std::vector<Vertex<T>> vertices;
};