inline vec3 vec4tovec3(vec4 B)
{
    vec3 Result;
    Result.x = B.x;
    Result.y = B.y;
    Result.z = B.z;
    return Result;
}

inline vec4 vec3tovec4(vec3 B, f32 w)
{
    vec4 Result;
    Result.x = B.x;
    Result.y = B.y;
    Result.z = B.z;
    Result.w = w;
    return Result;
}

inline bool32 operator!=(vec3 A, vec3 B)
{
    bool32 result = 0;
    result += (A.x != B.x);
    result += (A.y != B.y);
    result += (A.z != B.z);
    if(result > 0){return 1;}
    return 0;
}

inline bool32 operator==(vec3 A, vec3 B)
{
    bool32 result = 0;
    result += (A.x == B.x);
    result += (A.y == B.y);
    result += (A.z == B.z);
    if(result == 3){return 1;}
    return 0;
}

inline bool32 operator ==(vec2 A, vec2 B)
{
    bool32 result = 0;
    result += (A.x == B.x);
    result += (A.y == B.y);
    if(result == 2){return 1;}
    return 0;
}

inline bool32 operator<=(vec3 A, f32 B)
{
    bool32 result = 0;
    result += (A.x <= B);
    result += (A.y <= B);
    result += (A.z <= B);
    if(result == 3){return 1;}
    return 0;
}

inline bool32 operator<=(vec2 A, f32 B)
{
    bool32 result = 0;
    result += (A.x <= B);
    result += (A.y <= B);
    if(result == 2){return 1;}
    return 0;
}

inline bool32 operator>=(vec3 A, f32 B)
{
    bool32 result = 0;
    result += (A.x >= B);
    result += (A.y >= B);
    result += (A.z >= B);
    if(result == 3){return 1;}
    return 0;
}

inline bool32 operator<(vec3 A, f32 B)
{
    bool32 result = 0;
    result += (A.x < B);
    result += (A.y < B);
    result += (A.z < B);
    if(result == 3){return 1;}
    return 0;
}

inline bool32 operator>(vec3 A, f32 B)
{
    bool32 result = 0;
    result += (A.x > B);
    result += (A.y > B);
    result += (A.z > B);
    if(result == 3){return 1;}
    return 0;
}

inline vec2 operator-(vec2 A)
{
	vec2 result;
	result.x = -A.x;
	result.y = -A.y;
	return(result);
}
inline vec3 operator-(vec3 A)
{
	vec3 result;
	result.x = -A.x;
	result.y = -A.y;
	result.z = -A.z;
	return(result);
}
inline vec4 operator-(vec4 A)
{
	vec4 result;
	result.x = -A.x;
	result.y = -A.y;
	result.z = -A.z;
	return(result);
}

inline vec2 operator+(vec2 A, vec2 B)
{
	vec2 result;
	result.x = A.x + B.x;
	result.y = A.y + B.y;
	return(result);
}
inline vec3 operator+(vec3 A, vec3 B)
{
	vec3 result;
	result.x = A.x + B.x;
	result.y = A.y + B.y;
	result.z = A.z + B.z;
	return(result);
}
inline vec4 operator+(vec4 A, vec4 B)
{
	vec4 result;
	result.x = A.x + B.x;
	result.y = A.y + B.y;
	result.z = A.z + B.z;
	result.w = A.w + B.w;
	return(result);
}

inline void operator+=(vec2 &A, vec2 B)
{
    A.x = A.x + B.x;
    A.y =  A.y + B.y;
}
inline void operator+=(vec3 &A, vec3 B)
{
    A.x = A.x + B.x;
    A.y =  A.y + B.y;
    A.z =  A.z + B.z;
}
inline void operator+=(vec4 &A, vec4 B)
{
    A.x = A.x + B.x;
    A.y =  A.y + B.y;
    A.z =  A.z + B.z;
    A.w =  A.w + B.w;
}

inline void operator-=(vec3 &A, f32 s)
{
    A.x = A.x - s;
    A.y = A.y - s;
    A.z = A.z - s;
}

