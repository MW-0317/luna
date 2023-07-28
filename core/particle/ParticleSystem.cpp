#include "ParticleSystem.h"

using namespace luna;

ParticleSystem::ParticleSystem(ParticleSystemProps props) 
	: Object(props.mesh, props.shader, props.position, props.scale)
{
	systemProps = props;
}

void ParticleSystem::frameUpdate(float deltatime)
{
	//draw();
}

void ParticleSystem::tickUpdate(float deltatime)
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i].lifeRemaining -= deltatime;
	}
}

void ParticleSystem::draw()
{
	std::cout << "Just making sure d" << std::endl;
	for (int i = 0; i < particles.size(); i++)
		drawParticle(i);
}

void ParticleSystem::drawParticle(int index)
{
	std::cout << "Just making sure" << std::endl;
	Particle currentParticle = particles[index];
	if (!currentParticle.active)
		return;
	glm::mat4 particleModel = glm::mat4(1.0f);
	float currentSize = glm::mix(
		currentParticle.sizeBegin, 
		currentParticle.sizeEnd, 
		currentParticle.lifeRemaining
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
	particles.push_back(particle);
}

void ParticleSystem::createSpawner(ParticleSpawnerProps particleProps)
{

}