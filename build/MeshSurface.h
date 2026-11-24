#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <learnopengl/shader_m.h> // <-- ضع هنا نفس سطر الشيدر الموجود في Main.cpp عندك

struct SurfaceVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class MeshSurface {
private:
    unsigned int VAO, VBO;
    int vertexCount;
    unsigned int textureID;
    glm::vec3 color;

public:
    MeshSurface(const std::vector<SurfaceVertex>& vertices, unsigned int texID, glm::vec3 objColor = glm::vec3(1.0f))
        : vertexCount(vertices.size()), textureID(texID), color(objColor) {
        setupMesh(vertices);
    }

    static MeshSurface* createRectangle(float width, float height, unsigned int texID, glm::vec3 objColor = glm::vec3(1.0f), float uvRepeatX = 1.0f, float uvRepeatY = 1.0f) {
        float hw = width / 2.0f;
        float hh = height / 2.0f;
        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        std::vector<SurfaceVertex> verts = {
            { glm::vec3(-hw, -hh, 0.0f), normal, glm::vec2(0.0f, 0.0f) },
            { glm::vec3( hw, -hh, 0.0f), normal, glm::vec2(uvRepeatX, 0.0f) },
            { glm::vec3( hw,  hh, 0.0f), normal, glm::vec2(uvRepeatX, uvRepeatY) },
            { glm::vec3(-hw,  hh, 0.0f), normal, glm::vec2(0.0f, uvRepeatY) }
        };
        return new MeshSurface(verts, texID, objColor);
    }

    void draw(Shader& shader, const glm::mat4& modelMatrix) {
        shader.use();
        shader.setMat4("model", modelMatrix);
        shader.setVec3("objectColor", color);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        shader.setInt("texture_diffuse1", 0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
        glBindVertexArray(0);
    }

    void setColor(glm::vec3 newColor) { color = newColor; }
    void setTexture(unsigned int newTexID) { textureID = newTexID; }

    ~MeshSurface() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

private:
    void setupMesh(const std::vector<SurfaceVertex>& vertices) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SurfaceVertex), vertices.data(), GL_STATIC_DRAW);

        // 0: Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SurfaceVertex), (void*)0);
        // 1: Normal
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SurfaceVertex), (void*)offsetof(SurfaceVertex, Normal));
        // 2: TexCoords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SurfaceVertex), (void*)offsetof(SurfaceVertex, TexCoords));

        glBindVertexArray(0);
    }
};
