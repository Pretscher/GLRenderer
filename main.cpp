#include "Framework.hpp"
#include "Light.hpp"
//OpenGL groundwork + camera and renderer init-----------------------------------------------------------------------------------------------------------

Renderer* renderer;
Camera* cam;
void initEvents();
void eventloop();
int main() {
    srand(static_cast <unsigned> (time(0)));//seed random at the start of the program so we dont forget

    FrameWork frameWork;//inits opengl, camera and renderer
    renderer = frameWork.renderer;//i really cant be bothered to type framework.renderer and framework.cam all the time
    cam = frameWork.cam;
    initEvents();
    //draws till the end. 
    frameWork.startDrawingLoop(eventloop);//drawingLoop calls eventloop every frame
    //good practise: free up all memory at the end of the program
    delete renderer;
    delete cam;
    return 0;
}

//Init funcs---------------------------------------------------------------------------------------------------------------------------------------------

unsigned int containerTex, smileyTex, specularMap;
int cubeCount = 100;
vector<Cube> cubes;

vector<PointLight> pLights;
vector<Model> suns;

vector<DirectionalLight> dLights;
vector<SpotLight> sLights;

shared_ptr<Shader> singleColorShader;

//Model* sun;
Model* backpack;
Model* sun;
void initRotatingLights() {
    float brightness = 10.0f;
    vec3 red = { brightness, 0.0f, 0.0f }, green = { 0.0f, brightness, 0.0f }, blue = { 0.0f, 0.0f, brightness }, white = { brightness, brightness, brightness };
    vec3 a = { brightness, brightness, 0.0f }, b = { 0.0f, brightness, brightness }, c = { brightness, 0.0f, brightness }, d = { brightness, brightness / 2, brightness };
    suns.push_back(Model("C:/Users/Julian/source/repos/2022/GLRenderer/Models/moon/moon.obj", renderer->getDefaultShader()));
    suns[0].setLightingSensitivity(0.1f, 0.0f);
    suns[0].influencedByLighting = false;
    for (int i = 0; i < 7; i++) {
        suns.push_back(Model(suns[0]));
        suns[i].setShininess(16);
    }
    int dist = 40;
    pLights.push_back(PointLight(dist, red));
    suns[0].setBaseColor(red);
    pLights.push_back(PointLight(dist, green));
    suns[1].setBaseColor(green);
    pLights.push_back(PointLight(dist, blue));
    suns[2].setBaseColor(blue);
    pLights.push_back(PointLight(dist, white));
    suns[3].setBaseColor(white);

    pLights.push_back(PointLight(dist, a));
    suns[4].setBaseColor(a);
    pLights.push_back(PointLight(dist, b));
    suns[5].setBaseColor(b);
    pLights.push_back(PointLight(dist, c));
    suns[6].setBaseColor(c);
    pLights.push_back(PointLight(dist, d));
    suns[7].setBaseColor(d);

    for (int i = 0; i < pLights.size(); i++) {
        pLights[i].setIntensity(0.0f, 0.2f, 0.1f);
    }
}

//init all the stuff needed before drawing (renderer and camera already fully available)
void initEvents() {
    initRotatingLights();
 //   dLights.push_back(DirectionalLight({ 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }));
  //  dLights[0].setIntensity(0.0f, 1.0f, 2.0f);

   // sLights.push_back(SpotLight(5.0f, 10.0f, { 1.0f, 1.0f, 1.0f }));
    //sLights[0].setIntensity(0.0f, 0.5f, 1.0f);
    
    containerTex = Renderer::loadTexture("Textures/container2.png", false, true);
    specularMap = Renderer::loadTexture("Textures/container2_specular.png", false, true);
    smileyTex = Renderer::loadTexture("Textures/awesomeface.png", true, true);
    for (int i = 0; i < cubeCount; i++) {
        cubes.push_back(Cube({ containerTex }, { specularMap }, renderer->getDefaultShader()));
    }
 //   backpack = new Model("C:/Users/Julian/source/repos/2022/GLRenderer/Models/backpack/backpack.obj", renderer->getDefaultShader());
    sun = new Model("C:/Users/Julian/source/repos/2022/GLRenderer/Models/earth/earth.obj", renderer->getDefaultShader());
}

//loop funcs---------------------------------------------------------------------------------------------------------------------------------------------



void drawCubeOfCubes() {
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

void updateRotatingLights() {
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


    float lightWidth = 0.2f, lightHeight = 0.2f, lightDepth = 0.2f;

    pLights[0].setPosition(lightX, -lightHeight / 2, lightZ);
    suns[0].translate(lightX, -lightHeight / 2, lightZ);
    pLights[1].setPosition(-lightX, -lightHeight / 2, -lightZ);
    suns[1].translate(-lightX, -lightHeight / 2, -lightZ);


    pLights[2].setPosition(lightX2, 0.0f, lightZ2);
    suns[2].translate(lightX2, 0.0f, lightZ2);
    pLights[3].setPosition(-lightX2, 0.0f, -lightZ2);
    suns[3].translate(-lightX2, 0.0f, -lightZ2);

    pLights[4].setPosition(lightX3, 0.0f, lightZ3);
    suns[4].translate(lightX3, 0.0f, lightZ3);
    pLights[5].setPosition(-lightX3, 0.0f, -lightZ3);
    suns[5].translate(-lightX3, 0.0f, -lightZ3);

    pLights[6].setPosition(lightX4, 0.0f, lightZ4);
    suns[6].translate(lightX4, 0.0f, lightZ4);
    pLights[7].setPosition(-lightX4, 0.0f, -lightZ4);
    suns[7].translate(-lightX4, 0.0f, -lightZ4);
}

void drawLight() {
    updateRotatingLights();
    //sLights[0].setPosition(cam->getPos());
    //sLights[0].setDirection(cam->getDirection());
    renderer->updateLights(pLights, dLights, sLights, cam->getPos());//passes light positions to all shaders
}

//called from drawingLoop in Framework every frame
void eventloop() {

    drawLight();
   // drawCubeOfCubes();
 //   backpack->translate(0.0f, 5.0f, 0.0f);
   // backpack->updateAndDraw();
    sun->updateAndDraw();
    sun->rotate(180, 0.0f, 0.0f, 1.0f);
    for (int i = 0; i < suns.size(); i++) {
        suns[i].scale(0.2f, 0.2f, 0.2f);
        suns[i].updateAndDraw();
    }
}