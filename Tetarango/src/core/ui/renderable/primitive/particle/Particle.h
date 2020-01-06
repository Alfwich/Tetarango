#pragma once



#include "util/Rect.h"
#include "engine/module/display/Texture.h"
#include "engine/module/shader/ShaderReference.h"

namespace AW
{
	class Particle
	{
	protected:
		std::shared_ptr<Texture> particleTexture;
		std::shared_ptr<ShaderReference> fragmentShader, vertexShader;

	public:
		Rect clip;

		int state = 0, zIndex = 0,
			cModR = 0xff, cModG = 0xff, cModB = 0xff;
		
		bool shouldDecay = true;

		double ogX = 0.0, ogY = 0.0, ogW = 0.0, ogH = 0.0,
			x = 0.0, y = 0.0, w = 0.0, h = 0.0, r = 0.0,
			ogXV = 0.0, ogYV = 0.0, xV = 0.0, yV = 0.0,
			totalLife = 0.0, timeLeft = 0.0,
			alphaMod = 255.0;


		const std::shared_ptr<Texture>& getTexture();
		void setVertexShader(std::shared_ptr<ShaderReference> shader);
		void setFragmentShader(std::shared_ptr<ShaderReference> shader);
		const std::shared_ptr<ShaderReference>& getVertexShader();
		const std::shared_ptr<ShaderReference>& getFragmentShader();

		virtual void onEnterFrame(const double& frameTime);
	};
}

