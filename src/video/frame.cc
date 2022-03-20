#include <cstdlib>
#include <stdexcept>
#include "frame.hpp"

namespace video
{

Frame::Frame(const FrameSpec &spec, const bool extern_storage):
	spec(spec), extern_storage(extern_storage), buf(0)
{
	if (!extern_storage)
	{
		buf = new unsigned char[this->size()];
		
		if (buf == 0)
			throw std::runtime_error("Unable to allocate frame");
	}
}

Frame::~Frame()
{
	if (!extern_storage)
		delete[] buf;
}

}