inline vec2 operator-(vec2 A, vec2 B)
{
	vec2 result;
	result.x = A.x - B.x;
	result.y = A.y - B.y;
	return(result);
}
inline vec3 operator-(vec3 A, vec3 B)
{
	vec3 result;
	result.x = A.x - B.x;
	result.y = A.y - B.y;
	result.z = A.z - B.z;
	return(result);
}
inline vec4 operator-(vec4 A, vec4 B)
{
	vec4 result;
	result.x = A.x - B.x;
	result.y = A.y - B.y;
	result.z = A.z - B.z;
	result.w = A.w - B.w;
	return(result);
}

inline vec3 operator+(vec3 A, f32 S)
{
    vec3 result;
    result.x = A.x + S;
    result.y = A.y + S;
    result.z = A.z + S;
    return(result);
}
inline vec3 operator+(f32 S, vec3 A)
{
    vec3 result;
    result.x = S + A.x;
    result.y = S + A.y;
    result.z = S + A.z;
    return(result);
}
inline vec3 operator-(vec3 A, f32 S)
{
    vec3 result;
    result.x = A.x - S;
    result.y = A.y - S;
    result.z = A.z - S;
    return(result);
}
inline vec3 operator-(f32 S, vec3 A)
{
    vec3 result;
    result.x = S - A.x;
    result.y = S - A.y;
    result.z = S - A.z;
    return(result);
}


inline vec2 operator*(f32 x, vec2 A)
{
	vec2 result;
	result.x = x * A.x;
	result.y = x * A.y;
	return(result);
}
inline vec3 operator*(f32 x, vec3 A)
{
	vec3 result;
	result.x = x * A.x;
	result.y = x * A.y;
	result.z = x * A.z;
	return(result);
}
inline vec3 operator*(vec3 A, f32 x)
{
	vec3 result;
	result.x = A.x * x;
	result.y = A.y * x;
	result.z = A.z * x;
	return(result);
}
inline vec4 operator*(f32 x, vec4 A)
{
	vec4 result;
	result.x = x * A.x;
	result.y = x * A.y;
	result.z = x * A.z;
	result.w = x * A.w;
	return(result);
}

inline vec3 operator/(vec3 B, f32 A)
{
    vec3 result;
    result = (1.0f/A)*B;
    return(result);
}

inline vec3 operator/(f32 B, vec3 A)
{
    vec3 result;
    result.x = B / A.x;
    result.y = B / A.y;
    result.z = B / A.z;
    return(result);
}

inline f32 dot_vec2(vec2 A, vec2 B) { return (A.x * B.x) + (A.y * B.y); }
inline f32 dot_vec3(vec3 A, vec3 B) { return (A.x * B.x) + (A.y * B.y) + (A.z * B.z); }
inline f32 dot_vec4(vec4 A, vec4 B) { return (A.x * B.x) + (A.y * B.y) + (A.z * B.z) + (A.w * B.w); }

inline vec3 cross(vec3 A, vec3 B)
{
	vec3 result;
	result.x = (A.y*B.z - A.z*B.y);
	result.y = (A.z*B.x - A.x*B.z);
	result.z = (A.x*B.y - A.y*B.x);
	return(result);
}

inline mat3x3 identity3x3()
{
    mat3x3 result;
    result.d[0][0] = 1; result.d[0][1] = 0; result.d[0][2] = 0;
    result.d[1][0] = 0; result.d[1][1] = 1; result.d[1][2] = 0;
    result.d[2][0] = 0; result.d[2][1] = 0; result.d[2][2] = 1;
    return result;
}
inline mat3x3 rotate3x3X(f32 theta)
{
    mat3x3 result;
    result.d[0][0] = 1; result.d[0][1] = 0; result.d[0][2] = 0;
    result.d[1][0] = 0; result.d[1][1] = cosf(theta); result.d[1][2] = -sinf(theta);
    result.d[2][0] = 0; result.d[2][1] = sinf(theta); result.d[2][2] = cosf(theta);
    return result;
}
inline mat3x3 rotate3x3Y(f32 theta)
{
    mat3x3 result;
    result.d[0][0] = cosf(theta); result.d[0][1] = 0; result.d[0][2] = sinf(theta);
    result.d[1][0] = 0; result.d[1][1] = 1; result.d[1][2] = 0;
    result.d[2][0] = -sinf(theta); result.d[2][1] = 0; result.d[2][2] = cosf(theta);
    return result;
}
inline mat3x3 rotate3x3Z(f32 theta)
{
    mat3x3 result;
    result.d[0][0] = cosf(theta); result.d[0][1] = -sinf(theta); result.d[0][2] = 0;
    result.d[1][0] = sinf(theta); result.d[1][1] = cosf(theta); result.d[1][2] = 0;
    result.d[2][0] = 0; result.d[2][1] = 0; result.d[2][2] = 1;
    return result;
}

