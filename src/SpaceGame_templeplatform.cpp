#define RENDER_BOX_INDEX_COUNT 36
struct render_box
{
    vertex Vertex[8];
    bit16 Index[RENDER_BOX_INDEX_COUNT];
};

#define DESIRED_TEMPLE_PLATFORM_COUNT 1
#define TEMPLE_PLATFORM_MAX_TARGET_POSITION_COUNT 2
#define TEMPLE_PLATFORM_TIMER_LOOP_FRAME_COUNT 30
struct temple_platform_instance
{
    vec3 Target[TEMPLE_PLATFORM_MAX_TARGET_POSITION_COUNT];
    world_transform Transform;
    f32 CeilingHeight;
    s32 Timer;
    bool32 Rewind;
};

struct temple_platform
{
    render_box Mesh;
    bit32 InstanceCount; //NOTE: not much sense in making the instances a linked list tbh, so just doing this :)
    temple_platform_instance* Instance;
};

inline temple_platform_instance GenerateTemplePlatformInstance(bit32 StartTime, f32 CeilingHeight, vec3 tA, world_transform Transform)
{
    temple_platform_instance Result = {};
    Result.Timer = StartTime;
    Result.CeilingHeight = CeilingHeight;
    Result.Transform = Transform;
    bit32 t = 0;
    Result.Target[t] = tA; t++;
    return Result;
}