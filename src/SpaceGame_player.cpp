struct camera
{
    vec3 OrbitPosition;
    vec2 RotatePair; //NOTE: Only rotates on the X axis, so move the YZ plane, and the Y axis, moving the XZ plane
};

struct game_player
{
    world_transform Transform;
    world_transform Dest;
};