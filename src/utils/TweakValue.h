#pragma once
#include <diesel.h>

#ifdef DEBUG
#define _TV(value) _TweakValue(__FILE__, __COUNTER__, value)

void ReloadChangedTweakableValues();

#else
#define _TV(value) value

void ReloadChangedTweakableValues() {}

#endif

float _TweakValue(const char* file, size_t counter, float orgValue);
ds::vec2 _TweakValue(const char* file, size_t counter, const ds::vec2& orgValue);
ds::vec3 _TweakValue(const char* file, size_t counter, const ds::vec3& orgValue);
ds::vec4 _TweakValue(const char* file, size_t counter, const ds::vec4& orgValue);
ds::Color _TweakValue(const char* file, size_t counter, const ds::Color& orgValue);


