#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MeshSurface.h"

class Card {
private:
    MeshSurface* frontFace;
    MeshSurface* backFace;

    glm::vec3 currentPos;
    glm::vec3 targetPos;

    float currentPitch; // زاوية الميلان حول محور X (مسطحة = -90، واقفة = -15)
    float targetPitch;

    bool played;

public:
    Card(unsigned int frontTex, unsigned int backTex, glm::vec3 startPos)
        : currentPos(startPos), targetPos(startPos), currentPitch(-90.0f), targetPitch(-90.0f), played(false) {
        
        // أبعاد البطاقة الواحدة: عرض 0.45 وارتفاع 0.62
        frontFace = MeshSurface::createRectangle(0.45f, 0.62f, frontTex, glm::vec3(1.0f));
        backFace  = MeshSurface::createRectangle(0.45f, 0.62f, backTex,  glm::vec3(1.0f));
    }

    // تحديد الهدف الجديد للبطاقة (الموقع وزاوية الميلان)
    void setTarget(glm::vec3 pos, float pitchDegrees) {
        targetPos = pos;
        targetPitch = pitchDegrees;
    }

    void markAsPlayed(glm::vec3 playAreaPos) {
        played = true;
        // عند لعب البطاقة تستقر أفقياً (-90 درجة) في منطقة اللعب بالوسط
        setTarget(playAreaPos, -90.0f);
    }

    bool isPlayed() const {
        return played;
    }

    // تحريك البطاقة بنعومة نحو الهدف في كل إطار (Smooth Animation)
    void update(float deltaTime) {
        float speed = 6.0f * deltaTime;
        if (speed > 1.0f) speed = 1.0f;

        currentPos   = glm::mix(currentPos, targetPos, speed);
        currentPitch = glm::mix(currentPitch, targetPitch, speed);
    }

    void draw(Shader& shader) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, currentPos);
        model = glm::rotate(model, glm::radians(currentPitch), glm::vec3(1.0f, 0.0f, 0.0f));

        // رسم الوجه الأمامي
        frontFace->draw(shader, model);

        // رسم الوجه الخلفي (مدور 180 درجة حول Y ومزاح قليلاً للخلف لمنع التداخل)
        glm::mat4 backModel = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.002f));
        backModel = glm::rotate(backModel, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        backFace->draw(shader, backModel);
    }

    ~Card() {
        delete frontFace;
        delete backFace;
    }
};
