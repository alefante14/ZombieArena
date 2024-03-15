#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H

#include <SFML/Graphics.hpp>
#include <map>

using namespace std;
using namespace sf;

class TextureHolder {
private:
	map <String, Texture> m_Textures;

	//this class has an object that is the same type as itself.
	//Beacuse it's static, it can be used through the class itself, without an istance of the class
	static TextureHolder* m_s_Instance;

public:
	TextureHolder();
	static Texture& GetTexture(string const& filename);
};

#endif // !TEXTURE_HOLDER_H
