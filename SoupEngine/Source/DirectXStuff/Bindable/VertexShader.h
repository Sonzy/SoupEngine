#pragma once
#include "Bindable.h"

namespace Bind
{
	class VertexShader : public Bind::Bindable
	{
	public:
		VertexShader(Graphics& gfx, const std::wstring& path);
		void Bind(Graphics& gfx) noexcept override;
		ID3DBlob* GetByteCode() const noexcept;
	protected:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
		Microsoft::WRL::ComPtr<ID3DBlob> blob;
	};
}