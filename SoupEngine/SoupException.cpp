#include "SoupException.h"
#include <sstream>


SoupException::SoupException(int line, const char * file) noexcept
	: line(line), file(file)
{
}

const char* SoupException::what() const noexcept
{
	std::ostringstream stringStream;

	stringStream << GetType() << std::endl
		<< GetOriginString();

	whatBuffer = stringStream.str();
	return whatBuffer.c_str();
}

const char * SoupException::GetType() const noexcept
{
	return "Soup Exception";
}

int SoupException::GetLine() const noexcept
{
	return line;
}

const std::string & SoupException::GetFile() const noexcept
{
	return file;
}

std::string SoupException::GetOriginString() const noexcept
{
	std::ostringstream stringStream;
	stringStream << "File: " << file << std::endl
		<< "Line: " << line;
	return stringStream.str();
}
