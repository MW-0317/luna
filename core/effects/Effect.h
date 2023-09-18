#pragma once

#include "../Core.h"
#include "../Globals.h"

#include <vector>

namespace luna
{
	class SpriteParticleSystem;
	struct Particle;
	struct RenderProps;

	class Effect
	{
	public:
		LUNA_API Effect();
		LUNA_API ~Effect();

		LUNA_API virtual void preDraw(RenderProps renderProps);
		LUNA_API virtual void draw(RenderProps renderProps);
		LUNA_API virtual void postDraw(RenderProps renderProps);
	protected:
		unsigned int data[10000];
		GLuint ssbo;
	};

	class DisintegrationEffect : public Effect
	{
	public:
		LUNA_API DisintegrationEffect(SpriteParticleSystem* particleSystem);
		LUNA_API ~DisintegrationEffect();

		void preDraw(RenderProps renderProps) override;
		void draw(RenderProps renderProps) override;
		void postDraw(RenderProps renderProps) override;
	private:
		SpriteParticleSystem* particleSystem;
	};
}
