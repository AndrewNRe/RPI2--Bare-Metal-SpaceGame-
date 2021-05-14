bit32 Inside1DLineTest(f32 Value, f32 Start, f32 End)
{
    bit32 InsideRegion = 0; //Start as valid.
    if(Value < Start){ InsideRegion = REGION_BEHIND_START; } //Was to the left of the start. Invalid.
    else if(Value > End) { InsideRegion = REGION_BEHIND_END; } //Was to the right of end. Invalid
    return InsideRegion;
}

bit32 RegionCheck(vec3 Position,
                  f32 BottomClipPlane, f32 TopClipPlane,
                  f32 LeftClipPlane, f32 RightClipPlane,
                  f32 NearClipPlane, f32 FarClipPlane)
{
    bit32 Result = 0;
    bit32 Xin = Inside1DLineTest(Position.x, LeftClipPlane, RightClipPlane);
    bit32 Yin = Inside1DLineTest(Position.y, BottomClipPlane, TopClipPlane);
    bit32 Zin = 0;
    if(Position.z > NearClipPlane){ Zin = REGION_BEHIND_START; }
    else if(Position.z < FarClipPlane) { Zin = REGION_BEHIND_END; }
    //NOTE: Little endian machine! I have the bits going from least significant bit, Xin, Yin, Zin.
    //Going from most significant, it'd be Zin, Yin, Xin. So if you're using the Windows 2021 4/28/21 calculator bit viewer, you'll see the bits like this!!!
    Result = (Xin | (Yin << 2) | (Zin << 4));
}

//#define OutOfAllPlanes(Region) ( ((Region & REGION_X_OUT) && (Region & REGION_Y_OUT)) || (Region & REGION_Z_OUT))
//#define OutOfAllPlanes(Region) ( ((Region) & REGION_X_OUT) && ((Region) & REGION_Y_OUT) && ((Region) & REGION_Z_OUT) )
#define OutOfAllPlanes(Region) ( (Region & REGION_X_OUT) || (Region & REGION_Y_OUT) || (Region & REGION_Z_OUT))

bit32 WriteVertex(memory_block* Block, vertex VertexToWrite)
{
    bit32 CreatedVertexCount = 0;
    vertex* NewVertex = PushStruct(Block, vertex, MemoryFlag_NoAlign);
    (*NewVertex) = VertexToWrite;
    CreatedVertexCount++;
    return CreatedVertexCount;
}

bit32 SubdivideTriangle(memory_block* Block, vertex* Current, bit32 Region,
                        f32 BottomClipPlane, f32 TopClipPlane,
                        f32 LeftClipPlane, f32 RightClipPlane,
                        f32 NearClipPlane, f32 FarClipPlane)
{
    bit32 CreatedVertexCount = 0;
    if(Region)
    {
        if(Region & 0x1)
        {//Left
            CreatedVertexCount += WriteVertex(Block, {LeftClipPlane, Current->Position.y, Current->Position.z, Current->Color});
        }
        else if(Region & 0x2)
        {//Right
            CreatedVertexCount += WriteVertex(Block, {RightClipPlane, Current->Position.y, Current->Position.z, Current->Color});
        }
        
        if(Region & 0x4)
        {//Bottom
            CreatedVertexCount += WriteVertex(Block, {Current->Position.x, BottomClipPlane, Current->Position.z, Current->Color});
        }
        else if(Region & 0x8)
        {//Top
            CreatedVertexCount += WriteVertex(Block, {Current->Position.x, TopClipPlane, Current->Position.z, Current->Color});
        }
        
        if(Region & 0x10)
        {//Near
            CreatedVertexCount += WriteVertex(Block, {Current->Position.x, Current->Position.y, NearClipPlane - 0.1f, Current->Color});
        }
        else if(Region & 0x20)
        {//Far
            CreatedVertexCount += WriteVertex(Block, {Current->Position.x, Current->Position.y, FarClipPlane, Current->Color});
        }
    }
    else
    { CreatedVertexCount += WriteVertex(Block, (*Current)); }
    return CreatedVertexCount;
}