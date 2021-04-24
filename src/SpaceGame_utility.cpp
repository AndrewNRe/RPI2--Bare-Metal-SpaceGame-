#define Kilobytes(Value) ((Value)*1024LL)
#define PI32 3.14159265359f
#define Radians(Degrees) (Degrees * (PI32/180))
#define Degrees(Radians) (Radians * (180.0f/PI32))

#define framerate 15

f32 superslowf32_aprx_sqrt(f32 number) //TODO(Andrew): maybe get a large speedup from writing this in assembly?
{
    f32 value = number;
    f32 composite = value*value;
    for(;composite > number; value/=2)
    {
        composite = value*value;
        SDK_BLINKBOARD(1);
    }
}