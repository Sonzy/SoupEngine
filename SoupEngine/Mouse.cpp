#include "Mouse.h"


std::pair<int, int> Mouse::GetPos() const noexcept
{
	return std::make_pair(x, y);
}

int Mouse::GetXPos() const noexcept
{
	return x;
}

int Mouse::GetYPos() const noexcept
{
	return y;
}

bool Mouse::IsInWindow() const noexcept
{
	return inWindow;
}

bool Mouse::IsLeftPressed() const noexcept
{
	return leftPressed;
}

bool Mouse::IsRightPressed() const noexcept
{
	return rightPressed;
}

bool Mouse::IsEmpty() const noexcept
{
	return buffer.empty();
}

void Mouse::Flush() noexcept
{
	buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int x, int y) noexcept
{
	this->x = x;
	this->y = y;

	buffer.push(Mouse::Event(Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	leftPressed = true;

	buffer.push(Mouse::Event(Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	rightPressed = true;

	buffer.push(Mouse::Event(Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	inWindow = true;

	buffer.push(Mouse::Event(Event::Type::EnterWindow, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	inWindow = false;

	buffer.push(Mouse::Event(Event::Type::LeaveWindow, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	leftPressed = false;

	buffer.push(Mouse::Event(Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	rightPressed = false;

	buffer.push(Mouse::Event(Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}

Mouse::Event Mouse::ReadEvent() noexcept
{
	if (buffer.size() > 0)
	{
		Mouse::Event mouseEvent = buffer.front();
		buffer.pop();
		return mouseEvent;
	}
	return Mouse::Event();
}

Mouse::Event::Event()
	: type(Type::Invalid), leftPressed(false), rightPressed(false), x(0), y(0)
{

}

Mouse::Event::Event(Type type, const Mouse & parent) noexcept
	: type(type), leftPressed(parent.leftPressed), rightPressed(parent.rightPressed), x(parent.x), y(parent.y)
{
}

