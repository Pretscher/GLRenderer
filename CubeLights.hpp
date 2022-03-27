#pragma once
#include "Cube.hpp"
class CubeLight : public Cube {
protected://constructor is not callable for this class because its only use is being a base class for below specific lights
	CubeLight(vec3 lightRGB, vector<unsigned int> i_textures, vector<unsigned int> i_specMaps, shared_ptr<Shader> i_shader)
		: Cube(i_textures, i_specMaps, i_shader) {

		baseColor = lightRGB;
		position = { 0.0f, 0.0f, 0.0f };
	}

	//Warning: if you call this constructor, using the draw function will cause an error. 
	CubeLight(vec3 lightRGB) : Cube({}, {}, nullptr) {
		dontDraw = true;//member of drawable base class.
		baseColor = lightRGB;
		position = { 0.0f, 0.0f, 0.0f };
	}

public:
	virtual void setShaderLightObject(shared_ptr<Shader> shader, int index) = 0;
	void setIntensity(float ambient, float diffuse, float specular) {
		ambientIntensity = ambient;
		diffuseIntensity = diffuse;
		specularIntensity = specular;
	}

	float ambientIntensity = 0.1f;
	float diffuseIntensity = 1.0f;
	float specularIntensity = 1.0f;
};

class PointLight : public CubeLight {
public:
	float distance;
	PointLight(float distance, vec3 lightRGB, vector<unsigned int> i_textures,  vector<unsigned int> i_specMaps, shared_ptr<Shader> i_shader)
		: CubeLight(lightRGB, i_textures, i_specMaps, i_shader) {
		this->distance = distance;
	}
	//Warning: if you call this constructor, using the draw function will cause an error. 
	PointLight(float distance, vec3 lightRGB) : CubeLight(lightRGB) {
		this->distance = distance;
	}

	PointLight(vec3 lightRGB, vector<unsigned int> i_textures, vector<unsigned int> i_specMaps, shared_ptr<Shader> i_shader)
		: CubeLight(lightRGB, i_textures, i_specMaps, i_shader) {
		this->distance = 80.0f;//Default value
	}
	//Warning: if you call this constructor, using the draw function will cause an error. 
	PointLight(vec3 lightRGB) : CubeLight(lightRGB) {
		this->distance = 80.0f;//Default value
	}

	void setShaderLightObject(shared_ptr<Shader> shader, int index) {
		string brackets = "[" + to_string(index) + "]";
		shader->use();
		shader->setVec3("pLights" + brackets + ".position", position);
		shader->setVec3("pLights" + brackets + ".color", baseColor);
		shader->setFloat("pLights" + brackets + ".ambientIntensity", ambientIntensity);
		shader->setFloat("pLights" + brackets + ".diffuseIntensity", diffuseIntensity); // darken diffuse light a bit
		shader->setFloat("pLights" + brackets + ".specularIntensity", specularIntensity);

		//some precalculated values for those ranges
		float constant = 1.0f, linear, quadratic;
		if (distance <= 7.0f) { linear = 0.7; quadratic = 1.8; }
		else if (distance <= 13.0f) { linear = 0.35; quadratic = 0.44; }
		else if (distance <= 20.0f) { linear = 0.22; quadratic = 0.20; }
		else if (distance <= 32.0f) { linear = 0.14; quadratic = 0.07; }
		else if (distance <= 50.0f) { linear = 0.09; quadratic = 0.032; }
		else if (distance <= 65.0f) { linear = 0.07; quadratic = 0.017; }
		else if (distance <= 100.0f) { linear = 0.045; quadratic = 0.0075; }
		else if (distance <= 160.0f) { linear = 0.027; quadratic = 0.0028; }
		else if (distance <= 200.0f) { linear = 0.022; quadratic = 0.0019; }
		else if (distance <= 325.0f) { linear = 0.014; quadratic = 0.0007; }
		else if (distance <= 600.0f) { linear = 0.007; quadratic = 0.0002; }
		else if (distance <= 3250.0f) { linear = 0.0014; quadratic = 0.000007; }

		shader->setFloat("pLights" + brackets + ".constant", constant);
		shader->setFloat("pLights" + brackets + ".linear", linear);
		shader->setFloat("pLights" + brackets + ".quadratic", quadratic);
	}
};

