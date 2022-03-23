#include <iostream>
#include <vector>
using namespace std;

#include "Shader.hpp"
#include "Mesh.hpp"
//model loader imports
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
public:
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw(Shader& shader) {
        for (unsigned int i = 0; i < meshes.size(); i++) {
            meshes[i].Draw(shader);
        }
    }
private:
    // model data
    vector<Mesh> meshes;
    string directory;

    void loadModel(string path) {
        Assimp::Importer importer;
        //triangulate := if not only triangles, transform to triangles
        //FlipUvs:= reverses images that are flipped on the y axis (common in opengl)
        //more options here http://assimp.sourceforge.net/lib_html/postprocess_8h.html
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
            std::exit(0);
        }
        directory = path.substr(0, path.find_last_of('/'));
        
        //push all meshes to meshes vector (going down from root node means all nodes will be processed => all meshes in scene)
        processNode(scene->mRootNode, scene);
    }
    
    //pushes all meshes from the node and its children to the meshes array
    void processNode(aiNode* node, const aiScene* scene) {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }
    Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            // process vertex positions, normals and texture coordinates
           
            vertices.push_back(vertex);
        }
        // process indices
        
        // process material
        if (mesh->mMaterialIndex >= 0)
        {
           
        }

        return Mesh(vertices, indices, textures);
    }
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        string typeName);
};