inline mat3x3 rotate3x3YWORLD(f32 theta)
{
    mat3x3 result;
    result.d[0][0] = cosf(theta); result.d[0][1] = 0; result.d[0][2] = -sinf(theta);
    result.d[1][0] = 0; result.d[1][1] = 1; result.d[1][2] = 0;
    result.d[2][0] = sinf(theta); result.d[2][1] = 0; result.d[2][2] = cosf(theta);
    return result;
}

inline mat3x3 transpose(mat3x3 A)
{
    mat3x3 result;
    result.d[0][0] = A.d[0][0]; result.d[0][1] = A.d[1][0]; result.d[0][2] = A.d[2][0];
    result.d[1][0] = A.d[0][1]; result.d[1][1] = A.d[1][1]; result.d[1][2] = A.d[2][1];
    result.d[2][0] = A.d[0][2]; result.d[2][1] = A.d[1][2]; result.d[2][2] = A.d[2][2];
    return result;
}

inline mat3x3 operator*(mat3x3 A, mat3x3 B)
{
	mat3x3 result;
	result.d[0][0] = (A.d[0][0] * B.d[0][0]) + (A.d[0][1] * B.d[1][0]) + (A.d[0][2] * B.d[2][0]); result.d[0][1] = (A.d[0][0] * B.d[0][1]) + (A.d[0][1] * B.d[1][1]) + (A.d[0][2] * B.d[2][1]); result.d[0][2] = (A.d[0][0] * B.d[0][2]) + (A.d[0][1] * B.d[1][2]) + (A.d[0][2] * B.d[2][2]);
	result.d[1][0] = (A.d[1][0] * B.d[0][0]) + (A.d[1][1] * B.d[1][0]) + (A.d[1][2] * B.d[2][0]); result.d[1][1] = (A.d[1][0] * B.d[0][1]) + (A.d[1][1] * B.d[1][1]) + (A.d[1][2] * B.d[2][1]); result.d[1][2] = (A.d[1][0] * B.d[0][2]) + (A.d[1][1] * B.d[1][2]) + (A.d[1][2] * B.d[2][2]);
	result.d[2][0] = (A.d[2][0] * B.d[0][0]) + (A.d[2][1] * B.d[1][0]) + (A.d[2][2] * B.d[2][0]); result.d[2][1] = (A.d[2][0] * B.d[0][1]) + (A.d[2][1] * B.d[1][1]) + (A.d[2][2] * B.d[2][1]); result.d[2][2] = (A.d[2][0] * B.d[0][2]) + (A.d[2][1] * B.d[1][2]) + (A.d[2][2] * B.d[2][2]);
	return(result);
}

inline mat3x4 operator*(mat3x4 A, mat3x4 B)
{
	mat3x4 result;
	//Note on each of these rows. The mul by zero is implicit here. As i'd waste a mul and add zero otherwise if 4x4. So why not save some instructions!
	result.d[0][0] = (A.d[0][0] * B.d[0][0]) + (A.d[0][1] * B.d[1][0]) + (A.d[0][2] * B.d[2][0]); result.d[0][1] = (A.d[0][0] * B.d[0][1]) + (A.d[0][1] * B.d[1][1]) + (A.d[0][2] * B.d[2][1]); result.d[0][2] = (A.d[0][0] * B.d[0][2]) + (A.d[0][1] * B.d[1][2]) + (A.d[0][2] * B.d[2][2]);
	result.d[1][0] = (A.d[1][0] * B.d[0][0]) + (A.d[1][1] * B.d[1][0]) + (A.d[1][2] * B.d[2][0]); result.d[1][1] = (A.d[1][0] * B.d[0][1]) + (A.d[1][1] * B.d[1][1]) + (A.d[1][2] * B.d[2][1]); result.d[1][2] = (A.d[1][0] * B.d[0][2]) + (A.d[1][1] * B.d[1][2]) + (A.d[1][2] * B.d[2][2]);
	result.d[2][0] = (A.d[2][0] * B.d[0][0]) + (A.d[2][1] * B.d[1][0]) + (A.d[2][2] * B.d[2][0]); result.d[2][1] = (A.d[2][0] * B.d[0][1]) + (A.d[2][1] * B.d[1][1]) + (A.d[2][2] * B.d[2][1]); result.d[2][2] = (A.d[2][0] * B.d[0][2]) + (A.d[2][1] * B.d[1][2]) + (A.d[2][2] * B.d[2][2]);
	//Note on the final add here. Since i'm implying that the final value in the "w" slot is a 1. I just add the B and don't have an extra mul!
	result.d[3][0] = (A.d[3][0] * B.d[0][0]) + (A.d[3][1] * B.d[1][0]) + (A.d[3][2] * B.d[2][0]) + (B.d[3][0]); result.d[3][1] = (A.d[3][0] * B.d[0][1]) + (A.d[3][1] * B.d[1][1]) + (A.d[3][2] * B.d[2][1]) + (B.d[3][1]); result.d[3][2] = (A.d[3][0] * B.d[0][2]) + (A.d[3][1] * B.d[1][2]) + (A.d[3][2] * B.d[2][2]) + (B.d[3][2]);
	return(result);
}

