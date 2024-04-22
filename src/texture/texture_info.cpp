#include <texture/texture_info.h>

uint32_t TextureInfo::counter = 0;

TextureInfo::TextureInfo() :
	id(counter++) {}

bool operator < (const TextureInfo& a, const TextureInfo& b) {
	return a.id < b.id;
}

FileTextureInfo::FileTextureInfo(const std::string& filename) :
	filename(filename) {}

TextTextureInfo::TextTextureInfo(const std::string& text) :
	text(text) {}