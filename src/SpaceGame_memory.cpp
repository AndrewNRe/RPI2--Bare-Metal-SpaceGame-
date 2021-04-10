struct game_memory
{
    bit32 StartAddress;
    bit32 EndAddress;
    bit32 RecordPosition;
    bit32 StackPosition;
};
global_variable game_memory Memory;
#define DesiredMemorySize Kilobytes(4)

#define recordpush(sizeofdata) (Memory.RecordPosition); (Memory.RecordPosition += sizeofdata); Assert(Memory.RecordPosition < Memory.StackPosition)
#define StackPush(sizeofdata) (Memory.StackPosition - sizeofdata); (Memory.StackPosition-=sizeofdata); Assert(Memory.StackPosition > Memory.RecordPosition)
#define StackPop(sizeofdata) (Memory.StackPosition+=sizeofdata); Assert(Memory.StackPosition <= Memory.EndAddress)

#define RecordPushArray(type, Count) ((type*)Memory.RecordPosition); (Memory.RecordPosition += sizeof(type)*(Count)); Assert(Memory.RecordPosition < Memory.StackPosition)
#define StackPushArray(type, Count) ((type*)Memory.StackPosition - sizeof(type)*(Count)); (Memory.StackPosition-=sizeof(type)*(Count)); Assert(Memory.StackPosition > Memory.RecordPosition)
#define StackPopArray(type, Count) (Memory.StackPosition+=sizeof(type)*(Count)); Assert(Memory.StackPosition <= Memory.EndAddress)