#pragma once

#include "Scene/MeshObject.h"
#include "Structure/Structure.h"

#include <vector>

namespace SceneUtils {
	template<class PrimitiveType>
	MeshObject<Vertex3D> CreatePrimitiveMeshObject() {
		RenderingContext context{ "VertexShader", "PixelShader", CreateVertexLayout < PrimitiveType::Type > (), VertexPrimitiveType::TRIANGLESTRIP };
		MeshObject<Vertex3D> mesh(PrimitiveType{}, context);

		return mesh;
	}
}
