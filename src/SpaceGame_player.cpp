struct camera
{
    vec3 OrbitPosition;
    vec2 RotatePair; //NOTE: Only rotates on the X axis, so move the YZ plane, and the Y axis, moving the XZ plane
};

struct game_player
{
    world_transform Transform;
    world_transform PreviousTransform;
    interpolation_data Interpolation;
    interpolation_data Power;
    vec3 XMove; //NOTE: These are just conceptually the X Y and Z axis move amounts when the player is moving. Just storing each component separately for ease of overwriting.
    vec3 YMove;
    vec3 ZMove;
    bit32 Score;
};

vec3 PlayerMove(vec3 A, vec3 B, mat3x3 R, interpolation_data* Interpolation)
{
    vec3 Result = {};
    vec3 RA = R * A;
    vec3 RB = R * B;
    Result = lerp_vec3(RA, RB, Interpolation->Amount);
    IncrementInterpolation(Interpolation);
    return Result;
}