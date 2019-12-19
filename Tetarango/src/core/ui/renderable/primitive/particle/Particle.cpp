#include "Particle.h"


namespace AW
{
	const std::shared_ptr<Texture>& Particle::getTexture()
	{
		return particleTexture;
	}

	void Particle::setVertexShader(std::shared_ptr<ShaderReference> shader)
	{
		vertexShader = shader;
	}

	void Particle::setFragmentShader(std::shared_ptr<ShaderReference> shader)
	{
		fragmentShader = shader;
	}

	const std::shared_ptr<ShaderReference>& Particle::getVertexShader()
	{
		return vertexShader;
	}

	const std::shared_ptr<ShaderReference>& Particle::getFragmentShader()
	{
		return fragmentShader;
	}

	void Particle::onEnterFrame(const double& frameTime)
	{
		x += xV * frameTime;
		y += yV * frameTime;

		w = ogW * timeLeft / totalLife;
		h = ogH * timeLeft / totalLife;
	}

}
