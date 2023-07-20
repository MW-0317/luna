#pragma once

#include <thirdparty/glm/glm.hpp>

struct ParticleProps
{
	glm::vec3 position;
	glm::vec3 velocity, velocityVariation;
	glm::vec4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifeTime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem();

	void frameUpdate(float deltatime);
private:
	struct Particle
	{
		glm::vec3 position, velocity;
		glm::vec4 colorBegin, colorEnd;
		float rotation;
		float sizeBegin, sizeEnd;

		float lifeTime = 1.0f;
		float lifeRemaining = 0.0f;

		bool active = false;
	};
};

