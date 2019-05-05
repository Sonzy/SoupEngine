#include "Keyboard.h"


bool Keyboard::IsKeyPressed(unsigned char keycode)
{
	return keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	//If there are keys in the buffer, take the first keyevent from the queue and return it
	if (keyBuffer.size() > 0)
	{
		Keyboard::Event keyEvent = keyBuffer.front();
		keyBuffer.pop();
		return keyEvent;
	}

	return Keyboard::Event();
}

bool Keyboard::IsKeyEmpty() const noexcept
{
	return keyBuffer.empty();
}

void Keyboard::ClearKey() noexcept
{
	keyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept
{
	//If there are chars in the buffer, take the first char from the queue and return it
	if (charBuffer.size() > 0)
	{
		unsigned char charCode = charBuffer.front();
		charBuffer.pop();
		return charCode;
	}

	return 0;
}

bool Keyboard::IsCharEmpty() const noexcept
{
	return charBuffer.empty();
}

void Keyboard::ClearChar() noexcept
{
	charBuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	ClearKey();
	ClearChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
	autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
	autorepeatEnabled = false;
}

bool Keyboard::IsAutorepeatEnabled() const noexcept
{
	return autorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	//Update the key state
	keystates[keycode] = true;

	//Add to the key buffer
	keyBuffer.push(Keyboard::Event(Event::Type::Press, keycode));

	//Resize the buffer if needed
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	//Update the key state
	keystates[keycode] = false;

	//Add to the key buffer
	keyBuffer.push(Keyboard::Event(Event::Type::Release, keycode));

	//Resize the buffer if needed
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(char character) noexcept
{
	charBuffer.push(character);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearState() noexcept
{
	keystates.reset();
}

Keyboard::Event::Event()
	: type(Type::Invalid), keycode(0)
{
}

Keyboard::Event::Event(Type type, unsigned char keycode) noexcept
	: type(type), keycode(keycode)
{
}

bool Keyboard::Event::IsPressEvent() const noexcept
{
	return type == Type::Press;
}

bool Keyboard::Event::IsReleaseEvent() const noexcept
{
	return type == Type::Release;
}

bool Keyboard::Event::IsValid() const noexcept
{
	return type != Type::Invalid;
}

unsigned char Keyboard::Event::GetKeyCode() const noexcept
{
	return keycode;
}
