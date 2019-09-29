#pragma once
#include "DirectXStuff/Graphics.h"
#include <DirectXMath.h>

namespace Bind
{
	class Bindable;
	class IndexBuffer;
}

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

	virtual void Update(float deltaTime) noexcept
	{};

protected:
	template<class T>
	T* QueryBindable() noexcept
	{
		for (auto& pb : binds)
		{
			if (auto pt = dynamic_cast<T*>(pb.get()))
				return pt;
		}
		return nullptr;
	}


	void AddBind(std::unique_ptr<Bind::Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<Bind::IndexBuffer> iBuf) noexcept;


private:
	virtual const std::vector<std::unique_ptr<Bind::Bindable>>& GetStaticBinds() const noexcept = 0;
	const Bind::IndexBuffer* indexBuffer = nullptr;
	std::vector<std::unique_ptr<Bind::Bindable>> binds;
};