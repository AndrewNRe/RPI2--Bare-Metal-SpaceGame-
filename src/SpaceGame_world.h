struct world_transform
{
    vec3 Translation;
    vec3 RotationAxes;
};

world_transform InterpolateWorldTransform(world_transform A, world_transform B, vec3 RotationAxes, f32 Time)
{
    world_transform Result = {};
    Result.Translation = lerp_vec3(A.Translation, B.Translation, Time);
    Result.RotationAxes = RotationAxes; //TODO(Andrew) If you want to interpolate between rotation, you need to encode quaternion instead of euler axes for good results!!!!
    return Result;
}

inline mat4x4 RotationAxesAndTranslationToMat4x4(world_transform Transform)
{
    mat3x3 m3 = RotationMatrix(Transform.RotationAxes);
    mat4x4 Result;
    Result.d[0][0] = m3.d[0][0]; Result.d[0][1] = m3.d[0][1]; Result.d[0][2] = m3.d[0][2]; Result.d[0][3] = Transform.Translation.x;
    Result.d[1][0] = m3.d[1][0]; Result.d[1][1] = m3.d[1][1]; Result.d[1][2] = m3.d[1][2]; Result.d[1][3] = Transform.Translation.y;
    Result.d[2][0] = m3.d[2][0]; Result.d[2][1] = m3.d[2][1]; Result.d[2][2] = m3.d[2][2]; Result.d[2][3] = Transform.Translation.z;
    Result.d[3][0] = 0.0f; Result.d[3][1] = 0.0f; Result.d[3][2] = 0.0f; Result.d[3][3] = 1.0f;
    return Result;
}


inline mat4x4 InverseRotationAxesAndTranslationToMat4x4(world_transform Transform)
{
    mat3x3 m3 = InverseRotationMatrix(Transform.RotationAxes);
    mat4x4 Result = {};
    Result.d[0][0] = m3.d[0][0]; Result.d[0][1] = m3.d[0][1]; Result.d[0][2] = m3.d[0][2]; Result.d[0][3] = 0.0f;
    Result.d[1][0] = m3.d[1][0]; Result.d[1][1] = m3.d[1][1]; Result.d[1][2] = m3.d[1][2]; Result.d[1][3] = 0.0f;
    Result.d[2][0] = m3.d[2][0]; Result.d[2][1] = m3.d[2][1]; Result.d[2][2] = m3.d[2][2]; Result.d[2][3] = 0.0f;
    Result.d[3][0] = 0.0f; Result.d[3][1] = 0.0f; Result.d[3][2] = 0.0f; Result.d[3][3] = 1.0f;
    mat4x4 InverseTranslation = TranslationAxesToMat4x4(-Transform.Translation);
    Result = Result * InverseTranslation;
    return Result;
}