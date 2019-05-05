#pragma once
#include <queue>
#include <bitset>
#include <optional>


class Keyboard
{
	friend class Window;
public:

	class Event
	{
		public: 
			enum class Type
			{
				Press,
				Release,
				Invalid
			};

			Event();
			Event(Type type, unsigned char keycode) noexcept;

			bool IsPressEvent() const noexcept;
			bool IsReleaseEvent() const noexcept;
			bool IsValid() const noexcept;
			unsigned char GetKeyCode() const noexcept;
	private:
		Type type;
		unsigned char keycode;

	};



	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	// Key events

	bool IsKeyPressed(unsigned char keycode);
	Event ReadKey() noexcept;
	bool IsKeyEmpty() const noexcept;
	void ClearKey() noexcept;

	// Char events

	char ReadChar() noexcept;
	bool IsCharEmpty() const noexcept;
	void ClearChar() noexcept;
	void Flush() noexcept;

	// Autorepeat stuff

	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool IsAutorepeatEnabled() const noexcept;

private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;

	void ClearState() noexcept;

	template<typename T>
	static void TrimBuffer(std::queue<T> &buffer) noexcept;

private:
	// Number of keys to track
	static constexpr unsigned int nKeys = 256u;
	// Size of the buffers
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	// Bitset containing all the keystates
	std::bitset<nKeys> keystates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};

template<typename T>
inline void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
}
