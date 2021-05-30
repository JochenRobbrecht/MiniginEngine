#pragma once
// ====================================
// This class was created by Kevin Hoefman and Marleen De Wandel
// Professors at Howest Digital Arts and Entertainment
// ====================================
//#ifndef ENGINE_TEXTURE_H
//#define ENGINE_TEXTURE_H

#include <string>
#include <SDL_opengl.h>
#include "Structs.h"

struct SDL_Surface;
typedef struct _TTF_Font TTF_Font;

class Texture
{
public:
	Texture(const std::string& path);
	Texture(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor);
	Texture(const std::string& text, TTF_Font* pFont, const Color4f& textColor);
	~Texture();

	void Draw(const Rectf& destRect = Rectf{}, const Rectf& srcRect = Rectf{}) const;

	const float GetWidth() const;
	const float GetHeight() const;

private:
	GLuint m_Id;
	bool m_CreationOk;
	float m_Width;
	float m_Height;

	void CreateImageFromString(const std::string& path);
	void CreateFromString(const std::string& text, TTF_Font* pFont, const Color4f& textColor);
	void CreateFromString(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor);
	void CreateFromSurface(SDL_Surface* pSurface);
};


//#endif // !ENGINE_TEXTURE_H


