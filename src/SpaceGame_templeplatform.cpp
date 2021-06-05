#define RENDER_BOX_DEFAULT_X 25.0f
#define RENDER_BOX_DEFAULT_Y 5.0f
#define RENDER_BOX_DEFAULT_Z 25.0f
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
    vec3 RotationAxes;
    world_transform Target[TEMPLE_PLATFORM_MAX_TARGET_POSITION_COUNT];
    oriented_bounding_box OBB[3];
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


inline temple_platform_instance GenerateTemplePlatformInstance(f32 StartTime, f32 Increment, vec3 RotationAxes, world_transform tA, world_transform tB)
{
    temple_platform_instance Result = {};
    Result.RotationAxes = RotationAxes;
    Result.Timer = StartTime;
    Result.Increment = Increment;
    bit32 t = 0;
    Result.Target[0] = tA; t++;
    Result.Target[1] = tB; t++;
    //Collision OBB setup
    f32 Height = 0;
    mat3x3 Rotation = identity3x3();
    for(bit32 o = 0;
        o < 3;
        o++,
        Height += RENDER_BOX_DEFAULT_Y*2.0f)
    {
        Result.OBB[o].AxisLength.x = RENDER_BOX_DEFAULT_X; Result.OBB[o].AxisLength.y = RENDER_BOX_DEFAULT_Y; Result.OBB[o].AxisLength.z = RENDER_BOX_DEFAULT_Z;
        Result.OBB[o].Center.x = 0.0f; Result.OBB[o].Center.y = Height; Result.OBB[o].Center.z = 0.0f;
        Result.OBB[o].AxisDirectionLocal[0].x = 1.0f; Result.OBB[o].AxisDirectionLocal[0].y = 0.0f; Result.OBB[o].AxisDirectionLocal[0].z = 0.0f;
        Result.OBB[o].AxisDirectionLocal[1].x = 0.0f; Result.OBB[o].AxisDirectionLocal[1].y = 1.0f; Result.OBB[o].AxisDirectionLocal[1].z = 0.0f;
        Result.OBB[o].AxisDirectionLocal[2].x = 0.0f; Result.OBB[o].AxisDirectionLocal[2].y = 0.0f; Result.OBB[o].AxisDirectionLocal[2].z = 1.0f;
    }
    return Result;
}

inline void TemplePlatformIncrementTimer(f32* Timer, f32 Increment)
{
    (*Timer) += Increment;
    if((*Timer) > TEMPLE_PLATFORM_TIMER_END)
    {(*Timer) = Increment; }
}