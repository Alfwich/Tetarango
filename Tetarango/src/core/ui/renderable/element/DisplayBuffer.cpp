#include "DisplayBuffer.h"

namespace AW
{
	DisplayBuffer::DisplayBuffer()
	{
		renderMode = RenderMode::CachedElement;
		renderUpdateMode = RenderUpdateMode::WhenDirty;
		registerGameObject<DisplayBuffer>();
	}

	void DisplayBuffer::onLoadResources()
	{
		modules->shader->loadShader("res/shader/fragment/cached.glsl", "f-cached");
		modules->shader->registerShaderComposition({ "f-cached", "f-texture", "f-color" }, "cached");
	}

	void DisplayBuffer::onBindShaders()
	{
		if (fragmentShader == nullptr)
		{
			fragmentShader = modules->shader->getShader({ "cached" });
		}
	}

	void DisplayBuffer::setClearColor(int r, int g, int b, int a)
	{
		if (clearColor == nullptr)
		{
			clearColor = std::make_shared <Color>(r, g, b, a);
		}
		else
		{
			clearColor->r = r;
			clearColor->g = g;
			clearColor->b = b;
			clearColor->a = a;
		}
	}

	void DisplayBuffer::setClearColor(const Color & c)
	{
		setClearColor(c.r, c.g, c.b, c.a);
	}

	const std::shared_ptr<Color>& DisplayBuffer::getClearColor()
	{
		return clearColor;
	}

	void DisplayBuffer::rebuildInternalTexture()
	{
		setTexture(std::shared_ptr<Texture>());
	}
}