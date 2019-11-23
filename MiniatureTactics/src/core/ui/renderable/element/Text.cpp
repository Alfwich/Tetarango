#include "Text.h"

namespace MT
{
	Text::Text() : Text(std::string(), 0) {}

	Text::Text(std::string fontName, int fontSize)
	{
		this->fontName = fontName;
		this->fontSize = fontSize;
		textureBindingKey = "text_texture_" + std::to_string(getId());
		config.renderMode = TextRenderMode::SlowAlpha;
		dynamicResizing = true;
		setTextColor(0xff, 0xff, 0xff);
		setBackgroundColor(0x00, 0x00, 0x00);
		enableSerialization<Text>();
	}

	void Text::renderText()
	{
		if (texture == nullptr)
		{
			if (config.font == nullptr && fontName.size() && fontSize > 0)
			{
				config.font = modules->font->createFont(fontName, fontSize);
			}

			texture = modules->texture->getEmptyTextureTextForKey(textureBindingKey);
			setTexture(texture);
		}

		auto textureText = std::static_pointer_cast<TextureText>(texture);
		if (!textureText)
		{
			Logger::instance()->logCritical("Text::Failed to render text. Internal Texture is not a TextureText.");
			return;
		}

		textureText->updateConfiguration(config);
		textureText->rebind();

		if (dynamicResizing && texture->isLoaded())
		{
			setSize(texture->getWidth(), texture->getHeight());
		}
	}

	void Text::setDynamicResizing(bool flag)
	{
		dynamicResizing = flag;
	}

	void Text::setFont(std::string name, int fontSize)
	{
		if (!name.empty() && fontSize > 0)
		{
			fontName = name;
			this->fontSize = fontSize;
			config.font = modules->font->createFont(name, fontSize);
		}
	}

	void Text::setFontSize(int fontSize)
	{
		if (config.font->getFontSize() == fontSize)
		{
			return;
		}

		this->fontSize = fontSize;
		config.font->setFontSize(fontSize);

		renderText();
	}

	int Text::getFontSize()
	{
		return config.font->getFontSize();
	}

	void Text::setTextRenderMode(TextRenderMode mode)
	{
		config.renderMode = mode;
	}

	void Text::setTextColor(int r, int g, int b, int a)
	{
		config.textColor.r = r;
		config.textColor.g = g;
		config.textColor.b = b;
		config.textColor.a = a;
	}

	void Text::setBackgroundColor(int r, int g, int b, int a)
	{
		config.backgroundColor.r = r;
		config.backgroundColor.g = g;
		config.backgroundColor.b = b;
		config.backgroundColor.a = a;
	}

	Color Text::getBackgroundColor()
	{
		return config.backgroundColor;
	}

	void Text::setText(std::string text)
	{
		if (config.text == text)
		{
			return;
		}

		config.text = text;
		renderText();
	}

	std::string Text::getText()
	{
		return config.text;
	}

	void Text::onInitialAttach()
	{
		renderText();
	}

	std::shared_ptr<SerializationClient> Text::doSerialize(SerializationHint hint)
	{
		const auto client = serializationClient->getClient("__text__", hint);
		config.renderMode = (TextRenderMode)client->serializeInt("config.mode", (int)config.renderMode);
		config.text = client->serializeString("config.text", config.text);

		config.textColor.r = client->serializeInt("config.tc.r", (int)config.textColor.r);
		config.textColor.g = client->serializeInt("config.tc.g", (int)config.textColor.g);
		config.textColor.b = client->serializeInt("config.tc.b", (int)config.textColor.b);
		config.textColor.a = client->serializeInt("config.tc.a", (int)config.textColor.a);

		config.backgroundColor.r = client->serializeInt("config.bc.r", (int)config.backgroundColor.r);
		config.backgroundColor.g = client->serializeInt("config.bc.g", (int)config.backgroundColor.g);
		config.backgroundColor.b = client->serializeInt("config.bc.b", (int)config.backgroundColor.b);
		config.backgroundColor.a = client->serializeInt("config.bc.a", (int)config.backgroundColor.a);

		fontName = client->serializeString("font.name", fontName);
		fontSize = client->serializeInt("font.size", fontSize);

		return Element::doSerialize(hint);
	}
}
