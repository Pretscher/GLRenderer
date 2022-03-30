#pragma once
#include "EventManager.hpp"
#include <iostream> using namespace std;
#include "Shader.hpp"
#include "Cube.hpp"
#include "Model.hpp"
#include "Light.hpp"

//cube of cubes

unsigned int containerTex, smileyTex, specularMap;
int cubeCount = 100;
vector<Cube> cubes;

//lights handling
vector<PointLight> pLights;
vector<DirectionalLight> dLights;
vector<SpotLight> sLights;
//spinning moons

vector<Model> moons;
shared_ptr<Shader> singleColorShader;

Model* backpack;

Model* earth; Model* sun; Model* moon;

bool renderCubeOfCubes = false;
bool renderSpinningMoons = false;
bool renderFlashLight = false;
bool renderBackPack = false;
bool renderSolarSystem = true;

void EventManager::initRotatingLights() {
    float brightness = 1.0f;
    vec3 red = { brightness, 0.0f, 0.0f }, green = { 0.0f, brightness, 0.0f }, blue = { 0.0f, 0.0f, brightness }, white = { brightness, brightness, brightness };
    vec3 a = { brightness, brightness, 0.0f }, b = { 0.0f, brightness, brightness }, c = { brightness, 0.0f, brightness }, d = { brightness, brightness / 2, brightness };
    moons.push_back(Model("C:/Users/Julian/source/repos/2022/GLRenderer/Models/moon/moon.obj", renderer->getDefaultShader()));
    moons[0].influencedByLighting = false;
    for (int i = 0; i < 7; i++) {
        moons.push_back(Model(moons[0]));
    }
    int dist = 50;
    pLights.push_back(PointLight(dist, red));
    moons[0].setBaseColor(red);
    pLights.push_back(PointLight(dist, green));
    moons[1].setBaseColor(green);
    pLights.push_back(PointLight(dist, blue));
    moons[2].setBaseColor(blue);
    pLights.push_back(PointLight(dist, white));
    moons[3].setBaseColor(white);

    pLights.push_back(PointLight(dist, a));
    moons[4].setBaseColor(a);
    pLights.push_back(PointLight(dist, b));
    moons[5].setBaseColor(b);
    pLights.push_back(PointLight(dist, c));
    moons[6].setBaseColor(c);
    pLights.push_back(PointLight(dist, d));
    moons[7].setBaseColor(d);

   // for (int i = 0; i < pLights.size(); i++) {
       // pLights[i].setIntensity(0.0f, 0.2f, 0.1f);
   // }
}

//loop funcs---------------------------------------------------------------------------------------------------------------------------------------------

void EventManager::drawCubeOfCubes() {
    int lenght = (int)cbrt(cubeCount);
    float cubeSize = 1.0f;
    for (int x = 0; x < lenght; x++) {
        for (int y = 0; y < lenght; y++) {
            for (int z = 0; z < lenght; z++) {
                int index = (lenght * lenght * x) + (lenght * y) + z;
                //cubes[index].rotate(glfwGetTime() * 100, 1.0f, 1.0f, 1.0f);
                cubes[index].scale(cubeSize, cubeSize, cubeSize);
                cubes[index].translate((float)x - ((cubeSize * lenght) / 2),
                    (float)y - ((cubeSize * lenght) / 2), (float)z - ((cubeSize * lenght) / 2));
                cubes[index].updateAndDraw();
            }
        }
    }
}

#define _USE_MATH_DEFINES
#include <math.h>

