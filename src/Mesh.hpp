#pragma once
#include "Shader.hpp"
#include <vector>
using namespace std;

struct Vertex {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
    vec4 Color;
};

struct Texture {
    unsigned int id;
    string path;
    string type;//e.g. diffuse/specular texture
};

class Mesh {
public:
    // mesh data
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }

    void draw(shared_ptr<Shader> shader) {
        shader->use();
        unsigned int diffuseNr = 0;
        unsigned int specularNr = 0;
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string cNumber;
            string cType;
            if (textures[i].type == "texture_diffuse") {
                cNumber = to_string(diffuseNr);
                cType = "diffuse";
                diffuseNr ++;
            }
            else if (textures[i].type == "texture_specular") {
                cNumber = to_string(specularNr);
                cType = "specular";
                specularNr ++;
            }
            
            string name = ("material." + cType + "[" + cNumber + "]");
            shader->setInt(name, i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        shader->setInt("material.diffuseMapCount", diffuseNr);//lenght = last index + 1
        shader->setInt("material.specMapCount", specularNr);
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //structs have a sequential memory layout, thus an array of structs is like a tightly packed array of floats (like we had in examples before)
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex positions (layout = 0, 3 Elements per Vertex)

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(0);
        // vertex texture coords (layout = 2, 2 elements per Vertex)
        // vertex normals (layout = 1, 3 elements per vertex)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        glEnableVertexAttribArray(1);
        //layout = 2, 2 elements per vertex
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        glEnableVertexAttribArray(2);
;        // vertex color (layout = 3, 3 elements per vertex)
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
        glEnableVertexAttribArray(3);

        glBindVertexArray(0);
    }
};