inline mat4x4 operator*(mat4x4 A, mat4x4 B)
{
	mat4x4 result;
	result.d[0][0] = (A.d[0][0] * B.d[0][0]) + (A.d[0][1] * B.d[1][0]) + (A.d[0][2] * B.d[2][0]) + (A.d[0][3] * B.d[3][0]); result.d[0][1] = (A.d[0][0] * B.d[0][1]) + (A.d[0][1] * B.d[1][1]) + (A.d[0][2] * B.d[2][1]) + (A.d[0][3] * B.d[3][1]); result.d[0][2] = (A.d[0][0] * B.d[0][2]) + (A.d[0][1] * B.d[1][2]) + (A.d[0][2] * B.d[2][2]) + (A.d[0][3] * B.d[3][2]); result.d[0][3] = (A.d[0][0] * B.d[0][3]) + (A.d[0][1] * B.d[1][3]) + (A.d[0][2] * B.d[2][3]) + (A.d[0][3] * B.d[3][3]);
	result.d[1][0] = (A.d[1][0] * B.d[0][0]) + (A.d[1][1] * B.d[1][0]) + (A.d[1][2] * B.d[2][0]) + (A.d[1][3] * B.d[3][0]); result.d[1][1] = (A.d[1][0] * B.d[0][1]) + (A.d[1][1] * B.d[1][1]) + (A.d[1][2] * B.d[2][1]) + (A.d[1][3] * B.d[3][1]); result.d[1][2] = (A.d[1][0] * B.d[0][2]) + (A.d[1][1] * B.d[1][2]) + (A.d[1][2] * B.d[2][2]) + (A.d[1][3] * B.d[3][2]); result.d[1][3] = (A.d[1][0] * B.d[0][3]) + (A.d[1][1] * B.d[1][3]) + (A.d[1][2] * B.d[2][3]) + (A.d[1][3] * B.d[3][3]);
	result.d[2][0] = (A.d[2][0] * B.d[0][0]) + (A.d[2][1] * B.d[1][0]) + (A.d[2][2] * B.d[2][0]) + (A.d[2][3] * B.d[3][0]); result.d[2][1] = (A.d[2][0] * B.d[0][1]) + (A.d[2][1] * B.d[1][1]) + (A.d[2][2] * B.d[2][1]) + (A.d[2][3] * B.d[3][1]); result.d[2][2] = (A.d[2][0] * B.d[0][2]) + (A.d[2][1] * B.d[1][2]) + (A.d[2][2] * B.d[2][2]) + (A.d[2][3] * B.d[3][2]); result.d[2][3] = (A.d[2][0] * B.d[0][3]) + (A.d[2][1] * B.d[1][3]) + (A.d[2][2] * B.d[2][3]) + (A.d[2][3] * B.d[3][3]);
	result.d[3][0] = (A.d[3][0] * B.d[0][0]) + (A.d[3][1] * B.d[1][0]) + (A.d[3][2] * B.d[2][0]) + (A.d[3][3] * B.d[3][0]); result.d[3][1] = (A.d[3][0] * B.d[0][1]) + (A.d[3][1] * B.d[1][1]) + (A.d[3][2] * B.d[2][1]) + (A.d[3][3] * B.d[3][1]); result.d[3][2] = (A.d[3][0] * B.d[0][2]) + (A.d[3][1] * B.d[1][2]) + (A.d[3][2] * B.d[2][2]) + (A.d[3][3] * B.d[3][2]); result.d[3][3] = (A.d[3][0] * B.d[0][3]) + (A.d[3][1] * B.d[1][3]) + (A.d[3][2] * B.d[2][3]) + (A.d[3][3] * B.d[3][3]);
	return(result);
}

