#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;
TextureHolder::TextureHolder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

Texture& TextureHolder::GetTexture(string const& filename)
{
	// R�cup�rer la r�f�rence de m_Texture en utilisant m_s_Instance
	auto& m = m_s_Instance->m_Textures;
	// auto est l'�quivalent de map<string, Texture>::iterator

	// Cr�er un it�rateur pour stocker key-value-pair (kvp)
	// et r�cup�rer le kvp correspondant � la cl� filename
	auto keyValuePair = m.find(filename);
	
	// est ce que la cl� a �t� trouv� ?
	if (keyValuePair != m.end())
	{
		// Si oui
		// Retourner la texture, la second partie du kvp
		return keyValuePair->second;
	}
	else
	{
		// sinon
		// Create une nouvelle kvp en utilisant un filename
		auto& texture = m[filename];
		// Load� la texture � partir du fichier 
		texture.loadFromFile(filename);
		// Retourner la texture
		return texture;
	}
}