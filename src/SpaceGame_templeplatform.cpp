#define RENDER_BOX_INDEX_COUNT 36
struct render_box
{
    vertex Vertex[8];
    bit16 Index[RENDER_BOX_INDEX_COUNT];
};

#define DESIRED_TEMPLE_PLATFORM_COUNT 1
#define TEMPLE_PLATFORM_MAX_TARGET_POSITION_COUNT 2 //NOTE: Must always be greater than 2!!!!
#define TEMPLE_PLATFORM_TIMER_END 1.0f
struct temple_platform_instance
{
    world_transform Target[TEMPLE_PLATFORM_MAX_TARGET_POSITION_COUNT];
    f32 CeilingHeight;
    f32 Timer;
    f32 Increment;
    bool32 Rewind;
};

struct temple_platform
{
    render_box Mesh;
    bit32 InstanceCount;
    temple_platform_instance* Instance;
};

inline temple_platform_instance GenerateTemplePlatformInstance(f32 StartTime, f32 Increment, f32 CeilingHeight, world_transform tA, world_transform tB)
{
    temple_platform_instance Result = {};
    Result.Timer = StartTime;
    Result.Increment = Increment;
    Result.CeilingHeight = CeilingHeight;
    bit32 t = 0;
    Result.Target[0] = tA; t++;
    Result.Target[1] = tB; t++;
    return Result;
}

inline void TemplePlatformIncrementTimer(f32* Timer, f32 Increment)
{
    (*Timer) += Increment;
    if((*Timer) > TEMPLE_PLATFORM_TIMER_END)
    {(*Timer) = Increment; }
}