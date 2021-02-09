struct Memory
{
    bit32 StartAddress;
    bit32 EndAddress;
    bit32 RecordPosition;
    bit32 StackPosition;
};
#define DesiredMemorySize Kilobytes(4)

#define recordpush(sizeofdata) (memory.RecordPosition); (memory.RecordPosition += sizeofdata); Assert(memory.RecordPosition < memory.StackPosition);
#define stackpush(sizeofdata) (memory.StackPosition - sizeofdata); (memory.StackPosition-=sizeofdata); Assert(memory.StackPosition > memory.RecordPosition);
#define stackpop(sizeofdata) (memory.StackPosition+=sizeofdata); Assert(memory.StackPosition <= memory.EndAddress);

Memory memory; //NOTE: Memory global
inline void setupmemory()
{
    memory.StartAddress = platform_memoryallocate(DesiredMemorySize);
    memory.EndAddress = memory.StartAddress + DesiredMemorySize;
    memory.RecordPosition = memory.StartAddress;
    memory.StackPosition = memory.EndAddress;
}