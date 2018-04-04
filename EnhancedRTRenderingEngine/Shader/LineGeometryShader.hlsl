
struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float thickness : THICKNESS;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
    float4 col : COLOR;
};

[maxvertexcount(6)]
void main(
	line VSOutput input[2],
	inout TriangleStream< GSOutput > output
)
{
    /*for (uint i = 0; i < 2; i++)
    {
        float offset = input[i].thickness / 2.0f;

        {
            GSOutput element;
            element.pos = input[i].pos + float4(-offset, 0.0f, 0.0f, 0.0f);
            element.col = input[i].col;
            output.Append(element);
        }

        {
            GSOutput element;
            element.pos = input[i].pos + float4(0.0f, input[i].thickness, 0.0f, 0.0f);
            element.col = input[i].col;
            output.Append(element);
        }

        {
            GSOutput element;
            element.pos = input[i].pos + float4(offset, 0.0f, 0.0f, 0.0f);
            element.col = input[i].col;
            output.Append(element);
        }

        output.RestartStrip();
    }*/

	for (int i = 0; i < 2; i++)
	{
        float offset = input[i].thickness / 2.0f;

        {
            GSOutput element;
            element.pos = input[i].pos + float4(offset, 0.0f, 0.0f, 0.0f);
            element.col = input[i].col;
            output.Append(element);
        }
        {
            GSOutput element;
            element.pos = input[i].pos + float4(-offset, 0.0f, 0.0f, 0.0f);
            element.col = input[i].col;
            output.Append(element);
        }
        {
            GSOutput element;
            element.pos = input[(i + 1) % 2].pos + float4(offset * sign(i - 1), 0.0f, 0.0f, 0.0f);

            element.col = input[(i + 1) % 2].col;
            output.Append(element);
        }

        output.RestartStrip();
	}
}