#pragma once

#include "../Core.h"
#include "../Globals.h"

#include <vector>

namespace luna
{
	class SpriteParticleSystem;
	struct Particle;
	struct Frame;

	class Effect
	{
	public:
		LUNA_API Effect();
		LUNA_API ~Effect();

		LUNA_API virtual void preDraw(Frame frame);
		LUNA_API virtual void draw(Frame frame);
		LUNA_API virtual void postDraw(Frame frame);
	protected:
		unsigned int data[10000];
		GLuint ssbo;
	};

	class DisintegrationEffect : public Effect
	{
	public:
		LUNA_API DisintegrationEffect(SpriteParticleSystem* particleSystem);
		LUNA_API ~DisintegrationEffect();

		void preDraw(Frame frame) override;
		void draw(Frame frame) override;
		void postDraw(Frame frame) override;
	private:
		SpriteParticleSystem* particleSystem;
	};
}
