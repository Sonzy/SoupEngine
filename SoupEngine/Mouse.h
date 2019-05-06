#pragma once
#include <utility>
#include <queue>

class Mouse
{
	friend class Window;
public:

	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			EnterWindow,
			LeaveWindow,
			Invalid
		};

		Event();
		Event(Type type, const Mouse& parent) noexcept;

		bool IsValid() const noexcept { return type != Type::Invalid; };
		Type GetType() const noexcept { return type; };
		std::pair<int, int> GetPosition() const noexcept { return std::make_pair(x, y); };
		bool IsMouseLeftPressed() const noexcept { return leftPressed;};
		bool IsMouseRightPressed() const noexcept { return rightPressed; };
		int GetXPos() const noexcept { return x; };
		int GetYPos() const noexcept { return y; };
	private:
		Type type;
		bool leftPressed;
		bool rightPressed;

		// Mouse pos
		int x;
		int y;
	};

	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	
	std::pair<int, int> GetPos() const noexcept;
	int GetXPos() const noexcept;
	int GetYPos() const noexcept;
	bool IsInWindow() const noexcept;
	bool IsLeftPressed() const noexcept;
	bool IsRightPressed() const noexcept;
	Mouse::Event ReadEvent() noexcept;
	bool IsEmpty() const noexcept;

	void Flush() noexcept;

private:
	void OnMouseMove(int x, int y) noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseLeave() noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;

	static constexpr unsigned int bufferSize = 16u;
	int x;
	int y;
	bool inWindow;
	bool leftPressed;
	bool rightPressed;
	std::queue<Mouse::Event> buffer;
};

