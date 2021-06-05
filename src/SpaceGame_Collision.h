struct oriented_bounding_box
{
    vec3 Center;
    vec3 AxisDirectionLocal[3];
    vec3 AxisLength;
};

vec3 PointIsInsideOBB(vec3 Point, oriented_bounding_box OBB, bool32* IsIn)
{
    vec3 Result = OBB.Center; //Start at center of box and make steps from there.
    vec3 PC = Point - OBB.Center;
    (*IsIn) = true;
    for(bit32 a = 0;
        a < 3;
        a++)
    {
        vec3 AxisDirection = OBB.AxisDirectionLocal[a];
        f32 AxisLength = OBB.AxisLength.E[a];
        f32 Distance = dot_vec3(PC, AxisDirection); //Get distance by projecting point from center onto the axis's direction.
        //If projected distance is greater or less than the axis's length, clamp to the box.
        if(Distance > AxisLength)
        {
            Distance = AxisLength;
            (*IsIn) = false;
        }
        if(Distance < -AxisLength)
        {
            Distance = -AxisLength;
            (*IsIn) = false;
        }
        //Step that distance along the axis to get the world coordinate.
        Result += Distance * AxisDirection;
    }
    
    return Result;
}

oriented_bounding_box TransformOBB(world_transform Transform, oriented_bounding_box Base)
{
    oriented_bounding_box Result = Base;
    Result.Center = Base.Center + Transform.Translation;
    mat3x3 Rotation = RotationMatrix(Transform.RotationAxes);
    Result.AxisDirectionLocal[0] = Rotation * Base.AxisDirectionLocal[0];
    Result.AxisDirectionLocal[1] = Rotation * Base.AxisDirectionLocal[1];
    Result.AxisDirectionLocal[2] = Rotation * Base.AxisDirectionLocal[2];
    return Result;
}