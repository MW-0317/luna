#include "ParticleSystem.h"
#include "../random/Random.h"
#include "../Space.h"
#include "../Camera.h"

using namespace luna;

ParticleSystem::ParticleSystem(ParticleSystemProps props) 
	: Object(props.mesh, props.shader, props.position, props.scale)
{
	systemProps = props;
}

void ParticleSystem::frameUpdate(FrameProps fp)
{
	//draw();
}

void ParticleSystem::tickUpdate(TickProps tp)
{
	for (int i = 0; i < spawners.size(); i++)
	{
		ParticleSpawner s = spawners[i];
		Particle p;
		glm::vec3 randomPositionVariation = glm::vec3(
			tp.rng->generateUniformReal<float>(-1.0f, 1.0f),
			tp.rng->generateUniformReal<float>(-1.0f, 1.0f),
			tp.rng->generateUniformReal<float>(-1.0f, 1.0f)
		);
		glm::vec3 randomVelocityVariation = glm::vec3(
			tp.rng->generateUniformReal<float>(-1.0f, 1.0f),
			tp.rng->generateUniformReal<float>(-1.0f, 1.0f),
			tp.rng->generateUniformReal<float>(-1.0f, 1.0f)
		);
		p.position = s.position + s.positionVariation * randomPositionVariation;
		p.velocity = s.velocity + s.velocityVariation * randomVelocityVariation;
		p.acceleration = s.acceleration;
		addParticle(p);
	}

	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].lifeRemaining -= tp.deltatime;
		if (particles[i].lifeRemaining <= 0.0)
			particles[i].active = false;
		
		particles[i].velocity += particles[i].acceleration * tp.deltatime;
		particles[i].position += particles[i].velocity * tp.deltatime;
	}
}

void ParticleSystem::draw(RenderProps renderProps)
{
	for (int i = 0; i < particles.size(); i++)
	{
		Particle currentParticle = particles[i];
		drawParticle(renderProps, currentParticle);
	}	
}

void ParticleSystem::drawParticle(RenderProps renderProps, Particle currentParticle)
{
	if (!currentParticle.active)
		return;
	glm::mat4 particleModel = glm::mat4(1.0f);
	float currentSize = glm::mix(
		currentParticle.sizeEnd, 
		currentParticle.sizeBegin, 
		currentParticle.lifeRemaining / currentParticle.lifeTime
	);
	particleModel = glm::translate(particleModel, currentParticle.position);
	particleModel = glm::scale(particleModel, glm::vec3(currentSize));
	renderProps.model = particleModel;
	currentParticle.mesh.setNormalRotation(renderProps.camera->getForwardVector());
	currentParticle.mesh.draw(renderProps, shader);
}

void ParticleSystem::addParticle(Particle particle)
{
	particle.active = true;
	particle.lifeRemaining = particle.lifeTime;
	particles.push_back(particle);
}

void ParticleSystem::createSpawner(ParticleSpawner particleSpawner)
{
	spawners.push_back(particleSpawner);
}