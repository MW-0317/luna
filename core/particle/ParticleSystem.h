#pragma once

#include "../Globals.h"
#include "../Object.h"
#include "../System.h"

#include <thirdparty/glm/glm.hpp>
#include <vector>

namespace luna
{
	enum BasicParticleMeshes
	{
		Square
	};

	struct ParticleSystemProps
	{
		GLFWwindow* window;
		Mesh mesh;
		Shader shader;
		glm::vec3 position;
		glm::vec3 scale;
	};

	struct ParticleSpawnerProps
	{
		Mesh mesh = Mesh::createSquare();

		glm::vec3 position = glm::vec3(0.0f), positionVariation = glm::vec3(0.0f);
		glm::vec3 velocity = glm::vec3(0.0f), velocityVariation = glm::vec3(0.0f);
		glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec4 colorBegin = glm::vec4(1.0f), colorEnd = glm::vec4(1.0f);
		float sizeBegin = 1.0f, sizeEnd = 1.0f, sizeVariation = 0.0f;

		float lifeTime = 1.0f;
		int count = 1;
	};

	struct Particle
	{
		Mesh mesh = Mesh::createSquare();

		glm::vec3 position = glm::vec3(0.0f), velocity = glm::vec3(0.0f);
		glm::vec4 colorBegin = glm::vec4(1.0f), colorEnd = glm::vec4(1.0f);
		float rotation = 0.0f;
		float sizeBegin = 1.0f, sizeEnd = 1.0f;

		float lifeTime = 1.0f;
		float lifeRemaining = 0.0f;

		bool active = false;
	};

	class ParticleSystem : public Object, public virtual System
	{
	private:
		ParticleSystemProps systemProps;
		std::vector<Particle> particles;
	public:
		LUNA_API ParticleSystem(ParticleSystemProps props);

		void frameUpdate(float deltatime) override;
		void tickUpdate(float deltatime) override;

		void draw() override;
		void drawParticle(int index);

		LUNA_API void addParticle(Particle particle);
		void createSpawner(ParticleSpawnerProps particleProps);
	};
}