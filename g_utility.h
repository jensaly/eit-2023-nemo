#ifndef EIT_G_UTILITY_H
#define EIT_G_UTILITY_H

#include "libs/imgui/imgui.h"
#include "libs/imgui/backends/imgui_impl_sdl.h"
#include "libs/imgui/backends/imgui_impl_opengl3.h"
#include "libs/imgui/misc/cpp/imgui_stdlib.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "libs/imgui/imgui_internal.h"
#include "libs/implot/implot.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
//#include <SDL2/SDL_image.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif
#pragma once

#endif //EIT_G_UTILITY_H
