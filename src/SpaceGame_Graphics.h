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

//TODO(Andrew) IS THE BELOW USED? IF NOT DELETE!
struct projected_triangle
{
    triangle Triangle;
    bit32 Color;
};

struct scanline_triangle
{
    f32 Z; //NOTE: Rough approximation of where the triangle actually is because I want to go fast and not really care too much as of 4/9/21
    bit32 Color;
    bit32 TriangleID;
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