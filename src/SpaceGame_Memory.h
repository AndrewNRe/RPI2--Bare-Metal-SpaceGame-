#define GLOBAL_MEMORY_ALIGNMENT_BYTES 4
#define DESIRED_GAME_MEMORY_SIZE AlignPow2(Gigabytes(8), GLOBAL_MEMORY_ALIGNMENT_BYTES)

enum memory_flags
{
    MemoryFlag_NoAlign = 0x0,
    MemoryFlag_Pow2Align = 0x1,
    MemoryFlag_ClearToZero = 0x2,
};

struct memory_block
{
    bit32 Size;
    bit8* Base;
    bit32 Used;
    bit32 Flags;
};

#define PushStruct(Block, type, ...) (type*)PushSize((Block), sizeof(type), ## __VA_ARGS__)
#define PushArray(Block, Count, type, ...) (type*)PushSize((Block), (Count)*sizeof(type), ## __VA_ARGS__)
#define PushNoData(Block, type) (type*)PushSize((Block), 0, MemoryFlag_NoAlign)
inline void* PushSize(memory_block* Block, bit32 DesiredAllocation, bit32 Flags = MemoryFlag_Pow2Align)
{
    void* Result = NULL;
    bit32 ActualAllocation = 0;
    {//Get the new address
        Result = (Block->Base + Block->Used);
        if(Flags & MemoryFlag_Pow2Align)
        { ActualAllocation = AlignPow2(DesiredAllocation, GLOBAL_MEMORY_ALIGNMENT_BYTES); }
        else
        { ActualAllocation = DesiredAllocation; }
        Block->Used += ActualAllocation;
        Assert(Block->Used < Block->Size);
    }//End of getting the new address
    if(Flags & MemoryFlag_ClearToZero)
    {
        bit8* ClearToZeroPointer = (bit8*)Result;
        memset(ClearToZeroPointer, 0x00, ActualAllocation);
    }
    Block->Flags = Flags;
    return Result;
}

#define PushNewBlock(Block, DesiredBlockSize, ...) PushNewBlock_((Block), (DesiredBlockSize), ## __VA_ARGS__) 
inline memory_block PushNewBlock_(memory_block* Block, bit32 DesiredBlockSize, bit32 Flags = MemoryFlag_Pow2Align)
{
    memory_block NewBlock = {};
    NewBlock.Base = (bit8*)PushSize(Block, DesiredBlockSize, Flags);
    NewBlock.Size = DesiredBlockSize;
    return NewBlock;
}

#define DeleteBlock(Block, BlockToDelete) DeleteBlock_((Block), (BlockToDelete))
inline void DeleteBlock_(memory_block* Block, memory_block* BlockToDelete)
{
    Block->Used -= BlockToDelete->Size;
    Assert((Block->Base + Block->Used) == BlockToDelete->Base);
}