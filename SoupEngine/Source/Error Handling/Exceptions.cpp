#include "Exceptions.h"
#include "Error Handling/dxerr.h"
#include <sstream>

const char* HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "Error Code: 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " ( " << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "Error String: " << GetErrorString() << std::endl
		<< "Description: " << GetErrorDescription() << std::endl;
	if (!info.empty())
		oss << "\nError info: " << GetErrorInfo() << std::endl << std::endl;

	oss << GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* HrException::GetType() const noexcept
{
	return "Soup Graphics Exception";
}

HrException::HrException(int line, const char * file, HRESULT hr, std::vector<std::string> extrainfo) noexcept
	: Exception(line, file), hr(hr)
{
	//Join all messages into a single string (may have newlines)
	for (const auto& m : extrainfo)
	{
		info += m;
		info.push_back('\n');

	}
	//Remove final new line if it exists
	if (!info.empty())
	{
		info.pop_back();
	}
}

HRESULT HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string HrException::GetErrorString() const noexcept
{
	return DXGetErrorString(hr);
}

std::string HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription(hr, buf, sizeof(buf));
	return buf;
}

std::string HrException::GetErrorInfo() const noexcept
{
	return info;
}

const char* DeviceRemovedException::GetType() const noexcept
{
	return "Soup Graphics - Device removed";
}

InfoException::InfoException(int line, const char * file, std::vector<std::string> infoMessages) noexcept
	: Exception(line, file)
{
	//Join all messages with newlines into a single string
	for (const auto &m : infoMessages)
	{
		info += m;
		info.push_back('\n');
	}

	//Remove the last new line if it exists
	if (!info.empty())
		info.pop_back();
}

const char* InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\nErrorInfo: " << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* InfoException::GetType() const noexcept
{
	return "Soup Info Exception";
}

std::string InfoException::GetErrorInfo() const noexcept
{
	return info;
}