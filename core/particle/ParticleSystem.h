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

	struct ParticleSpawner
	{
		Mesh mesh = Mesh::createSquare();

		glm::vec3 position = glm::vec3(0.0f), positionVariation = glm::vec3(0.0f);
		glm::vec3 velocity = glm::vec3(0.0f), velocityVariation = glm::vec3(0.0f);
		glm::vec3 acceleration = glm::vec3(0.0f);

		glm::vec4 colorBegin = glm::vec4(1.0f), colorEnd = glm::vec4(1.0f);
		float sizeBegin = 1.0f, sizeEnd = 1.0f, sizeVariation = 0.0f;

		float lifeTime = 1.0f;
		int count = 1;
	};

	struct Particle
	{
		Mesh mesh = Mesh::createSquare();

		glm::vec3 position = glm::vec3(0.0f), velocity = glm::vec3(0.0f),
			acceleration = glm::vec3(0.0f);
		glm::vec4 colorBegin = glm::vec4(1.0f), colorEnd = glm::vec4(1.0f);
		float rotation = 0.0f;
		float sizeBegin = 1.0f, sizeEnd = 1.0f;

		float lifeTime = 5.0f;
		float lifeRemaining = 0.0f;

		bool active = false;
	};

	class ParticleSystem : public Object, public System
	{
	private:
		ParticleSystemProps systemProps;
		std::vector<Particle> particles;
		std::vector<ParticleSpawner> spawners;
	public:
		static const unsigned char NO_COLLISION		= 0b00000000;
		static const unsigned char SELF_COLLISION	= 0b00000001;
		static const unsigned char GROUND_COLLISION	= 0b00000010;

		bool selfCollision = false;
		bool groundCollision = false;

		LUNA_API ParticleSystem(ParticleSystemProps props);

		void frameUpdate(FrameProps fp) override;
		void tickUpdate(TickProps tp) override;

		void draw(RenderProps renderProps) override;
		void drawParticle(RenderProps renderProps, Particle particle);

		LUNA_API void addParticle(Particle particle);
		LUNA_API void createSpawner(ParticleSpawner particleSpawner);
	};
}