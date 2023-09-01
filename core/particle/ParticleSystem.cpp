#include "ParticleSystem.h"
#include "../random/Random.h"

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
			tp.rng.generateUniformReal<float>(-0.5f, 0.5f),
			tp.rng.generateUniformReal<float>(-0.5f, 0.5f),
			tp.rng.generateUniformReal<float>(-0.5f, 0.5f)
		);
		glm::vec3 randomVelocityVariation = glm::vec3(
			tp.rng.generateUniformReal<float>(-0.5f, 0.5f),
			tp.rng.generateUniformReal<float>(-0.5f, 0.5f),
			tp.rng.generateUniformReal<float>(-0.5f, 0.5f)
		);
		std::cout << tp.rng.generateUniformReal<float>(-0.5f, 0.5f) << std::endl;
		p.position = s.position + s.positionVariation * randomPositionVariation;
		p.velocity = s.velocity + s.velocityVariation * randomVelocityVariation;
		addParticle(p);
	}

	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].lifeRemaining -= tp.deltatime;
		if (particles[i].lifeRemaining <= 0.0)
			particles[i].active = false;

		particles[i].position += particles[i].velocity;
	}
}

void ParticleSystem::draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		Particle currentParticle = particles[i];
		drawParticle(currentParticle);
	}	
}

void ParticleSystem::drawParticle(Particle currentParticle)
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
	this->shader.setMat4("model", particleModel);
	//this->shader.setMat4("view", glm::translate(view, currentParticle.position));
	//this->shader.setMat4("projection", proj);
	currentParticle.mesh.draw(shader);
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