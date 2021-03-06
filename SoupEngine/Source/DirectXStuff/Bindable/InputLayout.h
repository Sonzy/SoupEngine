#pragma once
#include "Bindable.h"

namespace Bind
{
	class InputLayout : public Bind::Bindable
	{
	public:
		InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexShaderByteCode);
		void Bind(Graphics& gfx) noexcept override;

	private:
		//Describe the input layout for the vertex shader
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	};
}
