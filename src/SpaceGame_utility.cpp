#define Kilobytes(Value) ((Value)*1024LL)
#define PI32 3.14159265359f
#define Radians(Degrees) (Degrees * (PI32/180))
#define Degrees(Radians) (Radians * (180.0f/PI32))
#define F32_EPSILON 1.19209290e-7f
#define F32_MAX 3.402823466e+38

#define framerate 15

//#define AlignPow2(Value, ByteAlignment) ( (Value) + ((ByteAlignment) - ((Value) + ((ByteAlignment) - 1))) )
#define AlignPow2(Value, Alignment) (((Value) + ((Alignment) - 1)) & ~(((Value) - (Value)) + (Alignment) - 1)) //NOTE: I am using the Handmade Hero AlignPow2 macro that does the 
//ensures that no integral promotion occurs. What this basically means is taht if you were to use the commented out macro, the value that the math generates might be done in 32 bits
//instead of 64 bits and thus, will produce an incorrect result since you can (and on startup currently do as of 1/26/21) send a 64 bit integer, without checking for integral promotion,
//introduces a chance for the alignpow2 to have bugs that are dependant on if the compiler does a good job or not!!! So the HMH version fixes that.

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