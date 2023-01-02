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
	// Récupérer la référence de m_Texture en utilisant m_s_Instance
	auto& m = m_s_Instance->m_Textures;
	// auto est l'équivalent de map<string, Texture>::iterator

	// Créer un itérateur pour stocker key-value-pair (kvp)
	// et récupérer le kvp correspondant à la clé filename
	auto keyValuePair = m.find(filename);
	
	// est ce que la clé a été trouvé ?
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
		// Loadé la texture à partir du fichier 
		texture.loadFromFile(filename);
		// Retourner la texture
		return texture;
	}
}