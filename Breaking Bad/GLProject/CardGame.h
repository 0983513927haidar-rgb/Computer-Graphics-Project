#ifndef CARDGAME_H
#define CARDGAME_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <learnopengl/shader_m.h>
#include <vector>
#include <string>
#include "Card.h"

class CardGame {
public:
    CardGame();
    ~CardGame();

    void setCameraMode(int mode);
    void processMouseMovement(float xoffset, float yoffset, float sensitivity = 0.12f); // دعم الحساسية وتحديث الماوس
    void processKeyboardMovement(glm::vec3 direction, float deltaTime); // دالة حركة الكيبورد (WASD)
    void playRandomCard();
    void update(float deltaTime);
    void draw(Shader& shader);
    glm::mat4 getViewMatrix() const;
    glm::vec3 getCameraPos() const;

private:
    unsigned int cubeVAO, cubeVBO;
    unsigned int quadVAO, quadVBO;
    unsigned int tornPosterVAO, tornPosterVBO;
    unsigned int cylinderVAO, cylinderVBO;
    int cylinderVertexCount;

    unsigned int texWoodFloor;
    unsigned int texWallpaper;
    unsigned int texTableTop;
    unsigned int texPoster;
    unsigned int texDefaultWhite;
    unsigned int cardBackTex;       // خامة وجه البطاقة الخلفي
    unsigned int cardTextures[5];   // مصفوفة صور شخصيات بريكنغ باد

    int cameraMode; // 1 = Top-Down, 2 = Player View
    int playedCount;

    // إحداثيات الكاميرا والتوجيه (Yaw, Pitch) والانتقال السلس
    glm::vec3 currentCamPos, targetCamPos;
    float currentYaw, targetYaw;
    float currentPitch, targetPitch;
    bool isTransitioning;

    // استخدام كلاس Card الجديد بدلاً من الهيكل البسيط
    std::vector<Card*> cards;

    void initGeometry();
    void initTextures();
    void initCards();
    unsigned int loadTexture(const char* path, bool fallbackColored = false, glm::vec3 fallbackColor = glm::vec3(1.0f));
    unsigned int createSolidTexture(glm::vec3 color);

    void drawRoom(Shader& shader);
    void drawTable(Shader& shader);
    void drawDecorations(Shader& shader);
    void drawCards(Shader& shader);
    
    glm::vec3 getCameraFront() const;
};

#endif