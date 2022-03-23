#include "Framework.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//OpenGL groundwork + camera and renderer init-----------------------------------------------------------------------------------------------------------

Assimp::Importer importer;
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
int cubeCount = 1000;
std::vector<Cube> cubes;
std::vector<PointLight> pLights;
std::vector<DirectionalLight> dLights;
std::vector<SpotLight> sLights;

void initRotatingLights() {
    //we create new shaders for every light because they have different colors set in the shader 
    //and i cant be bothered with vertex coloring rn
    pLights.push_back(PointLight({ 1.0f, 0.0f, 0.0f }, {}, {}, {}, renderer->createShader("LightingVertexShader.vert", "lightingFragmentShader.frag", true)));
    pLights.push_back(PointLight({ 0.0f, 1.0f, 0.0f }, {}, {}, {}, renderer->createShader("LightingVertexShader.vert", "lightingFragmentShader.frag", true)));
    pLights.push_back(PointLight({ 0.0f, 0.0f, 1.0f }, {}, {}, {}, renderer->createShader("LightingVertexShader.vert", "lightingFragmentShader.frag", true)));
    pLights.push_back(PointLight({ 1.0f, 1.0f, 0.0f }, {}, {}, {}, renderer->createShader("LightingVertexShader.vert", "lightingFragmentShader.frag", true)));
   // pLights.push_back(PointLight({ 0.5f, 0.5f, 1.0f }));
    //pLights.push_back(PointLight({ 1.0f, 1.0f, 0.5f }));
}

//init all the stuff needed before drawing (renderer and camera already fully available)
void initEvents() {
    initRotatingLights();
    //  dLights.push_back(DirectionalLight({ 0.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }));
   // dLights[0].setIntensity(0.1f, 5.0f, 100.0f);

  //  sLights.push_back(SpotLight(1.0f, 10.0f, { 1.0f, 1.0f, 1.0f }));
   // sLights[0].setIntensity(0.3f, 1.5f, 0.0f);

    containerTex = renderer->loadTexture("Textures/container2.png", false, true);
    specularMap = renderer->loadTexture("Textures/container2_specular.png", false, true);
    smileyTex = renderer->loadTexture("Textures/awesomeface.png", true, true);
    for (int i = 0; i < cubeCount; i++) {
        cubes.push_back(Cube({ containerTex }, { specularMap }, { 0.7f }, renderer->getDefaultShader()));
        float temp[3] = { 1.0f, 1.0f, 1.0f };
        cubes[i].setMaterial({ 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, 32);
    }
}

//loop funcs---------------------------------------------------------------------------------------------------------------------------------------------



void drawCubeOfCubes() {
    int lenght = (int)std::cbrt(cubeCount);
    float cubeSize = 1.0f;
    for (int x = 0; x < lenght; x++) {
        for (int y = 0; y < lenght; y++) {
            for (int z = 0; z < lenght; z++) {
                int index = (lenght * lenght * x) + (lenght * y) + z;
                //cubes[index].rotate(glfwGetTime() * 100, 1.0f, 1.0f, 1.0f);
                cubes[index].scale(cubeSize, cubeSize, cubeSize);
                cubes[index].translate((float)x - ((cubeSize * lenght) / 2),
                    (float)y - ((cubeSize * lenght) / 2), (float)z - ((cubeSize * lenght) / 2));
                cubes[index].draw();
            }
        }
    }
}

void updateRotatingLights() {
    const float radius = 10.0f;
    float lightX = (float)sin((float)glfwGetTime() * 3) * radius;
    float lightZ = (float)cos((float)glfwGetTime() * 3) * radius;
    float lightY = (float)cos((float)glfwGetTime() * 3) * radius;
    pLights[0].scale(0.5f, 0.1f, 0.5f);
    pLights[1].scale(0.5f, 0.1f, 0.5f);
    pLights[2].scale(0.5f, 0.1f, 0.5f);
    pLights[3].scale(0.5f, 0.1f, 0.5f);

    pLights[0].translate(lightX, 0.0f, lightZ);
    pLights[1].translate(-lightX, 0.0f, -lightZ);
    pLights[2].translate(lightX, lightY, 0.0f);
    pLights[3].translate(-lightX, -lightY, 0.0f);

    for (int i = 0; i < pLights.size(); i++) {
        if (pLights[i].isDrawable() == true) {
            pLights[i].draw();
        }
    }
}

void drawLight() {
    updateRotatingLights();

   // sLights[0].setPosition(cam->getPos());
   // sLights[0].setDirection(cam->getDirection());

    renderer->updateLights(pLights, dLights, sLights, cam->getPos());//passes light positions to all shaders
}


//called from drawingLoop in Framework every frame
void eventloop() {
    drawLight();
    drawCubeOfCubes();
}