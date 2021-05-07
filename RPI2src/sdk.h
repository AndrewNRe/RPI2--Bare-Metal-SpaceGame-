#ifdef __cplusplus
extern "C" void SDK_BLINKBOARD(bit32 number_of_flashes);
extern "C" void SDK_CHECK_IF_INTTERUPT();
#define size_t unsigned int //NOTE: 32 bit platform!
//NOTE: C RUNTIME FUNCTIONS
extern "C" f32 sinf(f32 a);
extern "C" f32 cosf(f32 a);
extern "C" void* memset(void* dest, int c, size_t count);
extern "C" void* memcpy(void *dest, const void *src, size_t count);
#else
void SDK_BLINKBOARD(bit32 number_of_flashes);
void SDK_CHECK_IF_INTTERUPT();
#define size_t unsigned int //NOTE: 32 bit platform!
//NOTE: C RUNTIME FUNCTIONS
f32 sinf(f32 a);
f32 cosf(f32 a);
void* memset(void* dest, int c, size_t count);
#endif