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

    initVertices();//creates local vertex arrays for all shapes we want to reuse
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

//Triangle with Vertex colors
void Renderer::drawTriangle(float* vertexArr, bool useColors, bool useTexture, unsigned int texture, shared_ptr<Shader> shader) {
    if (shader == nullptr) shader = defaultShader;
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    int vertexParams = 8 - 3 * (!useColors) - 2 * (!useTexture);//3 if only coords, 6 with rgb, 8 with texture coords
    int vertexCount = 3;//a triangle has 3 vertices
    glBufferData(GL_ARRAY_BUFFER, vertexParams * vertexCount * sizeof(float), vertexArr, GL_STATIC_DRAW);//use dynamic draw if vertices moves a lot

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexParams * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    if (useColors == true) {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexParams * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    // texture coord attribute
    if (useTexture == true) {
        int offset = vertexParams - 2;//2 before the end of each vertex subsection in the array
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexParams * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    shader->use();
    shader->setBool("useColor", useColors);
    shader->setBool("useTexture", useTexture);
    if (useTexture == true) {
        glBindTexture(GL_TEXTURE_2D, texture);
    }
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);//unbind VBOs
    glBindVertexArray(0);//unbind VAOs

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

//draw a triangle with a texture. 
void Renderer::drawRect(float* vertexArr, bool useColors, bool useTexture, unsigned int texture, shared_ptr<Shader> shader) {
    if (shader == nullptr) shader = defaultShader;
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    int vertexParams = 8 - 3 * (!useColors) - 2 * (!useTexture);//3 if only coords, 6 with rgb, 8 with texture coords
    int vertexCount = 4;//a rect has 4 vertices with the right indexing (see above)

    glBufferData(GL_ARRAY_BUFFER, vertexParams * vertexCount * sizeof(float), vertexArr, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexParams * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    if (useColors == true) {
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexParams * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    // texture coord attribute
    if (useTexture == true) {
        int offset = vertexParams - 2;//2 before the end of each vertex subsection in the array
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexParams * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    //shader handling
    shader->use();
    shader->setBool("useColor", useColors);
    shader->setBool("useTexture", useTexture);
    if (useTexture == true) {
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    //drawing
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //deleting stuff
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Renderer::drawTexturedCubes(float* vertexArr, int verts, int dimension, unsigned int texture1, unsigned int texture2, shared_ptr<Shader> shader) {
    if (shader == nullptr) shader = defaultShader;
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 5 * verts * sizeof(float), vertexArr, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //there is no color attribute for this function (yet), so skip it
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);



    shader->use(); // don't forget to activate/use the shader before setting uniforms!
    //tell the shaders which uniform samples which texture unit
    shader->setInt("texture1", 0);
    shader->setInt("texture2", 1);



    //transformation testing
    /*mat4 trans = mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, vec3(0.5, 0.5, 0.5));
    shader.setMat4("transform", move(trans));*/
    //transformation over time testing 
    /*mat4 trans = mat4(1.0f);
    trans = glm::rotate(trans, (float)glfwGetTime() * 5, vec3(0.0f, 0.0f, 1.0f));
    shader.setMat4("transform", move(trans));*/

    //transformation to clipspace
    mat4 model = mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //transformations here---------------------------------------------------------------------------------------------------------------
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), vec3(0.5f, 1.0f, 0.0f));

    //-----------------------------------------------------------------------------------------------------------------------------------
    mat4 view = mat4(1.0f);

    //camera transformations here--------------------------------------------------------------------------------------------------------
    float radius = 10.0f;
    float camX = static_cast<float>(sin(glfwGetTime()) * radius);
    float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
    view = glm::lookAt(vec3(camX, 0.0f, camZ), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    //-----------------------------------------------------------------------------------------------------------------------------------

    mat4 projection = mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)windowW / (float)windowH, 0.1f, 100.0f);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to
    //set it outside the main loop only once.
    shader->setMat4("projection", projection);


    //bind bove textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    //drawing
    glBindVertexArray(VAO);

    vec3 cubePositions[] = {
    vec3(0.0f,  0.0f,  0.0f),
    vec3(2.0f,  5.0f, -15.0f),
    vec3(-1.5f, -2.2f, -2.5f),
    vec3(-3.8f, -2.0f, -12.3f),
    vec3(2.4f, -0.4f, -3.5f),
    vec3(-1.7f,  3.0f, -7.5f),
    vec3(1.3f, -2.0f, -2.5f),
    vec3(1.5f,  2.0f, -2.5f),
    vec3(1.5f,  0.2f, -1.5f),
    vec3(-1.3f,  1.0f, -1.5f)
    };
    for (unsigned int i = 0; i < 10; i++)
    {
        mat4 model2 = mat4(1.0f);
        model2 = glm::translate(model2, cubePositions[i]);
        float angle = 20.0f * (i + 1) * (float)glfwGetTime();
        model2 = glm::rotate(model2, glm::radians(angle), vec3(1.0f, 0.3f, 0.5f));
        shader->setMat4("model", model2);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    //deleting stuff
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

//draw a triangle with a texture
void Renderer::drawRect(float* vertexArr, int verts, int dimension, unsigned int texture1, unsigned int texture2, shared_ptr<Shader> shader) {
    if (shader == nullptr) shader = defaultShader;
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (3 + 2 + dimension) * verts * sizeof(float), vertexArr, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);



    shader->use(); // don't forget to activate/use the shader before setting uniforms!
    //tell the shaders which uniform samples which texture unit
    shader->setInt("texture1", 0);
    shader->setInt("texture2", 1);



    //transformation testing
    /*mat4 trans = mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, vec3(0.5, 0.5, 0.5));
    shader.setMat4("transform", move(trans));*/
    //transformation over time testing 
    /*mat4 trans = mat4(1.0f);
    trans = glm::rotate(trans, (float)glfwGetTime() * 5, vec3(0.0f, 0.0f, 1.0f));
    shader.setMat4("transform", move(trans));*/

    //transformation to clipspace
    mat4 model = mat4(1.0f); // make sure to initialize matrix to identity matrix first
    mat4 view = mat4(1.0f);
    mat4 projection = mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)windowW / (float)windowH, 0.1f, 100.0f);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader->setMat4("projection", projection);

    //bind bove textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    //drawing
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //deleting stuff
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

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