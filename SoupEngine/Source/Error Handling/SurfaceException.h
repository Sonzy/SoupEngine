#pragma once
#include "Error Handling/Exceptions.h"

class SurfaceException : public Exception
{
public:
	SurfaceException(int line, const char* file, std::string note) noexcept;
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	const std::string& GetNote() const noexcept;
private:
	std::string note;
};