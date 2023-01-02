#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <map>

using namespace sf;
using namespace std;


class TextureHolder
{
private:
	// A map container de STL,
	// qui lie chaque string à un objet Texture
	map<string, Texture> m_Textures;

	// Un pointer a la clase TextureHolder
	static TextureHolder* m_s_Instance;

public:
	// Constructeur de TextureHolder
	TextureHolder();
	
	// Retourne une référence vers le seul objet TextureHolder
	static Texture& GetTexture(string const& filename);
};

#endif
