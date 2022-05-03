#pragma once
#include "Shader.hpp"
using namespace glm;
using namespace std;
class Light {
protected://constructor is not callable for this class because its only use is being a base class for below specific lights
	Light(vec3 lightRGB) {
		lightColor = lightRGB;
	}

public:
	virtual void setShaderLightObject(shared_ptr<Shader> shader, int index) = 0;
	void setIntensity(float ambient, float diffuse, float specular) {
		ambientIntensity = ambient;
		diffuseIntensity = diffuse;
		specularIntensity = specular;
	}

	void setColor(float r, float g, float b) {
		lightColor = {r, g, b};
	}
	
	void translate(float x, float y, float z) {
		if (resetPosition == true) {
			resetPosition = false;
			position = vec3(0.0f);
		}
		position += vec3(x, y, z);
	}

	void translate(vec3 i_position) {
		if (resetPosition == true) {
			resetPosition = false;
			position = vec3(0.0f);
		}
		position += i_position;
	}
	vec3 position = vec3(0.0f);

protected:
	bool resetPosition = false;
	vec3 lightColor;
	float ambientIntensity = 0.1f;
	float diffuseIntensity = 1.0f;
	float specularIntensity = 1.0f;
};


class PointLight : public Light {
public:

	float distance;
	PointLight(float distance, vec3 lightRGB) : Light (lightRGB) {
		this->distance = distance;
	}

	void setShaderLightObject(shared_ptr<Shader> shader, int index) {
		string name = "pLights[" + to_string(index) + "]";
		shader->use();
		shader->setVec3(name + ".position", position);
		//reset position so that multiple translates can be added with +=
		shader->setVec3(name + ".color", lightColor);
		shader->setFloat(name + ".ambientIntensity", ambientIntensity);
		shader->setFloat(name + ".diffuseIntensity", diffuseIntensity); // darken diffuse light a bit
		shader->setFloat(name + ".specularIntensity", specularIntensity);

		//some precalculated values for those ranges
		float constant = 1.0f, linear, quadratic;
		if (distance <= 7.0f) { linear = 0.7f; quadratic = 1.8f; }
		else if (distance <= 13.0f) { linear = 0.35f; quadratic = 0.44f; }
		else if (distance <= 20.0f) { linear = 0.22f; quadratic = 0.20f; }
		else if (distance <= 32.0f) { linear = 0.14f; quadratic = 0.07f; }
		else if (distance <= 50.0f) { linear = 0.09f; quadratic = 0.032f; }
		else if (distance <= 65.0f) { linear = 0.07f; quadratic = 0.017f; }
		else if (distance <= 100.0f) { linear = 0.045f; quadratic = 0.0075f; }
		else if (distance <= 160.0f) { linear = 0.027f; quadratic = 0.0028f; }
		else if (distance <= 200.0f) { linear = 0.022f; quadratic = 0.0019f; }
		else if (distance <= 325.0f) { linear = 0.014f; quadratic = 0.0007f; }
		else if (distance <= 600.0f) { linear = 0.007f; quadratic = 0.0002f; }
		else { linear = 0.0014f; quadratic = 0.000007f; }//if (distance <= 3250.0f) would be textbook

		shader->setFloat(name + ".constant", constant);
		shader->setFloat(name + ".linear", linear);
		shader->setFloat(name + ".quadratic", quadratic);
		resetPosition = true;
	}
};


class DirectionalLight {
public:
	DirectionalLight(vec3 startDirection, vec3 lightRGB) {
		rgb = lightRGB;
		*direction = startDirection;
	}
	//call this constructor to bind the direction vector to a passed pointer, automatically actualizing it with the vector this pointer points to
	DirectionalLight(shared_ptr<vec3> boundDirection, vec3 lightRGB) {
		rgb = lightRGB;
		direction = boundDirection;
	}

	void setShaderLightObject(shared_ptr<Shader> shader, int index) {
		string brackets = "[" + to_string(index) + "]";
		shader->use();
		shader->setVec3("dLights" + brackets + ".direction", *direction);
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

	void setDirection(float x, float y, float z) {
		*direction = { x, y, z };
	}

	void setDirection(vec3 i_direction) {
		*direction = i_direction;
	}

	void bindDirection(shared_ptr<vec3> boundDirection) {
		direction = boundDirection;
	}
private:
	vec3 rgb;
	float ambientIntensity = 0.3f;
	float diffuseIntensity = 2.0f;
	float specularIntensity = 2.0f;
	shared_ptr<vec3>  direction = shared_ptr<vec3>(new vec3(0.0f, 0.0f, -1.0f));
};

class SpotLight : public Light {
public:

	//This constructor makes the Spotlight drawable.
	SpotLight(float i_cutOffAngle, float fadeOutRadius, vec3 lightRGB) : Light(lightRGB) {
		innerCutOffAngle = i_cutOffAngle;
		outerCutOffAngle = fadeOutRadius;
		direction = vec3(0.0f, 0.0f, -1.0f);
	}

	SpotLight(float i_cutOffAngle, float fadeOutRadius, vec3 lightRGB, shared_ptr<vec3> i_position, vec3 i_direction) : Light(lightRGB) {
		innerCutOffAngle = i_cutOffAngle;
		outerCutOffAngle = fadeOutRadius;
		direction = i_direction;
	}

	void setShaderLightObject(shared_ptr<Shader> shader, int index) {
		string brackets = "[" + to_string(index) + "]";
		shader->use();
		shader->setVec3("sLights" + brackets + ".position", position);
		shader->setVec3("sLights" + brackets + ".direction", direction);

		//cos so that we dont need the inverse cosine in the shader (expensive operation we would need to retrieve angle from dot product)
		shader->setFloat("sLights" + brackets + ".innerCutOff", cos(radians(innerCutOffAngle)));
		shader->setFloat("sLights" + brackets + ".outerCutOff", cos(radians(outerCutOffAngle)));
		shader->setVec3("sLights" + brackets + ".color", lightColor);
		shader->setFloat("sLights" + brackets + ".ambientIntensity", ambientIntensity);
		shader->setFloat("sLights" + brackets + ".diffuseIntensity", diffuseIntensity); // darken diffuse light a bit
		shader->setFloat("sLights" + brackets + ".specularIntensity", specularIntensity);
		resetPosition = true;
	}

	void setDirection(float x, float y, float z) {
		direction = { x, y, z };
	}

	void setDirection(vec3 i_direction) {
		direction = i_direction;
	}


private:
	vec3 direction;
	float innerCutOffAngle, outerCutOffAngle;
};