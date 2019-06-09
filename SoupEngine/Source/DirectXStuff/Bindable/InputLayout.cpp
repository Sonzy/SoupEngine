#include "InputLayout.h"
#include "Error Handling/GraphicsErrorMacros.h"

InputLayout::InputLayout(Graphics & gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob * vertexShaderByteCode)
{
	INFOMAN(gfx);
	GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(layout.data(), (UINT)layout.size(), vertexShaderByteCode->GetBufferPointer(), vertexShaderByteCode->GetBufferSize(), &inputLayout));
}

void InputLayout::Bind(Graphics & gfx) noexcept
{
	GetContext(gfx)->IASetInputLayout(inputLayout.Get());
}
