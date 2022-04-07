#include "Drawable.hpp"

void Drawable::update(shared_ptr<Shader> shader) {
    shader->use();
    //basic color properties
    shader->setVec3("baseColor", baseColor);//if 0.0 (default) nothing happens.
    shader->setBool("influencedByLighting", influencedByLighting);
    //material
    shader->setFloat("materialDissuseSensitivity", diffuseSensitivity);
    shader->setFloat("materialSpecularSensitivity", specularSensitivity);
    shader->setFloat("materialShininess", shininess);

    applyTransformations(shader);
}

void Drawable::applyTransformations(shared_ptr<Shader> shader) {
    //model matrix
    mat4 model(1.0f);
    //first scale
    model[0][0] = scaleX;
    model[1][1] = scaleY;
    model[2][2] = scaleZ;
    //then rotate (if you translate before that it will rotate in a circle around the origin from its coordinates)
    for (int i = 0; i < rotations.size(); i++) {
        model = glm::rotate(model, rotations[i].angle, vec3(rotations[i].x, rotations[i].y, rotations[i].z));//rotate
    }
    //then translate
    model[3][0] = transX;
    model[3][1] = transY;
    model[3][2] = transZ;
    shader->setMat4("model", model);
}

void Drawable::drawColoredOutline() {
    //stencil everything away around the object, the object itself is preserved by setting all its stencil values to 1 in prepareOutline()
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); // disable writing to the stencil buffer
    glDisable(GL_DEPTH_TEST);
    //use a single colored shader for the outline.
    singleColorShader->use();
    singleColorShader->setVec4("color", outlineColor);
    //scale drawable up by the outlinewidth
    float scaleSize = 1.0f + outlineWidth;
    this->scale(scaleSize, scaleSize, scaleSize);
    //draw upscaled version of drawable in one color, but stencil out the originally drawn shape (only outline will be drawn)
    //we need to update the vertex shader of our singleColorShader so that the outline is in the right place
    this->update(singleColorShader);
    this->draw(singleColorShader);

    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 0, 0xFF); // all fragments should pass the stencil test
    glStencilMask(0xFF); // enable writing to the stencil buffer
    glEnable(GL_DEPTH_TEST);
}