void EventManager::updateRotatingLights() {
    const float radius = 3.0f;
    const int speed = 5;

    float lightX = (float)sin((float)glfwGetTime() * speed) * radius;
    float lightZ = (float)cos((float)glfwGetTime() * speed) * radius;
    //float lightY = (float)cos((float)glfwGetTime() * 5) * radius;

    float lightX2 = (float)sin(M_PI / 4 + ((float)glfwGetTime() * speed)) * radius;
    float lightZ2 = (float)cos(M_PI / 4 + ((float)glfwGetTime() * speed)) * radius;

    float lightX3 = (float)sin(M_PI / 2 + ((float)glfwGetTime() * speed)) * radius;
    float lightZ3 = (float)cos(M_PI / 2 + ((float)glfwGetTime() * speed)) * radius;

    float lightX4 = (float)sin(3 * M_PI / 4 + ((float)glfwGetTime() * speed)) * radius;
    float lightZ4 = (float)cos(3 * M_PI / 4 + ((float)glfwGetTime() * speed)) * radius;

    pLights[0].translate(lightX, 0.0f, lightZ);
    moons[0].translate(pLights[0].position);
    pLights[1].translate(-lightX, 0.0f, -lightZ);
    moons[1].translate(pLights[1].position);


    pLights[2].translate(lightX2, 0.0f, lightZ2);
    moons[2].translate(pLights[2].position);
    pLights[3].translate(-lightX2, 0.0f, -lightZ2);
    moons[3].translate(pLights[3].position);

    pLights[4].translate(lightX3, 0.0f, lightZ3);
    moons[4].translate(pLights[4].position);
    pLights[5].translate(-lightX3, 0.0f, -lightZ3);
    moons[5].translate(pLights[5].position);

    pLights[6].translate(lightX4, 0.0f, lightZ4);
    moons[6].translate(pLights[6].position);
    pLights[7].translate(-lightX4, 0.0f, -lightZ4);
    moons[7].translate(pLights[7].position);

    for (int i = 0; i < moons.size(); i++) {
        moons[i].scale(0.2f, 0.2f, 0.2f);
        moons[i].translate(earth->getPosition());
        pLights[i].translate(earth->getPosition());
        moons[i].updateAndDraw();
    }
}

void EventManager::drawLights() {
    const float radius = 3.0f;
    const int speed = 5;
    float lightX = (float)sin((float)glfwGetTime() * speed) * radius;
    float lightZ = (float)cos((float)glfwGetTime() * speed) * radius;
    
    if (renderSpinningMoons == true) {
        updateRotatingLights();
    }
    if (renderFlashLight == true) {
        sLights[0].translate(cam->getPos());
        sLights[0].setDirection(cam->getDirection());
    }

    renderer->updateLights(pLights, dLights, sLights, cam->getPos());//passes light positions to all shaders
}

void EventManager::initLights() {
    if (renderSpinningMoons == true) {
        initRotatingLights();
    }
    if (renderFlashLight == true) {
        sLights.push_back(SpotLight(5.0f, 10.0f, { 1.0f, 1.0f, 1.0f }));
        sLights[0].setIntensity(0.0f, 0.5f, 1.0f);
    }

    //directional Light
    //dLights.push_back(DirectionalLight({ 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }));
    //dLights[0].setIntensity(0.0f, 1.0f, 2.0f);
}

void EventManager::initSolarSystem() {
    earth = new Model("C:/Users/Julian/source/repos/2022/GLRenderer/Models/earth/earth.obj", renderer->getDefaultShader());
    earth->setShininess(6.0f);
    earth->setLightingSensitivity(1.0f, 0.5f);
    sun = new Model("C:/Users/Julian/source/repos/2022/GLRenderer/Models/sun/sun.obj", renderer->getDefaultShader());
    sun->influencedByLighting = false;
    sun->setBaseColor({ 3.0f, 3.0f, 3.0f });
    pLights.push_back(PointLight(1000000, { 1.0f, 1.0f, 1.0f }));//sun light stays at origin
    pLights[0].setIntensity(0.1f, 1.0f, 1.2f);
    moon = new Model("C:/Users/Julian/source/repos/2022/GLRenderer/Models/moon/moon.obj", renderer->getDefaultShader());
   // moon->setLightingSensitivity(1.0f, 0.0f);
    moon->setShininess(4.0f);
    moon->setLightingSensitivity(1.0f, 0.5f);

    containerTex = Renderer::loadTexture("Textures/container2.png", false, true);
    specularMap = Renderer::loadTexture("Textures/container2_specular.png", false, true);
}

