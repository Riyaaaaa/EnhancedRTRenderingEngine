#pragma once

#include <vector>

template<class VertType>
class MeshBase
{
public:
	MeshBase() {};
	virtual ~MeshBase() {};

	std::vector<VertType> hVectorData;
	std::size_t vertexCount;
};

