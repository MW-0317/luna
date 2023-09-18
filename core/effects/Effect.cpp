#include "Effect.h"
#include "../Object.h"
#include "../particle/ParticleSystem.h";

namespace luna
{
	Effect::Effect() { }

	Effect::~Effect() { }

	void Effect::preDraw(RenderProps renderProps) { }

	void Effect::draw(RenderProps renderProps) { }

	void Effect::postDraw(RenderProps renderProps) { }

	DisintegrationEffect::DisintegrationEffect(SpriteParticleSystem* particleSystem)
	{
		this->particleSystem = particleSystem;
	}

	DisintegrationEffect::~DisintegrationEffect() { }

	void DisintegrationEffect::preDraw(RenderProps renderProps)
	{
		//data.reserve(renderProps.width * renderProps.height);
		//memset(data.data(), 0, sizeof(float) * renderProps.width * renderProps.height);
		data[0] = 0;
		glGenBuffers(1, &ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
	}

	void DisintegrationEffect::draw(RenderProps renderProps) { }

	void DisintegrationEffect::postDraw(RenderProps renderProps)
	{
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(data), data);
		//std::cout << (51 << 16) + (21 & 65535) << std::endl;
		int count = data[0];
		std::cout << count << std::endl;
		for (int i = 0; i < count / 1000; i++)
		{
			float x = (float) (data[i] >> 16) / renderProps.height;
			float y = (float) (data[i] & 65535) / renderProps.width;
			Particle particle;
			particle.position = glm::vec3(x, y, 0.0f);
			particle.acceleration = glm::vec3(-1.0f, 1.0f, 0.0f);
			//particleSystem->addParticle(particle);
		}
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}