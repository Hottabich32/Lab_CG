#define _CRT_SECURE_NO_WARNINGS
#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

int Image::getWidth() const {
	return width;
}

int Image::getHeight() const {
	return height;
}

bool Image::isEmpty() const {
	return width == 0 || height == 0;
}

bool Image::load(const std::string& path) {
	int w{}, h{}, n{};
	unsigned char* imgData = stbi_load(path.c_str(), &w, &h, &n, 3);
	if (!imgData)
		return false;

	width = w;
	height = h;
	img.assign(h, std::vector<Pixel>(w));

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			const unsigned char* p = imgData + ((size_t)i * w + j) * 3;
			img[i][j] = Pixel(p[0], p[1], p[2]);
		}
	}

	stbi_image_free(imgData);
	return true;
}

bool Image::save(const std::string& path) const {
	if (isEmpty())
		return false;

	std::string ext = extensionOf(path);
	std::vector<unsigned char> raw((size_t)width * height * 3);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			size_t p = ((size_t)i * width + j) * 3;
			raw[p] = img[i][j].r;
			raw[p + 1] = img[i][j].g;
			raw[p + 2] = img[i][j].b;
		}
	}

	if (ext == "png")
		return stbi_write_png(path.c_str(), width, height, 3, raw.data(), width * 3) != 0;

	if (ext == "jpg" || ext == "jpeg")
		return stbi_write_jpg(path.c_str(), width, height, 3, raw.data(), 95) != 0;

	return false;
}

Image Image::bilinearInterpolation(int newWidth, int newHeight) {
	if (isEmpty() || newWidth <= 0 || newHeight <= 0)
		return Image();

	Image result(newWidth, newHeight);

	double kX = (double)width / newWidth;
	double kY = (double)height / newHeight;

	for (int y = 0; y < newHeight; ++y) {
		double oldY = (y + 0.5) * kY - 0.5;
		if (oldY < 0)
			oldY = 0;

		int y0 = (int)oldY;
		int y1 = clampValue(y0 + 1, 0, height - 1);
		y0 = clampValue(y0, 0, height - 1);
		double dy = oldY - y0;

		for (int x = 0; x < newWidth; ++x) {
			double oldX = (x + 0.5) * kX - 0.5;
			if (oldX < 0)
				oldX = 0;

			int x0 = (int)oldX;
			int x1 = clampValue(x0 + 1, 0, width - 1);
			x0 = clampValue(x0, 0, width - 1);
			double dx = oldX - x0;

			const Pixel& p00 = img[y0][x0];
			const Pixel& p01 = img[y0][x1];
			const Pixel& p10 = img[y1][x0];
			const Pixel& p11 = img[y1][x1];

			double w00 = (1 - dx) * (1 - dy);
			double w01 = dx * (1 - dy);
			double w10 = (1 - dx) * dy;
			double w11 = dx * dy;

			double r = p00.r * w00 + p01.r * w01 + p10.r * w10 + p11.r * w11;
			double g = p00.g * w00 + p01.g * w01 + p10.g * w10 + p11.g * w11;
			double b = p00.b * w00 + p01.b * w01 + p10.b * w10 + p11.b * w11;

			result.img[y][x] = Pixel((uint8_t)(r + 0.5), (uint8_t)(g + 0.5), (uint8_t)(b + 0.5));

		}
	}

	return result;
}

std::string Image::extensionOf(const std::string& path) {
	size_t dot = path.find_last_of('.');

	if (dot == std::string::npos)
		return "";

	std::string ext = path.substr(dot + 1);

	for (size_t i = 0; i < ext.size(); ++i)
		if (ext[i] >= 'A' && ext[i] <= 'Z') ext[i] = ext[i] - 'A' + 'a';
	return ext;
}

int Image::clampValue(int v, int lo, int hi) {
	if (v < lo)
		return lo;
	if (v > hi)
		return hi;
	return v;
}
