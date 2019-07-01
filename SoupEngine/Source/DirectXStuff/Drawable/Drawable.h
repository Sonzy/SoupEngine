#pragma once
#include "DirectXStuff/Graphics.h"
#include <DirectXMath.h>

class Bindable;
class IndexBuffer;

class Drawable
{
	template<class T>
	friend class DrawableBase;
public:
	void Draw(Graphics& gfx) const noexcept;
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0; //=0 makes this class abstract, and has to implement this

	virtual void Update(float deltaTime) noexcept = 0;

protected:
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> iBuf) noexcept;


private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
	const IndexBuffer* indexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};