class DirectionalLight {
public:
	DirectionalLight(vec3 direction, vec3 lightRGB) {
		rgb = lightRGB;
		this->direction = direction;
	}

	void setShaderLightObject(shared_ptr<Shader> shader, int index) {
		string brackets = "[" + to_string(index) + "]";
		shader->use();
		shader->setVec3("dLights" + brackets + ".direction", direction);
		shader->setVec3("dLights" + brackets + ".color", rgb);
		shader->setFloat("dLights" + brackets + ".ambientIntensity", ambientIntensity);
		shader->setFloat("dLights" + brackets + ".diffuseIntensity", diffuseIntensity); // darken diffuse light a bit
		shader->setFloat("dLights" + brackets + ".specularIntensity", specularIntensity);
	}

	void setIntensity(float ambient, float diffuse, float specular) {
		ambientIntensity = ambient;
		diffuseIntensity = diffuse;
		specularIntensity = specular;
	}
	vec3 rgb;
	float ambientIntensity = 0.3f;
	float diffuseIntensity = 2.0f;
	float specularIntensity = 2.0f;
	vec3 direction;
};

class SpotLight : public CubeLight {
public:
	vec3 direction;
	float innerCutOffAngle, outerCutOffAngle;
	//This constructor makes the Spotlight drawable.
	SpotLight(float i_cutOffAngle, float fadeOutRadius, vec3 lightRGB, vector<unsigned int> i_textures, vector<unsigned int> i_specMaps, shared_ptr<Shader> i_shader)
		: CubeLight(lightRGB, i_textures, i_specMaps, i_shader) {
		innerCutOffAngle = i_cutOffAngle;
		outerCutOffAngle = fadeOutRadius;
		position = { 0.0f, 0.0f, 0.0f };
		direction = { 0.0f, 0.0f, -1.0f };
	}
	//Warning: if you call this constructor the drawing function will throw an error because the light has no shader 
	//AND the light position will be 0/0/0, and the direction 0/0/-1.
	SpotLight(float i_cutOffAngle, float fadeOutRadius, vec3 lightRGB) : CubeLight(lightRGB) {
		innerCutOffAngle = i_cutOffAngle;
		outerCutOffAngle = fadeOutRadius;
		position = { 0.0f, 0.0f, 0.0f };
		direction = { 0.0f, 0.0f, -1.0f };
	}

	//Warning: if you call this constructor the drawing function will throw an error because the light has no shader 
	//AND the light position will be 0/0/0, and the direction 0/0/-1.
	SpotLight(float i_cutOffAngle, float fadeOutRadius, vec3 lightRGB, vec3 i_position, vec3 i_direction) : CubeLight(lightRGB) {
		innerCutOffAngle = i_cutOffAngle;
		outerCutOffAngle = fadeOutRadius;
		position = i_position;
		direction = i_direction;
	}

	void setShaderLightObject(shared_ptr<Shader> shader, int index) {
		string brackets = "[" + to_string(index) + "]";
		shader->use();
		shader->setVec3("sLights" + brackets + ".position", position);
		shader->setVec3("sLights" + brackets + ".direction", direction);

		//cos so that we dont need the inverse cosine in the shader (expensive operation we would need to retrieve angle from dot product)
		shader->setFloat("sLights" + brackets + ".innerCutOff", glm::cos(glm::radians(innerCutOffAngle)));
		shader->setFloat("sLights" + brackets + ".outerCutOff", glm::cos(glm::radians(outerCutOffAngle)));
		shader->setVec3("sLights" + brackets + ".color", baseColor);
		shader->setFloat("sLights" + brackets + ".ambientIntensity", ambientIntensity);
		shader->setFloat("sLights" + brackets + ".diffuseIntensity", diffuseIntensity); // darken diffuse light a bit
		shader->setFloat("sLights" + brackets + ".specularIntensity", specularIntensity);
	}

	void setPosition(vec3 i_positon) {
		position = i_positon;
	}
	void setDirection(vec3 i_direction) {
		this->direction = i_direction;
	}
};