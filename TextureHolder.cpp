#include "TextureHolder.h"
#include <assert.h>

using namespace sf;
using namespace std;

TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder() {
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

Texture& TextureHolder::GetTexture(string const& filename) {

	//get a reference of m_Textures ( map<String,Texture> )
	auto& m = m_s_Instance->m_Textures;

	auto keyValuePair = m.find(filename);

	if (keyValuePair != m.end()) {
		return keyValuePair->second;
	}
	else {
		//Filename not found
		//create a new key value pair using the filename
		auto& texture = m[filename];    //a bit trippy, but here we are making a reference to the second value while we are creating a pair
		texture.loadFromFile(filename);
		return texture;
	}

}