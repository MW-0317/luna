#include "ParticleSystem.h"
#include "../Engine.h"
#include "../random/Random.h"
#include "../Space.h"
#include "../Camera.h"

using namespace luna;

SpriteParticleSystem::SpriteParticleSystem(ParticleSystemProps props)
	: Object(props.mesh, props.shader, props.position, props.scale)
{
	systemProps = props;
}

void SpriteParticleSystem::frameUpdate(FrameProps fp)
{
	//draw();
}

void SpriteParticleSystem::tickUpdate(TickProps tp)
{
	for (int i = 0; i < spawners.size(); i++)
	{
		if (spawners[i].n >= spawners[i].count)
			continue;
		ParticleSpawner s = spawners[i];
		Particle p;
		p.sizeBegin = s.sizeBegin;
		p.sizeEnd = s.sizeEnd;
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
		spawners[i].n++;
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

void SpriteParticleSystem::draw(RenderProps renderProps)
{
	for (int i = 0; i < particles.size(); i++)
	{
		Particle currentParticle = particles[i];
		drawParticle(renderProps, currentParticle);
	}	
}

void SpriteParticleSystem::drawParticle(RenderProps renderProps, Particle currentParticle)
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
	renderProps.view = glm::scale(renderProps.view, glm::vec3(currentSize));
	renderProps.model = particleModel;
	currentParticle.tex.bind();
	currentParticle.mesh.draw(renderProps, shader);
}

void SpriteParticleSystem::addParticle(Particle particle)
{
	particle.active = true;
	particle.lifeRemaining = particle.lifeTime;
	particles.push_back(particle);
}

void SpriteParticleSystem::createSpawner(ParticleSpawner particleSpawner)
{
	spawners.push_back(particleSpawner);
}

ParticleSystem::ParticleSystem(ParticleSystemProps props)
	: Object(props.mesh, props.shader, props.position, props.scale)
{
	systemProps = props;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, );

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 
		10 * sizeof(float) + sizeof(unsigned int), 
		(void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
		10 * sizeof(float) + sizeof(unsigned int), 
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 
		10 * sizeof(float) + sizeof(unsigned int), 
		(void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 
		10 * sizeof(float) + sizeof(unsigned int), 
		(void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 1, GL_UNSIGNED_INT, GL_FALSE, 
		10 * sizeof(float) + sizeof(unsigned int), 
		(void*)(10 * sizeof(float)));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);
}

float* ParticleSystem::particlesToFlatArray()
{
	if (particles.size() == 0)
		return nullptr;
	return &particles[0].position.x;
}

int ParticleSystem::getParticlesSize()
{
	return particles.size() * (10 * sizeof(float) + sizeof(unsigned int));
}

void ParticleSystem::frameUpdate(FrameProps fp)
{

}

void ParticleSystem::tickUpdate(TickProps tp)
{
	for (int i = 0; i < spawners.size(); i++)
	{
		if (spawners[i].n >= spawners[i].count)
			continue;
		ParticleSpawner s = spawners[i];
		ShaderParticle p;
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
		p.life = s.lifeTime;
		addParticle(p);
		spawners[i].n++;
	}
}

void ParticleSystem::draw(RenderProps renderProps)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, scale);
	model = glm::translate(model, position);
	renderProps.model = model;
	shader.setFloat("time", glfwGetTime());
	shader.setFloat("deltatime", renderProps.deltatime);

	shader.setInt("width", renderProps.width);
	shader.setInt("height", renderProps.height);

	shader.setMat4("model", renderProps.model);
	shader.setMat4("view", renderProps.view);
	shader.setMat4("projection", renderProps.proj);
	shader.use();
	glBindVertexArray(VAO);
	renderProps.engine->oLog.update("Size", particles.size());
	renderProps.engine->oLog.update("PSize", getParticlesSize());
	if (particles.size() > 0)
	{
		glBufferData(GL_ARRAY_BUFFER, getParticlesSize(), particlesToFlatArray(), GL_STATIC_DRAW);
		//glBufferData(GL_ARRAY_BUFFER,
		//		sizeof(float) * verticesFloatArray.size(), &verticesFloatArray[0], GL_STATIC_DRAW);
		//glDrawArrays(GL_POINTS, 0, particles.size());
	}
	glBindVertexArray(0);
}

void ParticleSystem::drawParticle(RenderProps renderProps, ShaderParticle particle)
{

}

void ParticleSystem::addParticle(ShaderParticle particle)
{
	particle.active = true;
	particles.push_back(particle);
}

void ParticleSystem::createSpawner(ParticleSpawner particleSpawner)
{
	spawners.push_back(particleSpawner);
}