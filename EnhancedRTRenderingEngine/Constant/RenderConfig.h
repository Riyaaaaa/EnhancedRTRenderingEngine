#pragma once

enum class VertexProperty {
	FloatRGBA,
	FloatRGB,
	FloatRG
};

enum class VertexPrimitiveType {
	TRIANGLELIST,
	TRIANGLESTRIP
};

enum class RenderingState {
	NONE,
	FAILED,
	RENDER_READIED
};

enum MSAAQualityType {
	RAW_QUALITY
};