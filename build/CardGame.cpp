#include "CardGame.h"
#include <stb_image.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

CardGame::CardGame() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    cameraMode = 1;
    playedCount = 0;
    isTransitioning = false;

    currentYaw = -90.0f;
    currentPitch = -85.0f;
    targetYaw = -90.0f;
    targetPitch = -85.0f;

    currentCamPos = glm::vec3(0.0f, 5.2f, 0.01f);
    targetCamPos = currentCamPos;

    initGeometry();
    initTextures();
    initCards();
}

CardGame::~CardGame() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &tornPosterVAO);
    glDeleteBuffers(1, &tornPosterVBO);
    glDeleteVertexArrays(1, &cylinderVAO);
    glDeleteBuffers(1, &cylinderVBO);

    for (auto card : cards) {
        delete card;
    }
}

void CardGame::initGeometry() {
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    float quadVertices[] = {
        -0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         0.5f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
         0.5f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    float tornPosterVertices[] = {
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

         0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.15f, 0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.65f,
         0.25f, 0.25f, 0.0f, 0.0f, 0.0f, 1.0f,  0.75f, 0.75f,

         0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         0.25f, 0.25f, 0.0f,  0.0f, 0.0f, 1.0f,  0.75f, 0.75f,
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
         0.25f, 0.25f, 0.0f,  0.0f, 0.0f, 1.0f,  0.75f, 0.75f,
         0.15f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.65f, 1.0f
    };

    glGenVertexArrays(1, &tornPosterVAO);
    glGenBuffers(1, &tornPosterVBO);
    glBindVertexArray(tornPosterVAO);
    glBindBuffer(GL_ARRAY_BUFFER, tornPosterVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tornPosterVertices), tornPosterVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    std::vector<float> cylVerts;
    int segments = 24;
    for (int i = 0; i < segments; ++i) {
        float theta1 = (2.0f * 3.1415926f * i) / segments;
        float theta2 = (2.0f * 3.1415926f * (i + 1)) / segments;
        float x1 = 0.5f * cos(theta1), z1 = 0.5f * sin(theta1);
        float x2 = 0.5f * cos(theta2), z2 = 0.5f * sin(theta2);

        cylVerts.insert(cylVerts.end(), {x1, -0.5f, z1, x1, 0.0f, z1, (float)i/segments, 0.0f});
        cylVerts.insert(cylVerts.end(), {x2, -0.5f, z2, x2, 0.0f, z2, (float)(i+1)/segments, 0.0f});
        cylVerts.insert(cylVerts.end(), {x2,  0.5f, z2, x2, 0.0f, z2, (float)(i+1)/segments, 1.0f});

        cylVerts.insert(cylVerts.end(), {x2,  0.5f, z2, x2, 0.0f, z2, (float)(i+1)/segments, 1.0f});
        cylVerts.insert(cylVerts.end(), {x1,  0.5f, z1, x1, 0.0f, z1, (float)i/segments, 1.0f});
        cylVerts.insert(cylVerts.end(), {x1, -0.5f, z1, x1, 0.0f, z1, (float)i/segments, 0.0f});

        cylVerts.insert(cylVerts.end(), {0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f});
        cylVerts.insert(cylVerts.end(), {x1,   0.5f, z1,   0.0f, 1.0f, 0.0f, 0.0f, 0.0f});
        cylVerts.insert(cylVerts.end(), {x2,   0.5f, z2,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f});
    }
    cylinderVertexCount = static_cast<int>(cylVerts.size() / 8);

    glGenVertexArrays(1, &cylinderVAO);
    glGenBuffers(1, &cylinderVBO);
    glBindVertexArray(cylinderVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
    glBufferData(GL_ARRAY_BUFFER, cylVerts.size() * sizeof(float), cylVerts.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

unsigned int CardGame::createSolidTexture(glm::vec3 color) {
    unsigned int texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    unsigned char data[3] = {
        static_cast<unsigned char>(color.r * 255),
        static_cast<unsigned char>(color.g * 255),
        static_cast<unsigned char>(color.b * 255)
    };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    return texID;
}

unsigned int CardGame::loadTexture(const char* path, bool fallbackColored, glm::vec3 fallbackColor) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RGB;
        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else if (nrComponents == 4) format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Warning: Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        glDeleteTextures(1, &textureID);
        return createSolidTexture(fallbackColor);
    }
    return textureID;
}

void CardGame::initTextures() {
    texDefaultWhite = createSolidTexture(glm::vec3(1.0f, 1.0f, 1.0f));
    texWoodFloor    = loadTexture("./textures/wood_floor.jpg", true, glm::vec3(0.45f, 0.28f, 0.15f));
    texWallpaper    = loadTexture("./textures/wallpaper.jpg", true, glm::vec3(0.75f, 0.72f, 0.65f));
    texTableTop     = loadTexture("./textures/table.jpg", true, glm::vec3(0.25f, 0.16f, 0.10f));
    texPoster       = loadTexture("./textures/breaking_bad_poster.jpg", true, glm::vec3(0.15f, 0.45f, 0.20f));
    cardBackTex     = loadTexture("./textures/card_back.jpg", true, glm::vec3(0.8f, 0.2f, 0.2f));

    cardTextures[0] = loadTexture("./textures/walter.jpg", true, glm::vec3(0.1f, 0.5f, 0.3f));
    cardTextures[1] = loadTexture("./textures/jesse.jpg",  true, glm::vec3(0.8f, 0.6f, 0.1f));
    cardTextures[2] = loadTexture("./textures/hank.jpg",   true, glm::vec3(0.6f, 0.4f, 0.2f));
    cardTextures[3] = loadTexture("./textures/gus.jpg",    true, glm::vec3(0.2f, 0.3f, 0.6f));
    cardTextures[4] = loadTexture("./textures/saul.jpg",   true, glm::vec3(0.7f, 0.2f, 0.2f));
}

void CardGame::initCards() {
    cards.clear();
    for (int i = 0; i < 5; ++i) {
        float xPos = (i - 2) * 0.58f;
        glm::vec3 startPos(xPos, 1.01f, 0.65f);
        cards.push_back(new Card(cardTextures[i], cardBackTex, startPos));
    }
}

void CardGame::setCameraMode(int mode) {
    cameraMode = mode;
    isTransitioning = true;

    if (cameraMode == 1) {
        targetCamPos = glm::vec3(0.0f, 3.6f, 0.25f);
        targetYaw    = -90.0f;
        targetPitch  = -85.0f;

        for (int i = 0; i < 5; i++) {
            if (!cards[i]->isPlayed()) {
                float xPos = (i - 2) * 0.58f;
                cards[i]->setTarget(glm::vec3(xPos, 1.01f, 0.65f), -90.0f);
            }
        }
    } 
    else if (cameraMode == 2) {
        targetCamPos = glm::vec3(0.0f, 2.15f, 2.35f);
        targetYaw    = -90.0f;
        targetPitch  = -22.0f;

        for (int i = 0; i < 5; i++) {
            if (!cards[i]->isPlayed()) {
                float xPos = (i - 2) * 0.52f;
                cards[i]->setTarget(glm::vec3(xPos, 1.38f, 0.85f), -18.0f);
            }
        }
    }
}

void CardGame::processMouseMovement(float xoffset, float yoffset, float sensitivity) {
    isTransitioning = false;
    currentYaw   += xoffset * sensitivity;
    currentPitch += yoffset * sensitivity;

    if (currentPitch > 89.0f)  currentPitch = 89.0f;
    if (currentPitch < -89.0f) currentPitch = -89.0f;
}

void CardGame::processKeyboardMovement(glm::vec3 direction, float deltaTime) {
    isTransitioning = false;
    float speed = 3.5f * deltaTime;
    glm::vec3 front = getCameraFront();
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

    currentCamPos += direction.z * front * speed;
    currentCamPos += direction.x * right * speed;

    currentCamPos.x = glm::clamp(currentCamPos.x, -6.2f, 6.2f);
    currentCamPos.z = glm::clamp(currentCamPos.z, -6.2f, 6.2f);
    currentCamPos.y = glm::clamp(currentCamPos.y, 0.4f, 5.4f);
}

void CardGame::playRandomCard() {
    std::vector<int> availableIndices;
    for (int i = 0; i < 5; i++) {
        if (!cards[i]->isPlayed()) availableIndices.push_back(i);
    }
    if (availableIndices.empty()) return;

    int randIdx = availableIndices[std::rand() % availableIndices.size()];
    float playX = (playedCount - 2) * 0.58f;
    glm::vec3 playAreaPos(playX, 1.01f + (playedCount * 0.001f), 0.0f);

    cards[randIdx]->markAsPlayed(playAreaPos);
    playedCount++;
}

void CardGame::update(float deltaTime) {
    if (isTransitioning) {
        float camSpeed = 5.0f * deltaTime;
        if (camSpeed > 1.0f) camSpeed = 1.0f;

        currentCamPos = glm::mix(currentCamPos, targetCamPos, camSpeed);
        currentYaw    = glm::mix(currentYaw, targetYaw, camSpeed);
        currentPitch  = glm::mix(currentPitch, targetPitch, camSpeed);

        if (glm::distance(currentCamPos, targetCamPos) < 0.01f) {
            isTransitioning = false;
        }
    }

    for (auto card : cards) {
        card->update(deltaTime);
    }
}

glm::vec3 CardGame::getCameraFront() const {
    glm::vec3 front;
    front.x = cos(glm::radians(currentYaw)) * cos(glm::radians(currentPitch));
    front.y = sin(glm::radians(currentPitch));
    front.z = sin(glm::radians(currentYaw)) * cos(glm::radians(currentPitch));
    return glm::normalize(front);
}

glm::mat4 CardGame::getViewMatrix() const {
    glm::vec3 front = getCameraFront();
    return glm::lookAt(currentCamPos, currentCamPos + front, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 CardGame::getCameraPos() const {
    return currentCamPos;
}

void CardGame::drawRoom(Shader& shader) {
    glBindVertexArray(cubeVAO);

    glBindTexture(GL_TEXTURE_2D, texWoodFloor);
    shader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.05f, 0.0f));
    model = glm::scale(model, glm::vec3(14.0f, 0.1f, 14.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindTexture(GL_TEXTURE_2D, texWallpaper);
    shader.setVec3("objectColor", 0.92f, 0.90f, 0.85f);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 3.5f, -7.0f));
    model = glm::scale(model, glm::vec3(14.0f, 7.0f, 0.2f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 3.5f, 7.0f));
    model = glm::scale(model, glm::vec3(14.0f, 7.0f, 0.2f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-7.0f, 3.5f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 7.0f, 14.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(7.0f, 3.5f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 7.0f, 14.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(tornPosterVAO);
    glBindTexture(GL_TEXTURE_2D, texPoster);
    shader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.8f, 3.2f, -6.88f));
    model = glm::rotate(model, glm::radians(3.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(1.8f, 2.5f, 1.0f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 12);
}

void CardGame::drawTable(Shader& shader) {
    glBindVertexArray(cubeVAO);
    glBindTexture(GL_TEXTURE_2D, texTableTop);
    shader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.95f, 0.0f));
    model = glm::scale(model, glm::vec3(3.6f, 0.1f, 2.4f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    float legX[2] = {-1.6f, 1.6f};
    float legZ[2] = {-1.0f, 1.0f};
    shader.setVec3("objectColor", 0.6f, 0.4f, 0.25f);
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(legX[i], 0.45f, legZ[j]));
            model = glm::scale(model, glm::vec3(0.15f, 0.9f, 0.15f));
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }
}

void CardGame::drawDecorations(Shader& shader) {
    glBindTexture(GL_TEXTURE_2D, texDefaultWhite);
    glm::mat4 model;

    // 1. البرميل الكيميائي الأزرق في زاوية الغرفة اليسرى
    glBindVertexArray(cylinderVAO);
    shader.setVec3("objectColor", 0.08f, 0.35f, 0.75f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-4.5f, 0.75f, -4.5f));
    model = glm::scale(model, glm::vec3(1.1f, 1.5f, 1.1f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, cylinderVertexCount);

    shader.setVec3("objectColor", 0.2f, 0.2f, 0.22f);
    for (float yRing : {0.4f, 1.1f}) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-4.5f, yRing, -4.5f));
        model = glm::scale(model, glm::vec3(1.14f, 0.08f, 1.14f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, cylinderVertexCount);
    }

    // 2. قبعة هايزنبرغ السوداء على الطرف الأيسر من الطاولة الرئيسية (تم جعلها دائرية باستخدام cylinderVAO)
    glBindVertexArray(cylinderVAO);
    
    // أ. حافة القبعة (Brim) - أسطوانة مفلطحة وعريضة
    shader.setVec3("objectColor", 0.08f, 0.08f, 0.08f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.35f, 1.02f, -0.85f));
    model = glm::scale(model, glm::vec3(0.9f, 0.03f, 0.9f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, cylinderVertexCount);

    // ب. تاج القبعة (Crown) - أسطوانة دائرية فوق الحافة
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.35f, 1.18f, -0.85f));
    model = glm::scale(model, glm::vec3(0.55f, 0.28f, 0.55f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, cylinderVertexCount);

    // ج. شريط القبعة (Hat Band) - شريط دائر حول التاج
    shader.setVec3("objectColor", 0.35f, 0.35f, 0.35f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.35f, 1.08f, -0.85f));
    model = glm::scale(model, glm::vec3(0.57f, 0.04f, 0.57f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, cylinderVertexCount);

    // --- 3. الطاولة الصغيرة في الزاوية اليمنى مع القطارة فوقها ---
    glBindVertexArray(cubeVAO);
    
    // أ. سطح الطاولة الصغيرة في الزاوية اليمنى
    shader.setVec3("objectColor", 0.32f, 0.18f, 0.09f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.2f, 0.85f, -4.5f));
    model = glm::scale(model, glm::vec3(1.8f, 0.1f, 1.3f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // ب. أرجل الطاولة الصغيرة الأربعة
    shader.setVec3("objectColor", 0.2f, 0.1f, 0.05f);
    float smallTableLegX[2] = {3.4f, 5.0f};
    float smallTableLegZ[2] = {-5.0f, -4.0f};
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(smallTableLegX[i], 0.4f, smallTableLegZ[j]));
            model = glm::scale(model, glm::vec3(0.12f, 0.8f, 0.12f));
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    // ج. القطارة الكيميائية فوق الطاولة الصغيرة في الزاوية اليمنى
    shader.setVec3("objectColor", 0.1f, 0.75f, 0.95f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.2f, 1.07f, -4.5f));
    model = glm::scale(model, glm::vec3(0.35f, 0.3f, 0.35f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    shader.setVec3("objectColor", 0.85f, 0.92f, 0.95f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.2f, 1.32f, -4.5f));
    model = glm::scale(model, glm::vec3(0.12f, 0.25f, 0.12f));
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void CardGame::drawCards(Shader& shader) {
    for (auto card : cards) {
        card->draw(shader);
    }
}

void CardGame::draw(Shader& shader) {
    drawRoom(shader);
    drawTable(shader);
    drawDecorations(shader);
    drawCards(shader);
}