#pragma once

#include "../Core.h"
#include "../Globals.h"
#include "../System.h"

#include <vector>

namespace luna
{
	class SpriteParticleSystem;
	class ParticleSystem;
	struct Frame;

	class Effect : public System
	{
	public:
		LUNA_API Effect();
		LUNA_API ~Effect();

		LUNA_API virtual void draw(Frame frame);

		LUNA_API void frameUpdate(Frame frame) override;
		LUNA_API void tickUpdate(Tick tick) override;
	};

	class DisintegrationEffect : public Effect
	{
	protected:
		const static int SIZE = 100000;
		float data[SIZE] = {};
		GLuint ssbo;
		GLuint abo;
		ParticleSystem* particleSystem;
	public:
		LUNA_API DisintegrationEffect(ParticleSystem* particleSystem);
		LUNA_API ~DisintegrationEffect();

		LUNA_API virtual void draw(Frame frame) override;
	};
}
