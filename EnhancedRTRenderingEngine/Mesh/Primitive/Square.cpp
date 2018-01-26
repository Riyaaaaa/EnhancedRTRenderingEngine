#include "stdafx.h"
#include "Square.h"

static const float      POLYGON_EDGE_LENGTH = 0.1f;
static const float      POLYGON_DEPTH = 0.5f;

Square::Square()
{
	hVectorData = std::vector<SimpleVertex>{
		SimpleVertex{ { +POLYGON_EDGE_LENGTH,+POLYGON_EDGE_LENGTH, POLYGON_DEPTH },{ 0.8f,0.0f,0.2f,1.0f } },
		SimpleVertex{ { +POLYGON_EDGE_LENGTH,-POLYGON_EDGE_LENGTH, POLYGON_DEPTH },{ 0.8f,0.0f,0.2f,1.0f } },
		SimpleVertex{ { -POLYGON_EDGE_LENGTH,-POLYGON_EDGE_LENGTH, POLYGON_DEPTH },{ 0.8f,0.0f,0.2f,1.0f } },

		SimpleVertex{ { +POLYGON_EDGE_LENGTH,+POLYGON_EDGE_LENGTH, POLYGON_DEPTH },{ 0.8f,0.0f,0.2f,1.0f } },
		SimpleVertex{ { -POLYGON_EDGE_LENGTH,-POLYGON_EDGE_LENGTH, POLYGON_DEPTH },{ 0.8f,0.0f,0.2f,1.0f } },
		SimpleVertex{ { -POLYGON_EDGE_LENGTH,+POLYGON_EDGE_LENGTH, POLYGON_DEPTH },{ 0.8f,0.0f,0.2f,1.0f } },
	};

	vertexCount = hVectorData.size();
}


Square::~Square()
{
}
