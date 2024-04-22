#pragma once

#include <shape/shapes.h>
#include <string>

struct TextureInfo {
	static uint32_t counter;
	uint32_t id;
	TextureInfo();
	~TextureInfo() {}
	friend bool operator < (const TextureInfo& a, const TextureInfo& b);
};

struct FileTextureInfo : TextureInfo {
	const std::string filename;
	FileTextureInfo(const std::string& filename);
};

struct TextTextureInfo : TextureInfo {
	const std::string text;
	TextTextureInfo(const std::string& text);
};