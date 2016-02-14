//
// Created by johan on 2015-12-12.
//

#include <ResourceManager.h>
#include <Utils.h>
#include <ControlsManager.h>
#include <Controls.h>
#include <View.h>
#include <GLSquare.h>
#include "HudRenderer.h"

HudRenderer::HudRenderer(int *scoreBoard, State *state){
    this->state = state;
    this->scoreBoard = scoreBoard;

    ResourceManager::loadShader("../shaders/hud.vert", "../shaders/hud.frag", "hudShader");
    shaderProgram = ResourceManager::getShader("hudShader");
    conf = new HudConfig;

    glGenVertexArrays(1, &m_vaob);

    /*GLfloat quad[] = { //POSITION3 TEXCOORD2 WTF, magic array?
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

            0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    };

    glGenVertexArrays(1, &m_vaob);
    glBindVertexArray(m_vaob);

    GLuint pos_vbo;
    glGenBuffers(1, &pos_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    // CLEANUP
    glBindVertexArray(0);
    CHECK_GL_ERROR();*/
}
/*
void HudRenderer::renderView(View view){
    //GLfloat vertices[view.getSize()*5];
}
*/
HudRenderer::~HudRenderer(){
    delete conf;
}

void HudRenderer::renderNum(int n) {

    std::string texturePath = "../scenes/HUD/num";
    texturePath.append(std::to_string(n));
    texturePath.append(".png");

    Texture* texture = ResourceManager::loadAndFetchTexture(texturePath);
    GLSquare(-0.9f,0.7f,0.2f,0.2f,texture).render(shaderProgram,&m_vaob);
}

struct HudRenderer::HudConfig* HudRenderer::getConfig() {
    return conf;
}

void HudRenderer::render() {
    float4x4 modelMat;

    ControlsManager *cm = ControlsManager::getInstance();

    if (*state == Start) {
        modelMat = make_translation(make_vector(-.5f, -.5f, 0.0f));
        Texture *texture = ResourceManager::loadAndFetchTexture("../scenes/HUD/mission_box.png");
        GLSquare(-0.5f,-0.5f,1.0f,1.0f,texture).render(shaderProgram,&m_vaob);

        if (cm->getStatus(CONTINUE).isActive()) {
            *state = Playing;
        }
    }


    int score = *scoreBoard;


    if (score >= 50 && *state != Credits) {
        modelMat = make_translation(make_vector(-.5f, -.5f, 0.0f));
        Texture *texture = ResourceManager::loadAndFetchTexture("../scenes/HUD/win_box.png");
        GLSquare(-0.5f,-0.5f,1.0f,1.0f,texture).render(shaderProgram,&m_vaob);

        if (cm->getStatus(CONTINUE).isActive()) {
            *state = Credits;
        }

    } else if(*state == Credits) {
        modelMat = make_translation(make_vector(-.5f, -.5f, 0.0f));
        Texture *texture = ResourceManager::loadAndFetchTexture("../scenes/HUD/credit_box.png");
        GLSquare(-0.5f,-0.5f,1.0f,1.0f,texture).render(shaderProgram,&m_vaob);
    }else if(*state == Died) {
        modelMat = make_translation(make_vector(-.5f, -.5f, 0.0f));
        Texture *texture = ResourceManager::loadAndFetchTexture("../scenes/HUD/fail_box.png");
        GLSquare(-0.5f,-0.5f,1.0f,1.0f,texture).render(shaderProgram,&m_vaob);
    }


    modelMat = make_translation(make_vector(-.5f, -.9f, 0.0f)) * make_scale<float4x4>(make_vector(0.2f, 0.2f, 0.5f));


    if (score == 0) {
       renderNum(0);
    } else {
        while (score > 0) {
            int n = score % 10;
            renderNum(n);
            modelMat = make_translation(make_vector(-0.1f, 0.0f, 0.0f)) * modelMat;
            score /= 10;
        }
    }


    modelMat = make_translation(make_vector(0.5f, -0.9f, 0.0f)) * make_scale<float4x4>(make_vector(0.4f, 0.4f, 0.5f));
    Texture* texture = ResourceManager::loadAndFetchTexture("../scenes/HUD/meter2.0.png");
    GLSquare(0.8f,0.8f,0.2f,0.2f,texture).render(shaderProgram,&m_vaob);

    int degrees = ((int)round(conf->speed*(220.0f/100.0f)))+80;
    modelMat = make_translation(make_vector(0.70f, -0.71f, 0.0f)) * make_rotation_z<float4x4>((float) (M_PI / 180 * degrees)) * make_translation(make_vector(-0.01f, -0.18f, 1.0f)) * make_scale<float4x4>(make_vector(0.02f, 0.2f, 1.0f));

    Texture* texture1 = ResourceManager::loadAndFetchTexture("../scenes/HUD/arrow.png");
    GLSquare(0.89f,0.825f,0.02f,0.125f,texture1).render(shaderProgram,&m_vaob);
}

void HudRenderer::render2DHud(Texture* texture, float4x4 *modelMatrix) {
    GLSquare square = GLSquare(-1.0f,-1.0f,2.0f,2.0f,texture);
    square.render(shaderProgram,&m_vaob);
    /*GLint currentDepthFunc;
    glGetIntegerv(GL_DEPTH_FUNC, &currentDepthFunc);

    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaderProgram->backupCurrentShaderProgram();
    shaderProgram->use();
    glBindVertexArray(m_vaob);

    shaderProgram->setUniform1i("sprite", 0);
    shaderProgram->setUniformMatrix4fv("modelMatrix", *modelMatrix);

    texture->bind(GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    CHECK_GL_ERROR();

    glDepthFunc(currentDepthFunc);
    glDisable(GL_BLEND);

    glEnable(GL_CULL_FACE);
    shaderProgram->restorePreviousShaderProgram();*/
}


void HudRenderer::renderShadow(Shader *shaderProgram) {}

void HudRenderer::update(float dt){

}