/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <iostream>
#include <vector>

struct PixelColor {
	float r, g, b, a;

	inline unsigned Red()   const { return (unsigned) (r * 255); }
	inline unsigned Green() const { return (unsigned) (g * 255); }
	inline unsigned Blue()  const { return (unsigned) (b * 255); }
	inline unsigned Alpha() const { return (unsigned) (a * 255); }

	inline unsigned RGBA()  const { return (Red() << 24) | (Green() << 16) | (Blue() << 8) | Alpha(); }
	inline operator unsigned() const { return RGBA(); }

	inline void SetRed  (unsigned r) { this->r = r / 255.0f; }
	inline void SetGreen(unsigned g) { this->g = g / 255.0f; }
	inline void SetBlue (unsigned b) { this->b = b / 255.0f; }
	inline void SetAlpha(unsigned a) { this->a = a / 255.0f; }

	inline void SetRGBA(unsigned rgba) {
		SetRed  ((rgba >> 24) & 0xFF);
		SetGreen((rgba >> 16) & 0xFF);
		SetBlue ((rgba >>  8) & 0xFF);
		SetAlpha((rgba >>  0) & 0xFF);
	}
};

class Image {

public:
	Image(unsigned width, unsigned height);
	Image(const std::string& filename);
	Image(std::istream& input);

	inline const PixelColor& At(unsigned x, unsigned y) const { return _pixels[x + _width * y]; }
	inline       PixelColor& At(unsigned x, unsigned y)       { return _pixels[x + _width * y]; }

	inline unsigned Width()  const { return _width;  }
	inline unsigned Height() const { return _height; }

	const float *Data() const { return reinterpret_cast<const float *>(_pixels.data()); };

	Image SubImage(unsigned x, unsigned y, unsigned width, unsigned height) const;

	void Save(const std::string& filename) const;

private:
	void _ReadFromStream(std::istream& input);
	bool _Validate(std::istream& input);
	void _ReadStream(std::istream& input);

	std::vector<PixelColor> _pixels;

	unsigned _width;
	unsigned _height;

};

#endif
