#include "Shader.hpp"
#include "Mesh.hpp"
#include "Drawable.hpp"

//model loader imports
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model : public Drawable {
public:
    Model(const char* path, shared_ptr<Shader> i_shader) : Drawable(i_shader) {
        loadModel(path);
    }
    void draw() {
        for (unsigned int i = 0; i < meshes.size(); i++) {
            meshes[i].draw(this->shader);
        }
    };
private:
    // model data
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;

    void loadModel(string path);
    //pushes all meshes from the node and its children to the meshes array
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    
    //Assumes that the texture is in the same directory as the model
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

    struct Material {
        vec3 Diffuse;
        vec3 Specular;
        vec3 Ambient;
        float Shininess;
    };
    Material loadMaterial(aiMaterial* mat) {
        Material material;
        aiColor3D color(0.f, 0.f, 0.f);
        float shininess;

        mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        material.Diffuse = vec3(color.r, color.b, color.g);

        mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
        material.Ambient = vec3(color.r, color.b, color.g);

        mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
        material.Specular = vec3(color.r, color.b, color.g);

        mat->Get(AI_MATKEY_SHININESS, shininess);
        material.Shininess = shininess;

        return material;
    }
}; 
