#include "Renderer.hpp"
#include "Shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int Renderer::windowW, Renderer::windowH;

void resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

//public methods------------------------------------------------------------------------------------------------------------------------

Renderer::Renderer() {
    //init glfw------------------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //create window--------------------------------------------------------------
    
    int windowW = 1600;
    int windowH = 1200;
    Renderer::windowW = windowW;
    Renderer::windowH = windowH;
    window = glfwCreateWindow(windowW, windowH, "MyEngine", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    //init glad (after opening window) ------------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
    }


    glViewport(0, 0, windowW, windowH);//initial window bounds
    glfwSetFramebufferSizeCallback(window, resize);//tell opengl which function to use if window is resized
    initDrawingStuff();
}

void Renderer::setBackgroundColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

unsigned int Renderer::loadTexture(string path, bool flipVertically, bool alphaValue) {
    unsigned int newTex;
    glGenTextures(1, &newTex);

    //texture preferences-------------------------------------------------------------------------------
    glBindTexture(GL_TEXTURE_2D, newTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //Loading the Texture from image file----------------------------------------------------------------
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(flipVertically);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    stbi_set_flip_vertically_on_load(false);//set back to default
    if (data) {
        /*  1) specifies the texture target; setting this to GL_TEXTURE_2D means this operation will generate a texture on the
            currently bound texture object at the same target (so any textures bound to targets GL_TEXTURE_1D or GL_TEXTURE_3D
            will not be affected).
            2) specifies the mipmap level for which we want to create a texture for if you want to set each mipmap level manually,
            but we'll leave it at the base level which is 0.
            3) tells OpenGL in what kind of format we want to store the texture. Our image has only RGB values so we'll store the
            texture with RGB values as well.
            4 and 5) sets the width and height of the resulting texture. We stored those earlier when loading the image so we'll
            use the corresponding variables.
            6) should always be 0 (some legacy stuff).
            7 and 8) specify the format and datatype of the source image. We loaded the image with RGB values and stored them as
            chars (bytes) so we'll pass in the corresponding values.
            9) is the actual image data. */
        if (alphaValue == true) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cout << "Failed to load texture" << endl;
    }
    stbi_image_free(data);
    return newTex;
}

//drawing methods-----------------------------------------------------------------------------------------------------------------------

shared_ptr<Shader> Renderer::createShader(string vertexPath, string fragmentPath, bool perspective, bool updateAutomatically) {
    shared_ptr<Shader> shader(new Shader(vertexPath.c_str(), fragmentPath.c_str()));
    //set projection matrix (only need to do this once usually)e changes
    shader->setProjection(globalProjection);
    shader->setView(globalView);
    if (updateAutomatically == true) {//push back to shaders array to set relevant uniforms when the time has come
        shaders.push_back(shader);
    }
    return shader;
}

shared_ptr<Shader> Renderer::getDefaultShader() {
    return defaultShader;
}
//right -> left -> top -> bottom -> back -> front
unsigned int Renderer::loadCubeMap(vector<string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            // glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i goes through the textures in the order right -> left -> top -> bottom -> back -> front
            // or GL_TEXTURE_CUBE_MAP_POSITIVE_X -> GL_TEXTURE_CUBE_MAP_NEGATIVE_X -> GL_TEXTURE_CUBE_MAP_POSITIVE_Y -> 
            // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
            // -> GL_TEXTURE_CUBE_MAP_POSITIVE_Z -> GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    //texture options
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}