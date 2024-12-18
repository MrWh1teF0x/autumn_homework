#pragma once

#include <map>

#include "vertex.hpp"

template <typename T>
class Graph {
 public:
  Graph(bool is_oriented = true) : is_oriented(is_oriented) {}

  Graph(const std::vector<Vertex<T>>& vertices, bool is_oriented = true)
      : vertices(vertices), is_oriented(is_oriented) {}

  Graph(const std::vector<std::pair<T, T>>& edges, bool is_oriented = true)
      : is_oriented(is_oriented) {
    std::map<T, std::vector<T>> vertices_to_add;

    for (auto edge : edges) {
      vertices_to_add[edge.first].push_back(edge.second);
      if (!is_oriented) vertices_to_add[edge.second].push_back(edge.first);
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

  bool IsOriented() const { return is_oriented; }

  std::vector<Vertex<T>> GetVertices() const { return vertices; }

  std::vector<T> GetVerticesIds() const {
    std::vector<T> vertices_ids;
    for (int i = 0; i < vertices.size(); i++)
      vertices_ids.push_back(vertices[i].GetVertexId());
    return vertices_ids;
  }

  std::vector<T> GetAdjVertices(const T& vertex) const {
    if (!ContainsVertex(vertex))
      throw std::invalid_argument("Vertex not found!");

    std::vector<T> adj_vertices;
    for (int i = 0; i < vertices.size(); i++) {
      if (vertices[i].GetVertexId() == vertex) {
        adj_vertices = vertices[i].GetAdjVertices();
        break;
      }
    }
    return adj_vertices;
  }

  size_t GetVerticesCount() const { return vertices.size(); }

  size_t GetEdgesCount() const {
    size_t count = 0;
    for (int i = 0; i < vertices.size(); i++)
      count += vertices[i].GetAdjVerticesCount();
    // У неориентированного графа в два раза меньше ребер, чем у такого же
    // ориентированного
    if (!is_oriented) count /= 2;

    return count;
  }

  bool ContainsVertex(const Vertex<T>& vertex) const {
    for (auto vert : vertices) {
      if (vert.GetVertexId() == vertex.GetVertexId() &&
          vert.GetAdjVerticesCount() == vertex.GetAdjVerticesCount()) {
        // Проверяем смежности
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
    for (auto vert : vertices) {
      if (vert.GetVertexId() == vertex) return true;
    }
    return false;
  }

  bool ContainsEdge(const T& vert_1, const T& vert_2) const {
    for (int i = 0; i < vertices.size(); i++) {
      if (vert_1 == vertices[i].GetVertexId() &&
          vertices[i].ContainsAdjVertex(vert_2))
        return true;
    }
    return false;
  }

  void AddVertex(const Vertex<T>& vertex) {
    if (is_oriented) {
      vertices.push_back(vertex);
      return;
    }

    // Добавление вершины для неориентированного графа
    for (auto adj_vertex : vertex.GetAdjVertices()) {
      if (!ContainsVertex(adj_vertex)) AddVertex(adj_vertex);

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

  void AddEdge(const T& vert_1, const T& vert_2) {
    if (!ContainsVertex(vert_1)) AddVertex(vert_1);
    if (!ContainsVertex(vert_2)) AddVertex(vert_2);

    for (int i = 0; i < vertices.size(); i++) {
      if (vertices[i].GetVertexId() == vert_1) {
        vertices[i].AddAdjVertex(vert_2);
        if (is_oriented) return;
        break;
      }
    }

    // Если граф неориентированный, то мы добавляем ребро vert_2->vert_1
    for (int i = 0; i < vertices.size(); i++) {
      if (vertices[i].GetVertexId() == vert_2) {
        vertices[i].AddAdjVertex(vert_1);
        return;
      }
    }
  }

  void DeleteVertex(const T& vertex) {
    if (!ContainsVertex(vertex))
      throw std::invalid_argument("Vertex not found!");

    for (int i = 0; i < vertices.size(); i++) {
      if (vertex == vertices[i].GetVertexId()) {
        vertices.erase(vertices.begin() + i);

        // Удаляем смежности с удаленной вершиной
        for (int i = 0; i < vertices.size(); i++) {
          if (vertices[i].ContainsAdjVertex(vertex))
            vertices[i].DeleteAdjVertex(vertex);
        }
        return;
      }
    }
  }

  void DeleteEdge(const T& vert_1, const T& vert_2) {
    if (!ContainsVertex(vert_1)) throw std::invalid_argument("Edge not found!");

    for (int i = 0; i < vertices.size(); i++) {
      if (vertices[i].GetVertexId() == vert_1) {
        vertices[i].DeleteAdjVertex(vert_2);
        if (is_oriented)
          return;
        else
          break;
      }
    }

    // Для неориентированного графа нужно удалить и у vert_2 смежную вершину
    // vert_1
    if (!ContainsVertex(vert_2)) throw std::invalid_argument("Edge not found!");

    for (int i = 0; i < vertices.size(); i++) {
      if (vertices[i].GetVertexId() == vert_2) {
        vertices[i].DeleteAdjVertex(vert_1);
        return;
      }
    }
  }

 private:
  std::vector<Vertex<T>> vertices;
  bool is_oriented;
};