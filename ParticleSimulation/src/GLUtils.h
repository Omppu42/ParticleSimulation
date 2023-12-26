#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#define ROUND_TO_DECIMAL(num, decimals) round(num * pow(10.0f, decimals)) / pow(10.0f, decimals)

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);