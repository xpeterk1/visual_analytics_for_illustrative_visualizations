#include "Icosphere.h"


Icosphere::Icosphere(int subdivisions)
{
    for (int i = 0; i < subdivisions; ++i)
    {
        triangles = subdivide(vertices, triangles);
    }
}

Index Icosphere::vertex_for_edge(Lookup& lookup, std::vector<glm::vec3>& vertices, Index first, Index second)
{
    Lookup::key_type key(first, second);
    if (key.first > key.second)
        std::swap(key.first, key.second);

    auto inserted = lookup.insert({ key, vertices.size() });
    if (inserted.second)
    {
        auto& edge0 = vertices[first];
        auto& edge1 = vertices[second];
        auto point = normalize(edge0 + edge1);
        vertices.push_back(point);
    }

    return inserted.first->second;
}

std::vector<Triangle> Icosphere::subdivide(std::vector<glm::vec3>& vertices, std::vector<Triangle> triangles)
{
    Lookup lookup;
    std::vector<Triangle> result;

    for (auto&& each : triangles)
    {
        Index mid[3];
        for (int edge = 0; edge < 3; ++edge)
        {
            mid[edge] = vertex_for_edge(lookup, vertices, each.vertex[edge], each.vertex[(edge + 1) % 3]);
        }

        result.push_back({ each.vertex[0], mid[0], mid[2] });
        result.push_back({ each.vertex[1], mid[1], mid[0] });
        result.push_back({ each.vertex[2], mid[2], mid[1] });
        result.push_back({ mid[0], mid[1], mid[2] });
    }

    return result;
}

void Icosphere::draw() 
{
    GLuint VAO, VBO, elementbuffer;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &elementbuffer);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    
    std::vector<unsigned int> indices;
    for (const auto& triangle : triangles) {
        indices.push_back(triangle.vertex[0]);
        indices.push_back(triangle.vertex[1]);
        indices.push_back(triangle.vertex[2]);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &elementbuffer);
}