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

};

#endif
