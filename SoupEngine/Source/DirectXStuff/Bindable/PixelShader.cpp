#include "PixelShader.h"
#include "Error Handling/GraphicsErrorMacros.h"
#include <d3dcompiler.h>

PixelShader::PixelShader(Graphics & gfx, const std::wstring & path)
{
	INFOMAN(gfx);

	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	GFX_THROW_INFO(D3DReadFileToBlob(path.c_str(), &blob));
	GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &pixelShader));
}

void PixelShader::Bind(Graphics& gfx) noexcept
{
	GetContext(gfx)->PSSetShader(pixelShader.Get(), nullptr, 0);
}
