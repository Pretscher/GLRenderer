#pragma once
#include "Model.hpp"
#include "Drawable.hpp"
class ModelLight : public Model {
protected://constructor is not callable for this class because its only use is being a base class for below specific lights
	ModelLight(string path, vec3 lightRGB, shared_ptr<Shader> i_shader)
		: Model(path.c_str(), i_shader) {
		lightColor = lightRGB;
	}

public:
	virtual void setShaderLightObject(shared_ptr<Shader> shader, int index) = 0;
	void setIntensity(float ambient, float diffuse, float specular) {
		ambientIntensity = ambient;
		diffuseIntensity = diffuse;
		specularIntensity = specular;
	}

	vec3 lightColor;
	float ambientIntensity = 0.1f;
	float diffuseIntensity = 1.0f;
	float specularIntensity = 1.0f;
};


class ModelPointLight : public ModelLight {
public:
	float distance;
	ModelPointLight(string path, float distance, vec3 lightRGB, shared_ptr<Shader> i_shader)
		: ModelLight(path, lightRGB, i_shader) {
		this->distance = distance;
	}
	//constructor with default lighting falloff distance
	ModelPointLight(string path, vec3 lightRGB, shared_ptr<Shader> i_shader)
		: ModelLight(path, lightRGB, i_shader) {
		this->distance = 80.0f;//Default value
	}

	void setShaderLightObject(shared_ptr<Shader> shader, int index) {
		string brackets = "[" + to_string(index) + "]";
		shader->use();
		shader->setVec3("pLights" + brackets + ".position", position);
		shader->setVec3("pLights" + brackets + ".color", lightColor);
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
