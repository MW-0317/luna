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

		glGenBuffers(1, &abo);
		glBindBuffer(GL_UNIFORM_BUFFER, abo);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(int) * SIZE, data, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 6, abo);
	}

	DisintegrationEffect::~DisintegrationEffect() { }

	void DisintegrationEffect::draw(Frame frame)
	{
		// Post Draw
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(data), data);
		//std::cout << (51 << 16) + (21 & 65535) << std::endl;
		Log::console.log("%d\n", *(int*) &data[0]); // "evil bit hack" idea taken from fast inverse square root code.
		unsigned int count = *(int*)&data[0];
		Log::updateOverlay("Count", count);
		//float x = static_cast<float>(data[count]) / 100;
		//float y = static_cast<float>(data[count + 1]) / 100;
		float x = data[count];
		float y = data[count + 1];
		//Log::console.log("%d %d\n", data[count], data[count + 1]);
		Log::console.log("%.2f %.2f\n", x, y);
		
		for (int i = 0; i < count; i++)
		{
			//float x = static_cast<float>(data[i] >> 16) / 100;
			//float y = static_cast<float>((data[i] << 16) >> 16) / 100;
			//float x = static_cast<float>(data[2 * i]) / 100;
			//float y = static_cast<float>(data[2 * i + 1]) / 100;
			float x = data[1 + 2 * i];
			float y = data[1 + 2 * i + 1];
			if ((x >= -0.001f && x <= 0.001f) || (y >= -0.001f && y <= 0.001f))
				continue;
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