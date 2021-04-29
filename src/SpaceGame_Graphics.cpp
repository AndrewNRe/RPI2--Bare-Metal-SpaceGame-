bit32 Inside1DLineTest(f32 Value, f32 Start, f32 End)
{
    bit32 InsideRegion = 0; //Start as valid.
    if(Value < Start){ InsideRegion = REGION_BEHIND_START; } //Was to the left of the start. Invalid.
    else if(Value > End) { InsideRegion = REGION_BEHIND_END; } //Was to the right of end. Invalid
    return InsideRegion;
}


#define OutOfAllPlanes(Region) ( ((Region & REGION_X_OUT) && (Region & REGION_Y_OUT)) || (Region & REGION_Z_OUT))

bit32 SubdivideTriangle(memory_block* Block, triangle* Triangle, bit32 VertexID, bit32 Region)
{
    bit32 VertexDataWrote = 0;
    vertex CheckVertex = Triangle->E[VertexID];
    if(Region)
    {
        //TODO(Andrew) FINISH ALL THE VARIOUS REGION CASES FOR A SINGLE POINT! THERE ARE ONLY TWO PER PLANE! AND THREE PLANES PER VERTEX!!!
        //So all this function should do, is simply generate vertex data into the array data.
    }
    else //Vertex was completely within all possible clip planes and thus, is an "early out".
    {
        vertex* Current = PushStruct(Block, vertex, MemoryFlag_NoAlign);
        (*Current) = CheckVertex;
        VertexDataWrote++;
    }
    return VertexDataWrote;
}