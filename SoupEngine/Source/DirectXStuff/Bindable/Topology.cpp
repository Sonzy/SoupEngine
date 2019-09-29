#include "Topology.h"

using namespace Bind;

Topology::Topology(Graphics & gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	: type(type)
{
}

void Topology::Bind(Graphics& gfx) noexcept
{
	//Setup topology to triangle list
	GetContext(gfx)->IASetPrimitiveTopology(type);
}