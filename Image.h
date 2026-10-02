#pragma once
#include <vector>
#include <string>
#include <cstdint>

struct Pixel {
	uint8_t r, g, b;
	Pixel() : r(0), g(0), b(0) {}
	Pixel(uint8_t red, uint8_t green, uint8_t blue): r(red), g(green), b(blue) {}
};

class Image
{
	int width;
	int height;
	std::vector<std::vector<Pixel>> img;

public:
	Image() : width(0), height(0) {}
	Image(int pWidth, int pHeight): width(pWidth), height(pHeight), img(pHeight, std::vector<Pixel>(pWidth)) {}

	int getWidth() const;
	int getHeight() const;
	bool isEmpty() const;
	

	bool load(const std::string& path);
	bool save(const std::string& path) const;
	
	Image bilinearInterpolation(int newWidth, int newHeight);
	static std::string extensionOf(const std::string& path);
	static int clampInt(int v, int lo, int hi);
};

