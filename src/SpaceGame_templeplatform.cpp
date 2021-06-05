#define DESIRED_TEMPLE_PLATFORM_COUNT 1
#define TEMPLE_PLATFORM_MAX_TARGET_POSITION_COUNT 2 //NOTE: Must always be greater than 2!!!!
#define TEMPLE_PLATFORM_TIMER_END 1.0f
struct temple_platform_instance
{
    vec3 RotationAxes;
    vec3 Target[TEMPLE_PLATFORM_MAX_TARGET_POSITION_COUNT];
    oriented_bounding_box OBB[3];
    f32 Timer;
    f32 Increment;
    bool32 Rewind;
};

struct temple_platform
{
    bit32 InstanceCount;
    temple_platform_instance* Instance;
};


inline temple_platform_instance GenerateTemplePlatformInstance(f32 StartTime, f32 Increment, vec3 RotationAxes, vec3 tA, vec3 tB)
{
    temple_platform_instance Result = {};
    Result.RotationAxes = RotationAxes;
    Result.Timer = StartTime;
    Result.Increment = Increment;
    bit32 t = 0;
    Result.Target[0] = tA; t++;
    Result.Target[1] = tB; t++;
    //Collision OBB setup
    f32 Height[3];
    Height[0] = 0;
    Height[1] = RENDER_BOX_DEFAULT_Y*4.0f;
    Height[2] = RENDER_BOX_DEFAULT_Y*2.0f;
    
    mat3x3 Rotation = RotationMatrix(RotationAxes);
    for(bit32 o = 0;
        o < 3;
        o++)
    {
        Result.OBB[o].AxisLength.x = RENDER_BOX_DEFAULT_X; Result.OBB[o].AxisLength.y = RENDER_BOX_DEFAULT_Y; Result.OBB[o].AxisLength.z = RENDER_BOX_DEFAULT_Z;
        Result.OBB[o].Center.x = 0.0f; Result.OBB[o].Center.y = Height[o]; Result.OBB[o].Center.z = 0.0f;
        Result.OBB[o].Center = Rotation * Result.OBB[o].Center;
        Result.OBB[o].AxisDirectionLocal[0].x = Rotation.d[0][0]; Result.OBB[o].AxisDirectionLocal[0].y = Rotation.d[1][0]; Result.OBB[o].AxisDirectionLocal[0].z = Rotation.d[2][0];
        Result.OBB[o].AxisDirectionLocal[1].x = Rotation.d[0][1]; Result.OBB[o].AxisDirectionLocal[1].y = Rotation.d[1][1]; Result.OBB[o].AxisDirectionLocal[1].z = Rotation.d[2][1];
        Result.OBB[o].AxisDirectionLocal[2].x = Rotation.d[0][2]; Result.OBB[o].AxisDirectionLocal[2].y = Rotation.d[1][2]; Result.OBB[o].AxisDirectionLocal[2].z = Rotation.d[2][2];
    }
    return Result;
}

inline void TemplePlatformIncrementTimer(f32* Timer, f32 Increment)
{
    (*Timer) += Increment;
    if((*Timer) > TEMPLE_PLATFORM_TIMER_END)
    {(*Timer) = Increment; }
}