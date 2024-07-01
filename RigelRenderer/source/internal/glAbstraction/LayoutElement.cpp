#include "LayoutElement.hpp"

namespace rgr
{
	LayoutElement::LayoutElement(int count, int type, bool normalized, int stride, int offset) :
		count(count), type(type), normalized(normalized), stride(stride), offset(offset) {}

	LayoutElement::~LayoutElement()
	{

	}
}
