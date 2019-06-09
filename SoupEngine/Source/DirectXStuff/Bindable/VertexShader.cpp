#include "VertexShader.h"
#include "Error Handling/GraphicsErrorMacros.h"
#include <d3dcompiler.h>

VertexShader::VertexShader(Graphics & gfx, const std::wstring & path)
{
	INFOMAN(gfx);

	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &blob));
	GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vertexShader));
}

ID3DBlob * VertexShader::GetByteCode() const noexcept
{
	return blob.Get();
}

void VertexShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->VSSetShader(vertexShader.Get(), nullptr, 0);
}