inline mat4x4 operator*(mat3x3 A, mat4x4 B)
{ //NOTE: assumes that the A matrix would have all 1 values in the w components.
    mat4x4 result;
    result.d[0][0] = (A.d[0][0] * B.d[0][0]) + (A.d[0][1] * B.d[1][0]) + (A.d[0][2] * B.d[2][0]); result.d[0][1] = (A.d[0][0] * B.d[0][1]) + (A.d[0][1] * B.d[1][1]) + (A.d[0][2] * B.d[2][1]); result.d[0][2] = (A.d[0][0] * B.d[0][2]) + (A.d[0][1] * B.d[1][2]) + (A.d[0][2] * B.d[2][2]); result.d[0][3] = (A.d[0][0] * B.d[0][3]) + (A.d[0][1] * B.d[1][3]) + (A.d[0][2] * B.d[2][3]); 
    result.d[1][0] = (A.d[1][0] * B.d[0][0]) + (A.d[1][1] * B.d[1][0]) + (A.d[1][2] * B.d[2][0]); result.d[1][1] = (A.d[1][0] * B.d[0][1]) + (A.d[1][1] * B.d[1][1]) + (A.d[1][2] * B.d[2][1]); result.d[1][2] = (A.d[1][0] * B.d[0][2]) + (A.d[1][1] * B.d[1][2]) + (A.d[1][2] * B.d[2][2]); result.d[1][3] = (A.d[1][0] * B.d[0][3]) + (A.d[1][1] * B.d[1][3]) + (A.d[1][2] * B.d[2][3]); 
    result.d[2][0] = (A.d[2][0] * B.d[0][0]) + (A.d[2][1] * B.d[1][0]) + (A.d[2][2] * B.d[2][0]); result.d[2][1] = (A.d[2][0] * B.d[0][1]) + (A.d[2][1] * B.d[1][1]) + (A.d[2][2] * B.d[2][1]); result.d[2][2] = (A.d[2][0] * B.d[0][2]) + (A.d[2][1] * B.d[1][2]) + (A.d[2][2] * B.d[2][2]); result.d[2][3] = (A.d[2][0] * B.d[0][3]) + (A.d[2][1] * B.d[1][3]) + (A.d[2][2] * B.d[2][3]); 
    result.d[3][0] = B.d[3][0]; result.d[3][1] = B.d[3][1]; result.d[3][2] = B.d[3][2]; result.d[3][3] = B.d[3][3]; 
    return (result);
}


inline vec2 operator*(mat2x2 M, vec2 V)
{
    vec2 result;
    result.x = (M.d[0][0] * V.x) + (M.d[0][1] * V.y);
    result.y = (M.d[1][0] * V.x) + (M.d[1][1] * V.y);
    return(result);
}

inline vec3 operator*(mat3x3 M, vec3 V)
{
    vec3 result;
    result.x = (M.d[0][0] * V.x) + (M.d[0][1] * V.y) + (M.d[0][2] * V.z);
    result.y = (M.d[1][0] * V.x) + (M.d[1][1] * V.y) + (M.d[1][2] * V.z);
    result.z = (M.d[2][0] * V.x) + (M.d[2][1] * V.y) + (M.d[2][2] * V.z);
    return(result);
}

inline vec3 operator*(mat3x4 M, vec3 V) //NOTE: this ASSUMES that you are translating.  
{
    vec3 result;
    result.x = (M.d[0][0] * V.x) + (M.d[0][1] * V.y) + (M.d[0][2] * V.z);
    result.y = (M.d[1][0] * V.x) + (M.d[1][1] * V.y) + (M.d[1][2] * V.z);
    result.z = (M.d[2][0] * V.x) + (M.d[2][1] * V.y) + (M.d[2][2] * V.z);
    //THE TRANSLATION IS AN IMPLIED * 1! OF A 4 COMPONENT VECTOR! (x, y, z, 1)
    vec3 translate; translate.x = M.d[3][0];  translate.y = M.d[3][1]; translate.z = M.d[3][2];
    result = result + translate;
    return(result);
}

