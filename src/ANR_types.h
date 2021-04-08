//Float
typedef float f32;
typedef double f64;

//Signed types
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

//Unsigned types
typedef unsigned char bit8;
typedef unsigned short bit16;
typedef unsigned int bit32;
typedef unsigned long long bit64;

//Boolean type(s)
typedef unsigned int bool32;
typedef unsigned long long bool64;

//Keyword
#define global_variable static
#define internal static

//Vector stuff
union vec2 
{
    struct { f32 x, y; };
    struct { f32 u, v; };
    f32 E[2];
};

union vec3
{
    struct { f32 x, y, z; };
    struct { f32 u, v, w; };
    f32 E[3];
};

union vec4
{
    struct{ f32 x, y, z, w; };
    f32 E[4];
};

union ivec2
{
    struct { bit32 x, y; };
    bit32 E[2];
};

//Matricies
struct mat2x2 { f32 d[2][2]; };
struct mat3x3 { f32 d[3][3]; };
struct mat3x4 { f32 d[4][3]; };
struct mat4x4 { f32 d[4][4]; };