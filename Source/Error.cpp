/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#include "Error.h"

#include <windows.h>

std::runtime_error showerror(const std::string& msg) {
	MessageBoxA(nullptr, msg.c_str(), "Error!", MB_OK | MB_ICONERROR);
	return std::runtime_error(msg);
}
