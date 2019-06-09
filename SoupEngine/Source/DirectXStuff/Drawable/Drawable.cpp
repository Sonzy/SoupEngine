#include "Drawable.h"
#include "Error Handling/GraphicsErrorMacros.h"
#include "DirectXStuff/Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo>

DirectX::XMMATRIX Drawable::GetTransformXM() const noexcept
{
	return DirectX::XMMATRIX();
}

void Drawable::Draw(Graphics & gfx) const noexcept
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	
	gfx.DrawIndexed(indexBuffer->GetCount());
}

void Drawable::Update(float deltaTime) noexcept
{
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept
{
	//Check that we arent trying to add an index buffer
	assert("MUST use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> iBuf) noexcept
{
	//Check that we havent added an index buffer
	assert("Attempting to add index buffer a second time" && indexBuffer == nullptr);
	indexBuffer = iBuf.get();
	binds.push_back(std::move(iBuf));
}
