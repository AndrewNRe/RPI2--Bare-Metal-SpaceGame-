struct vertex
{
    vec3 Position;
    bit32 Color;
};

#define NUMBER_OF_VERTEXES_IN_TRIANGLE 3

struct triangle
{
    union
    {
        struct { vertex A, B, C; };
        vertex E[NUMBER_OF_VERTEXES_IN_TRIANGLE];
    };
};

struct scanline_triangle
{
    f32 Z; //NOTE: Rough approximation of where the triangle actually is because I want to go fast and not really care too much as of 4/9/21
    bit32 Color;
    bit32 TriangleID; //TODO(Andrew) Delete this because it's debug info at some point or compile it out and in using a debug switch!
    //vec2 PostDiv[3] //TODO(Andrew) Delete this because it's debug info at some point or compile it out and in using a debug switch!;
    bit32 BranchID; //TODO(Andrew) Delete this because it's debug info at some point or compile it out and in using a debug switch!4
    vec3 PreDiv[3]; //TODO(Andrew) Delete this because it's debug info at some point or compile it out and in using a debug switch!
    union
    {
        struct { ivec2 A, B, C; };
        bit32 E[6];
    };
};

void ScanlineTriangleTransfer(scanline_triangle* A, scanline_triangle* B)
{
    scanline_triangle Temp = (*A);
    (*A) = (*B);
    (*B) = Temp;
}

#define REGION_BEHIND_START 1
#define REGION_BEHIND_END 2
#define REGION_X_OUT 0x3
#define REGION_Y_OUT 0xC
#define REGION_Z_OUT 0x30

#define RENDER_BOX_DEFAULT_X 25.0f
#define RENDER_BOX_DEFAULT_Y 5.0f
#define RENDER_BOX_DEFAULT_Z 25.0f
#define RENDER_BOX_INDEX_COUNT 36
struct render_box
{
    vertex Vertex[8];
    bit16 Index[RENDER_BOX_INDEX_COUNT];
};