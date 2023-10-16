#include "Effect.h"
#include "../Object.h"
#include "../logging/ImguiLogs.h"
#include "../particle/ParticleSystem.h";

namespace luna
{
	Effect::Effect() { }

	Effect::~Effect() { }

	void Effect::draw(Frame frame) { }

	void Effect::frameUpdate(Frame frame)
	{
		this->draw(frame);
	}

	void Effect::tickUpdate(Tick tick) {}

	DisintegrationEffect::DisintegrationEffect(ParticleSystem* particleSystem)
	{
		this->ssbo = 0;
		this->particleSystem = particleSystem;
	}

	DisintegrationEffect::~DisintegrationEffect() { }

	void DisintegrationEffect::draw(Frame frame)
	{
		// Post Draw
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(data), data);
		//std::cout << (51 << 16) + (21 & 65535) << std::endl;
		int count = data[0];
		Log::updateOverlay("Count", count);
		float x = static_cast<float>(data[1] >> 16) / frame.width;
		float y = static_cast<float>((data[1] << 16) >> 16) / frame.height;
		Log::console.log("%.2f %.2f\n", static_cast<float>(data[1] >> 16), static_cast<float>((data[1] << 16) >> 16));
		
		for (int i = 0; i < count; i++)
		{
			float x = static_cast<float>(data[i] >> 16) / frame.width;
			float y = static_cast<float>((data[i] << 16) >> 16) / frame.height;
			ShaderParticle particle;
			particle.position = glm::vec3(x, y, 0.0f);
			particle.acceleration = glm::vec3(-1.0f, 1.0f, 0.0f);
			particleSystem->addParticle(particle);
		}
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

		// Pre Draw
		data[0] = 0;
		glGenBuffers(1, &ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, ssbo);
	}
}