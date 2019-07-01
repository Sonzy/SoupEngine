#pragma once
#include "Drawable.h"
#include "DirectXStuff/Bindable/IndexBuffer.h"

template<class T>
class DrawableBase : public Drawable
{
protected:

	static bool IsStaticInitialised() noexcept
	{
		return !staticBinds.empty();
	}
	
	static void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept
	{
		assert("USE ADD INDEX BUFFER TO BIND INDEX BUFFER" && typeid(*bind) != typeid(Bindable));
		staticBinds.push_back(std::move(bind));
	}

	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuffer) noexcept
	{
		assert("Attempting to add a 2nd index buffer" && indexBuffer == nullptr);
		indexBuffer = ibuffer.get();
		staticBinds.push_back(std::move(ibuffer));
	}

	void SetIndexFromStatic() noexcept
	{
		assert("Attempting to add a 2nd index buffer" && indexBuffer == nullptr);
		for (const auto& b : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
			{
				indexBuffer = p;
				return;
			}
		}
		assert("Failed to find index buffer in static binds" && indexBuffer != nullptr);
	}

private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept
	{
		return staticBinds;
	}

	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;