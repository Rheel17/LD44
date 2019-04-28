/**
 * Copyright (c) 2019 Levi van Rheenen - All rights reserved
 */

#ifndef ERROR_H_
#define ERROR_H_

#include <stdexcept>
#include <string>

std::runtime_error showerror(const std::string& msg);

#endif
