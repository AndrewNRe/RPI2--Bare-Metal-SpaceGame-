void SubdivideTriangle(bit32 Region, triangle* Triangle, memory_block* Block)
{
    triangle* Result = PushStruct(Block, triangle, MemoryFlag_NoAlign);
    
    if(Region == 1)
    {
        
    }
    else // == 2
    {
        
    }
}

bit32 Inside1DLineTest(f32 Value, f32 Start, f32 End)
{
    bit32 InsideRegion = 0; //Start as valid.
    if(Value < Start){ InsideRegion = 1; } //Was to the left of the start. Invalid.
    else if(Value > End) { InsideRegion = 2; } //Was to the right of end. Invalid
    return InsideRegion;
}