void EventManager::updateSolarSystem() {
    earth->rotate(180.0f, 0.0f, 0.0f, 1.0f);//model is flipped horizontally from globe world view
    earth->rotate(glfwGetTime() * 20, 0.0f, 1.0f, 0.0f);

    const float radius = 10.0f;
    const float speed = 0.3f;
    float xRot = (float)sin((float)glfwGetTime() * speed) * radius;
    float zRot = (float)cos((float)glfwGetTime() * speed) * radius;
    earth->translate(xRot, 0.0f, zRot);

    const float radius2 = 2.0f;
    const float speed2 = 1.0f;
    float xRot2 = (float)sin((float)glfwGetTime() * speed2) * radius2;
    float yRot2 = (float)cos((float)glfwGetTime() * speed2) * radius2;
    float zRot2 = (float)cos((float)glfwGetTime() * speed2) * radius2;
    moon->translate(xRot2, 0.0f, zRot2);
    moon->translate(earth->getPosition());//move to earth
    moon->scale(0.3f, 0.3f, 0.3f);


    sun->rotate(glfwGetTime() * 20, 0.0f, 1.0f, 0.0f);
    earth->updateAndDraw();
    moon->updateAndDraw();
    sun->updateAndDraw();
}

//----------------------------------------------------------------------------------------------------------------------------------------------
unsigned int cubemapTexture;
shared_ptr<Shader> skyboxShader;
//init all the stuff needed before drawing (renderer and camera already fully available)
void EventManager::initEvents() {
      initLights();

      if (renderCubeOfCubes == true) {
          containerTex = Renderer::loadTexture("Textures/container2.png", false, true);
          specularMap = Renderer::loadTexture("Textures/container2_specular.png", false, true);
        //  smileyTex = Renderer::loadTexture("Textures/awesomeface.png", true, true);
          for (int i = 0; i < cubeCount; i++) {
              cubes.push_back(Cube({ containerTex }, { specularMap }, renderer->getDefaultShader()));
          }
      }
      if (renderBackPack == true) {
          backpack = new Model("C:/Users/Julian/source/repos/2022/GLRenderer/Models/backpack/backpack.obj", renderer->getDefaultShader());
      }
      if (renderSolarSystem == true) {
          initSolarSystem();
      }

      skyboxShader = renderer->createShader("SkyboxVertShader.vert", "SkyboxFragShader.frag", true, false);
      string directory("C:/Users/Julian/source/repos/2022/GLRenderer/Textures/skyboxes_space/blue/");
      vector<std::string> faces {
              directory + "right.png",
              directory + "left.png",
              directory + "top.png",
              directory + "bottom.png",
              directory + "front.png",
              directory + "back.png"
      };
      cubemapTexture = renderer->loadCubeMap(faces);
}

//called from drawingLoop in Framework every frame
void EventManager::eventloop() {
    drawCubeMap();
    if (renderSolarSystem == true) {
        updateSolarSystem();
    }
    drawLights();
    if (renderCubeOfCubes == true) {
        drawCubeOfCubes();
    }
    if (renderBackPack == true) {
        backpack->translate(0.0f, 5.0f, 0.0f);
        backpack->updateAndDraw();
    }
   
}







void EventManager::drawCubeMap() {
    glDepthMask(GL_FALSE);
    skyboxShader->use();
    skyboxShader->setMat4("model", mat4(1.0f));
    //remove translation(camera momevent should have no effect on cubemap, only camra rotation, so we have to ignore the translation part
    //of the matrix (last colum, can be done by casting to mat3))
    glm::mat4 view = glm::mat4(glm::mat3(*renderer->globalView));
    skyboxShader->setMat4("view", view);

    glBindVertexArray(Cube::getCubeVAO());
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}