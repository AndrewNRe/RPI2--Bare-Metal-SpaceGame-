bit32 Inside1DLineTest(f32 Value, f32 Start, f32 End)
{
    bit32 InsideRegion = 0; //Start as valid.
    if(Value < Start){ InsideRegion = REGION_BEHIND_START; } //Was to the left of the start. Invalid.
    else if(Value > End) { InsideRegion = REGION_BEHIND_END; } //Was to the right of end. Invalid
    return InsideRegion;
}


#define OutOfAllPlanes(Region) ( ((Region & REGION_X_OUT) && (Region & REGION_Y_OUT)) || (Region & REGION_Z_OUT))

bit32 SubdivideTriangle(memory_block* Block, triangle* Triangle, bit32 VertexID, bit32 Region,
                        vertex* VertexArray, bit32 Base,
                        f32 BottomClipPlane, f32 TopClipPlane,
                        f32 LeftClipPlane, f32 RightClipPlane,
                        f32 NearClipPlane, f32 FarClipPlane)
{
    bit32 VertexDataWrote = 0;
    vertex CheckVertex = Triangle->E[VertexID];
    if(Region)
    {
        vec3 Result = CheckVertex.Position;
        bit32 Color = CheckVertex.Color;
        //First test to see if you're out of 2 planes at once.
        if(Region & 0x3 && Region & 0x12)
        {//XY double out
            Result.x = (Region & 0x1) ? LeftClipPlane : RightClipPlane;
            Result.y = (Region & 0x4) ? BottomClipPlane : TopClipPlane;
            if(Region & 0x30)
            {
                Result.z = (Region & 0x10) ? NearClipPlane : FarClipPlane;
            }
        }
        else if(Region & 0x12 && Region & 0x30)
        {//YZ double out
            Result.y = (Region & 0x4) ? BottomClipPlane : TopClipPlane;
            Result.z = (Region & 0x10) ? NearClipPlane : FarClipPlane;
        }
        else if(Region & 0x30 && Region & 0x3)
        {//ZX double out
            Result.z = (Region & 0x10) ? NearClipPlane : FarClipPlane;
            Result.x = (Region & 0x1) ? LeftClipPlane : RightClipPlane;
        }
        else
        {//Was valid except for one set of planes relating to a given axis. So you just want to figure out where along that line the vector would go if intersecting the line the two planes form!
            vec3 LinePointA = {0.0f, 0.0f, 0.0f}; vec3 LinePointB = {0.0f, 0.0f, 0.0f};
            if(Region & 0x3)
            {//X
                LinePointA.x = LeftClipPlane;
                LinePointB.x = RightClipPlane;
            }
            else if(Region & 0x12)
            {//Y
                LinePointA.y = BottomClipPlane;
                LinePointB.y = TopClipPlane;
            }
            else// if(Region & 0x30)
            {//Z
                LinePointA.z = NearClipPlane;
                LinePointB.z = FarClipPlane;
            }
            bit32 BInd = (VertexID == 2) ? 0 : VertexID+1;
            bit32 CInd = (BInd == 2) ? 0 : BInd+1;
            vec3 B = Triangle->E[BInd].Position;
            vec3 C = Triangle->E[CInd].Position;
            vec3 First = ClosestPointBetweenTwoLines(LinePointA, LinePointB, Result, B);
            Result = ClosestPointBetweenTwoLines(LinePointA, LinePointB, Result, C);
            if(First != INVALID_VECTOR_3)
            {
                if(Result != INVALID_VECTOR_3)
                {//2 valid vertexes, so definitely go ahead and write the first.
                    vertex* Current = PushStruct(Block, vertex, MemoryFlag_NoAlign);
                    (*Current).Position = First;
                    (*Current).Color = Color;
                    VertexDataWrote++;
                }
                else
                {
                    Result = First; //Want to just write the first.
                }
            }
        }
        VertexDataWrote++;
        CheckVertex.Position = Result;
        CheckVertex.Color = Color;
    }
    //else //Vertex was completely within all possible clip planes and thus, is an "early out".
    {//All cases will generate at least one vertex!!!
        vertex* Current = PushStruct(Block, vertex, MemoryFlag_NoAlign);
        (*Current) = CheckVertex;
        VertexDataWrote++;
    }
    return VertexDataWrote;
}