inline vec4 operator*(mat4x4 M, vec4 V)
{
    vec4 Result;
    Result.x = (M.d[0][0] * V.x) + (M.d[0][1] * V.y) + (M.d[0][2] * V.z) + (M.d[0][3] * V.w);
    Result.y = (M.d[1][0] * V.x) + (M.d[1][1] * V.y) + (M.d[1][2] * V.z) + (M.d[1][3] * V.w);
    Result.z = (M.d[2][0] * V.x) + (M.d[2][1] * V.y) + (M.d[2][2] * V.z) + (M.d[2][3] * V.w);
    Result.w = (M.d[3][0] * V.x) + (M.d[3][1] * V.y) + (M.d[3][2] * V.z) + (M.d[3][3] * V.w);
    return(Result);
}

inline void transpose_mat3(mat3x3* A)
{
    mat3x3 temp = (*A);
    A->d[0][1] = temp.d[1][0];  A->d[0][2] = temp.d[2][0];
    A->d[1][0] = temp.d[0][1]; 				   A->d[1][2] = temp.d[2][1];
    A->d[2][0] = temp.d[0][2]; A->d[2][1] = temp.d[1][2];
}

inline void transpose_mat4(mat4x4* A)
{
    mat4x4 temp = (*A);
    A->d[0][1] = temp.d[1][0];  A->d[0][2] = temp.d[2][0];	A->d[0][3] = temp.d[3][0];
    A->d[1][0] = temp.d[0][1]; 				   A->d[1][2] = temp.d[2][1]; A->d[1][3] = temp.d[3][1];
    A->d[2][0] = temp.d[0][2]; A->d[2][1] = temp.d[1][2];					A->d[2][3] = temp.d[3][2];
    A->d[3][0] = temp.d[0][3]; A->d[3][1] = temp.d[1][3];  A->d[3][2] = temp.d[2][3];
}

inline vec3 lerp_vec3(vec3 a, vec3 b, f32 progression)
{
    vec3 result;
    result.x = a.x + (b.x - a.x) * progression;
    result.y = a.y + (b.y - a.y) * progression;
    result.z = a.z + (b.z - a.z) * progression;
    return result;
} 

inline vec3 normalize_vec3(vec3 A)
{
    vec3 result;
    f32 lengthsqrt = superslowf32_aprx_sqrt(dot_vec3(A, A));
    result = A * (1.0f/lengthsqrt);
    return(result);
}

inline f32 clamp(f32 min, f32 value, f32 max)
{
    f32 result = value;
    if      (result < min) { result = min - .000001f; }
    else if(result > max) { result = max - .000001f ; }
    return result;
}

inline vec2 clamp_vec2(vec2 Min, vec2 Value, vec2 Max)
{
    vec2 Result;
    Result.x = clamp(Min.x, Value.x, Max.x);
    Result.y = clamp(Min.y, Value.y, Max.y);
    return Result;
}

inline mat3x3 RotationMatrix(vec3 RotationAxes)
{
    mat3x3 Result;
    Result = rotate3x3Z(RotationAxes.z) * rotate3x3Y(RotationAxes.y) * rotate3x3X(RotationAxes.x); //NOTE: Right handed coordinate system. +X right, +Y up, +Z out.
    return Result;
}

inline mat4x4 RotationAxesAndTranslationToMat4x4(vec3 RotationAxes, vec3 Translation)
{
    mat3x3 m3 = RotationMatrix(RotationAxes);
    mat4x4 Result;
    Result.d[0][0] = m3.d[0][0]; Result.d[0][1] = m3.d[0][1]; Result.d[0][2] = m3.d[0][2]; Result.d[0][3] = Translation.x;
    Result.d[1][0] = m3.d[1][0]; Result.d[1][1] = m3.d[1][1]; Result.d[1][2] = m3.d[1][2]; Result.d[1][3] = Translation.y;
    Result.d[2][0] = m3.d[2][0]; Result.d[2][1] = m3.d[2][1]; Result.d[2][2] = m3.d[2][2]; Result.d[2][3] = Translation.z;
    Result.d[3][0] = 0.0f; Result.d[3][1] = 0.0f; Result.d[3][2] = 0.0f; Result.d[3][3] = 1.0f;
    return Result;
}