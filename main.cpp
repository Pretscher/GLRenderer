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
vector<DirectionalLight> dLights;
vector<SpotLight> sLights;

shared_ptr<Shader> singleColorShader;

Model* sun;
Model* backpack;
void initRotatingLights() {
    //we create new shaders for every light because they have different colors set in the shader 
    //and i cant be bothered with vertex coloring rn
    pLights.push_back(PointLight(100, { 1.0f, 0.0f, 0.0f }));
    pLights.push_back(PointLight(100, { 0.0f, 1.0f, 0.0f }));
    pLights.push_back(PointLight(100, { 0.0f, 0.0f, 1.0f }));
    pLights.push_back(PointLight(100, { 1.0f, 1.0f, 0.0f }));

   // pLights.push_back(PointLight({ 0.5f, 0.5f, 1.0f }));
}

//init all the stuff needed before drawing (renderer and camera already fully available)
void initEvents() {
    initRotatingLights();
    dLights.push_back(DirectionalLight({ 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }));
    dLights[0].setIntensity(0.0f, 1.0f, 2.0f);

    sLights.push_back(SpotLight(5.0f, 10.0f, { 1.0f, 1.0f, 1.0f }));
    sLights[0].setIntensity(0.0f, 1.0f, 2.0f);
    
    containerTex = Renderer::loadTexture("Textures/container2.png", false, true);
    specularMap = Renderer::loadTexture("Textures/container2_specular.png", false, true);
    smileyTex = Renderer::loadTexture("Textures/awesomeface.png", true, true);
    for (int i = 0; i < cubeCount; i++) {
        cubes.push_back(Cube({ containerTex }, { specularMap }, renderer->getDefaultShader()));
    }
    backpack = new Model("C:/Users/Julian/source/repos/2022/GLRenderer/Models/backpack/backpack.obj", renderer->getDefaultShader());
    sun = new Model("C:/Users/Julian/source/repos/2022/GLRenderer/Models/sun/Sun.obj", renderer->getDefaultShader());
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

void updateRotatingLights() {
    const float radius = 5.0f;
    float lightX = (float)sin((float)glfwGetTime() * 2) * radius;
    float lightZ = (float)cos((float)glfwGetTime() * 2) * radius;
    float lightY = (float)cos((float)glfwGetTime() * 2) * radius;

    float lightWidth = 0.5f, lightHeight = 0.1f, lightDepth = 0.5f;
    pLights[0].setPosition( lightX, -lightHeight / 2,  lightZ);
    pLights[1].setPosition(-lightX, -lightHeight / 2, -lightZ);
    pLights[2].setPosition( lightX,  lightY,          -lightDepth / 2);
    pLights[3].setPosition(-lightX, -lightY,          -lightDepth / 2);
}

void drawLight() {
    updateRotatingLights();
    sLights[0].setPosition(cam->getPos());
    sLights[0].setDirection(cam->getDirection());
    renderer->updateLights(pLights, dLights, sLights, cam->getPos());//passes light positions to all shaders
}

//called from drawingLoop in Framework every frame
void eventloop() {

    drawLight();
   // drawCubeOfCubes();
    backpack->translate(0.0f, 5.0f, 0.0f);
    backpack->updateAndDraw();
    sun->updateAndDraw();
}