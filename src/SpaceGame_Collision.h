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

oriented_bounding_box TransformOBB(vec3 TargetTransform, oriented_bounding_box Base)
{
    oriented_bounding_box Result = Base;
    Result.Center = Base.Center + TargetTransform;
    return Result;
}

inline mat3x3 OBBMatrixEncodingOrientation(oriented_bounding_box OBB)
{
    mat3x3 Result;
    Result.d[0][0] = OBB.AxisDirectionLocal[0].x; Result.d[0][1] = OBB.AxisDirectionLocal[1].x; Result.d[0][2] = OBB.AxisDirectionLocal[2].x;
    Result.d[1][0] = OBB.AxisDirectionLocal[0].y; Result.d[1][1] = OBB.AxisDirectionLocal[1].y; Result.d[1][2] = OBB.AxisDirectionLocal[2].y;
    Result.d[2][0] = OBB.AxisDirectionLocal[0].z; Result.d[2][1] = OBB.AxisDirectionLocal[1].z; Result.d[2][2] = OBB.AxisDirectionLocal[2].z;
    return Result;
}

render_box GenerateBoxGraphicsData(oriented_bounding_box OBB, vec3 TargetTransform, bit32 Color)
{
    render_box Result = {};
    
    {//Set the 8 vertexes to the proper positions.
        bit32 v = 0;
        vec3 T = OBB.Center;
        vec3 S = OBB.AxisLength;
        mat3x3 Rotation = OBBMatrixEncodingOrientation(OBB);
        Result.Vertex[v].Position = (Rotation * vec3({+S.x, +S.y, +S.z}) ) + T; Result.Vertex[v].Color = Color; v++;
        Result.Vertex[v].Position = (Rotation * vec3({+S.x, +S.y,  -S.z}) ) + T; Result.Vertex[v].Color = Color; v++;
        Result.Vertex[v].Position = (Rotation * vec3({ -S.x, +S.y,  -S.z}) ) + T; Result.Vertex[v].Color = Color; v++;
        Result.Vertex[v].Position = (Rotation * vec3({ -S.x, +S.y, +S.z}) ) + T; Result.Vertex[v].Color = Color; v++;
        Result.Vertex[v].Position = (Rotation * vec3({+S.x,  -S.y, +S.z}) ) + T; Result.Vertex[v].Color = Color; v++;
        Result.Vertex[v].Position = (Rotation * vec3({+S.x,  -S.y,  -S.z}) ) + T; Result.Vertex[v].Color = Color; v++;
        Result.Vertex[v].Position = (Rotation * vec3({ -S.x,  -S.y,  -S.z}) ) + T; Result.Vertex[v].Color = Color; v++;
        Result.Vertex[v].Position = (Rotation * vec3({ -S.x,  -S.y, +S.z}) ) + T; Result.Vertex[v].Color = Color; v++;
    }
    
    for(bit32 v = 0;
        v < 8;
        v++)
    {
        Result.Vertex[v].Position = Result.Vertex[v].Position + TargetTransform;
    }
    
    {//Setup the index buffer
        bit32 i = 0;
        //Front
        Result.Index[i] = 4; Result.Index[i+1] = 0; Result.Index[i+2] = 7; i += 3;
        Result.Index[i] = 0; Result.Index[i+1] = 3; Result.Index[i+2] = 7; i += 3;
        //Top
        Result.Index[i] = 0; Result.Index[i+1] = 1; Result.Index[i+2] = 3; i += 3;
        Result.Index[i] = 1; Result.Index[i+1] = 2; Result.Index[i+2] = 3; i += 3;
        //Right
        Result.Index[i] = 5; Result.Index[i+1] = 1; Result.Index[i+2] = 4; i += 3;
        Result.Index[i] = 1; Result.Index[i+1] = 0; Result.Index[i+2] = 4; i += 3;
        //Back
        Result.Index[i] = 6; Result.Index[i+1] = 2; Result.Index[i+2] = 5; i += 3;
        Result.Index[i] = 2; Result.Index[i+1] = 1; Result.Index[i+2] = 5; i += 3;
        //Left
        Result.Index[i] = 7; Result.Index[i+1] = 3; Result.Index[i+2] = 6; i += 3;
        Result.Index[i] = 3; Result.Index[i+1] = 2; Result.Index[i+2] = 6; i += 3;
        //Bottom
        Result.Index[i] = 5; Result.Index[i+1] = 4; Result.Index[i+2] = 6; i += 3;
        Result.Index[i] = 4; Result.Index[i+1] = 7; Result.Index[i+2] = 6; i += 3;
    }
    return Result;
}