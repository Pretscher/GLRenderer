#include "Model.hpp"
#include <iostream>
#include <vector>
using namespace std;
#include "stb_image.h"

unsigned int TextureFromFile(const char* path, const string& directory)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    stbi_set_flip_vertically_on_load(false);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void Model::loadModel(string path) {
    Assimp::Importer importer;
    //triangulate := if not only triangles, transform to triangles
    //FlipUvs:= reverses images that are flipped on the y axis (common in opengl)
    //more options here http://assimp.sourceforge.net/lib_html/postprocess_8h.html
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        std::exit(0);
    }
    directory = path.substr(0, path.find_last_of('/'));

    //push all meshes to meshes vector (going down from root node means all nodes will be processed => all meshes in scene)
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
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

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        // process vertex positions, normals and texture coordinates
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);//position
        vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);//normals
        if (mesh->mTextureCoords[0]) {// does the mesh contain texture coordinates?
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);//tex coords
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        //apply vertex colors
        if (scene->mNumMaterials > mesh->mMaterialIndex)
        {
            const auto& mat = scene->mMaterials[mesh->mMaterialIndex];
            aiColor4D diffuse;
            if (AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
            {
                vertex.Color = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
            }
        }
        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace cFace = mesh->mFaces[i];
        for (unsigned int j = 0; j < cFace.mNumIndices; j++) {
            indices.push_back(cFace.mIndices[j]);
        }
    }



    // process material
    if (mesh->mMaterialIndex >= 0) {
        //retrieve material (diffuse maps, specular maps) from index
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());//first APPEND vector of diffuse maps
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());//then APPEND vector of specular maps
    }

    return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
    vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        string texPath = directory + '/' + str.C_Str();
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), texPath.c_str()) == 0)//compare strings
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = texPath;
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}