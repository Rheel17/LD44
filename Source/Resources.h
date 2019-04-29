/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef RESOURCES_H_
#define RESOURCES_H_

#include <map>

#include "Image.h"
#include "GL.h"

class Resources {

private:
	Resources() = delete;
	~Resources() = delete;

public:
	static const Image level;

	static const Image font0;
	static const Image font1;
	static const Image font2;
	static const Image font3;
	static const Image font4;
	static const Image font5;
	static const Image font6;
	static const Image font7;
	static const Image font8;
	static const Image font9;

};

#endif
