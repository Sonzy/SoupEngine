#pragma once
#include "DirectXStuff/Graphics.h"
#include <DirectXMath.h>

class Bindable;
class IndexBuffer;

class Drawable
{
public:
	void Draw(Graphics& gfx) const noexcept;
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0; //=0 makes this class abstract, and has to implement this

	virtual void Update(float deltaTime) noexcept = 0;
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> iBuf) noexcept;

	virtual ~Drawable() = default;

private:
	const IndexBuffer* indexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};