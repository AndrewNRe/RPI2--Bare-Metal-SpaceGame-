struct vertex
{
    vec3 Position;
    bit32 Color;
};

struct render_box
{
    vertex Vertex[8];
    bit16 Index[36];
};

#define DESIRED_TEMPLE_PLATFORM_COUNT 2
#define TEMPLE_PLATFORM_MAX_TARGET_POSITION_COUNT 2
#define TEMPLE_PLATFORM_TIMER_LOOP_FRAME_COUNT 30
struct temple_platform_instance
{
    vec3 Target[TEMPLE_PLATFORM_MAX_TARGET_POSITION_COUNT];
    f32 CeilingHeight;
    s32 Timer;
    bool32 Rewind;
};

struct temple_platform
{
    render_box Mesh;
    temple_platform_instance* Instance;
};

inline temple_platform_instance GenerateTemplePlatformInstance(bit32 StartTime, f32 CeilingHeight, vec3 tA, vec3 tB)
{
    temple_platform_instance Result = {};
    Result.Timer = StartTime;
    Result.CeilingHeight = CeilingHeight;
    bit32 t = 0;
    Result.Target[t] = tA; t++;
    Result.Target[t] = tB; t++;
    return Result;
}