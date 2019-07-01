#include "SurfaceException.h"
#include <sstream>

SurfaceException::SurfaceException(int line, const char * file, std::string note) noexcept
	: Exception(line, file), note(std::move(note))
{
}

const char* SurfaceException::what() const noexcept
{
	std::ostringstream oss;
	oss << Exception::what() << std::endl
		<< "[Note]" << GetNote();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* SurfaceException::GetType() const noexcept
{
	return "Surface Exception";
}

const std::string & SurfaceException::GetNote() const noexcept
{
	return note;
}
