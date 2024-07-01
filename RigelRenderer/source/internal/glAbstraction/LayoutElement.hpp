#pragma once

#include "glew.h"

#define Vec1Element LayoutElement(1, GL_FLOAT, false, sizeof(float), 0)
#define Vec2Element LayoutElement(2, GL_FLOAT, false, sizeof(float) * 2, 0)
#define Vec3Element LayoutElement(3, GL_FLOAT, false, sizeof(float) * 3, 0)
#define Vec4Element LayoutElement(4, GL_FLOAT, false, sizeof(float) * 4, 0)

namespace rgr
{
	struct LayoutElement
	{
	public:
		const int count;
		const int type;
		const bool normalized;
		const int stride;
		const int offset;

		LayoutElement(int count, int type, bool normalized, int stride, int offset);
		~LayoutElement();
	};
}