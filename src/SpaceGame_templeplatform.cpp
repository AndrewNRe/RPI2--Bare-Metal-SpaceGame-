#define RENDER_BOX_DEFAULT_X 25.0f
#define RENDER_BOX_DEFAULT_Y 5.0f
#define RENDER_BOX_DEFAULT_Z 25.0f
#define RENDER_BOX_INDEX_COUNT 36
struct render_box
{
    vertex Vertex[8];
    bit16 Index[RENDER_BOX_INDEX_COUNT];
};

#define DESIRED_TEMPLE_PLATFORM_COUNT 2
#define TEMPLE_PLATFORM_MAX_TARGET_POSITION_COUNT 2 //NOTE: Must always be greater than 2!!!!
#define TEMPLE_PLATFORM_TIMER_END 1.0f
struct temple_platform_instance
{
    vec3 RotationAxes;
    world_transform Target[TEMPLE_PLATFORM_MAX_TARGET_POSITION_COUNT];
    vec3 ModelSpaceCollisionNormal[6]; //NOTE: These are currently hardcoded because of the way that the collision system works.
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

inline temple_platform_instance GenerateTemplePlatformInstance(f32 StartTime, f32 Increment, f32 CeilingHeight, vec3 RotationAxes, world_transform tA, world_transform tB)
{
    temple_platform_instance Result = {};
    Result.RotationAxes = RotationAxes;
    Result.Timer = StartTime;
    Result.Increment = Increment;
    Result.CeilingHeight = CeilingHeight;
    bit32 t = 0;
    Result.Target[0] = tA; t++;
    Result.Target[1] = tB; t++;
    Result.ModelSpaceCollisionNormal[0] = {0.0f, 1.0f, 0.0f}; //Bottom
    Result.ModelSpaceCollisionNormal[1] = {-1.0f, 0.0f, 0.0f}; //Left
    Result.ModelSpaceCollisionNormal[2] = {0.0f, 0.0f, 1.0f}; //Back
    Result.ModelSpaceCollisionNormal[3] = {1.0f, 0.0f, 0.0f}; //Right
    Result.ModelSpaceCollisionNormal[4] = {0.0f, 0.0f, -1.0f}; //Front
    Result.ModelSpaceCollisionNormal[5] = {0.0f, -1.0f, 0.0f}; //Top
    return Result;
}

inline void TemplePlatformIncrementTimer(f32* Timer, f32 Increment)
{
    (*Timer) += Increment;
    if((*Timer) > TEMPLE_PLATFORM_TIMER_END)
    {(*Timer) = Increment; }
}