#pragma once

#include <vector>

template<class VertType>
class MeshBase
{
public:
	typedef VertType Type;

	MeshBase() {};
	virtual ~MeshBase() {};

	std::vector<VertType> hVectorData;
	std::size_t vertexCount;
};

