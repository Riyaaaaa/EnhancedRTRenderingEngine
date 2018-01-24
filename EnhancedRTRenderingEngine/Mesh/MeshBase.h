#pragma once

#include "../Structure/Structure.h"
#include <array>

template<std::size_t VertNum>
class MeshBase
{
public:
	MeshBase() {};
	virtual ~MeshBase() {};

	static constexpr std::size_t GetVertNum() { return VertNum; }
	std::array<Vertex3D, VertNum> hVectorData;
};

