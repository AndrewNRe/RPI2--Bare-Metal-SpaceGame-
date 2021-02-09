struct Vertex
{
    vec3 position;
    bit32 color;
};

struct TemplePlatformMove
{
    vec3 translationaxes;
    bit32 timer;
};

#define templeplatformcollisionshapes 2
struct TemplePlatform
{
    Vertex floorCD[8];
    Vertex ceilingCD[8];
    bit16 index[72];
    TemplePlatformMove movedata;
};

TemplePlatform platform[2]; //NOTE: global: platforms

#define resettempleplatformposition(value) { f32 collisionboxsize = 0.1f; bit32 platformindex = value;\
    {/*Floor*/\
        platform[platformindex].floorCD[0].position.x = -0.4f; platform[platformindex].floorCD[0].position.y = -0.4f; platform[platformindex].floorCD[0].position.z = 0.4f; platform[platformindex].floorCD[0].color = 0xFF0000FF;\
        platform[platformindex].floorCD[1].position.x = 0.4f; platform[platformindex].floorCD[1].position.y = -0.4f; platform[platformindex].floorCD[1].position.z = 0.4f; platform[platformindex].floorCD[1].color = 0xFF0080FF;\
        platform[platformindex].floorCD[2].position.x = 0.9f; platform[platformindex].floorCD[2].position.y = -0.4f; platform[platformindex].floorCD[2].position.z = -0.4f; platform[platformindex].floorCD[2].color = 0xFF00FFFF;\
        platform[platformindex].floorCD[3].position.x = -0.9f; platform[platformindex].floorCD[3].position.y = -0.4f; platform[platformindex].floorCD[3].position.z = -0.4f; platform[platformindex].floorCD[3].color = 0xFF00FF80;\
        platform[platformindex].floorCD[4].position.x = -0.4f; platform[platformindex].floorCD[4].position.y = -0.4f - collisionboxsize; platform[platformindex].floorCD[4].position.z = 0.4f; platform[platformindex].floorCD[4].color = 0xFF00FF00;\
        platform[platformindex].floorCD[5].position.x = 0.4f; platform[platformindex].floorCD[5].position.y = -0.4f - collisionboxsize; platform[platformindex].floorCD[5].position.z = 0.4f; platform[platformindex].floorCD[5].color = 0xFF00FF80;\
        platform[platformindex].floorCD[6].position.x = 0.9f; platform[platformindex].floorCD[6].position.y = -0.4f - collisionboxsize; platform[platformindex].floorCD[6].position.z = -0.4f; platform[platformindex].floorCD[6].color = 0xFFFFFF00;\
        platform[platformindex].floorCD[7].position.x = -0.9f; platform[platformindex].floorCD[7].position.y = -0.4f - collisionboxsize; platform[platformindex].floorCD[7].position.z = -0.4f; platform[platformindex].floorCD[7].color = 0xFFFF8000;\
    }\
    {/*Ceiling*/\
        platform[platformindex].ceilingCD[0].position.x = -0.4f; platform[platformindex].ceilingCD[0].position.y = 0.4f; platform[platformindex].ceilingCD[0].position.z = 0.4f; platform[platformindex].ceilingCD[0].color = 0xFFFF0000;\
        platform[platformindex].ceilingCD[1].position.x = 0.4f; platform[platformindex].ceilingCD[1].position.y = 0.4f; platform[platformindex].ceilingCD[1].position.z = 0.4f; platform[platformindex].ceilingCD[1].color = 0xFFFF007F;\
        platform[platformindex].ceilingCD[2].position.x = 0.9f; platform[platformindex].ceilingCD[2].position.y = 0.4f; platform[platformindex].ceilingCD[2].position.z = -0.4f; platform[platformindex].ceilingCD[2].color = 0xFFFF00FF;\
        platform[platformindex].ceilingCD[3].position.x = -0.9f; platform[platformindex].ceilingCD[3].position.y = 0.4f; platform[platformindex].ceilingCD[3].position.z = -0.4f; platform[platformindex].ceilingCD[3].color = 0xFF7F00FF;\
        platform[platformindex].ceilingCD[4].position.x = -0.4f; platform[platformindex].ceilingCD[4].position.y = 0.4f + collisionboxsize; platform[platformindex].ceilingCD[4].position.z = 0.4f; platform[platformindex].ceilingCD[4].color = 0xFF7F00FF;\
        platform[platformindex].ceilingCD[5].position.x = 0.4f; platform[platformindex].ceilingCD[5].position.y = 0.4f + collisionboxsize; platform[platformindex].ceilingCD[5].position.z = 0.4f; platform[platformindex].ceilingCD[5].color = 0xFF0000FF;\
        platform[platformindex].ceilingCD[6].position.x = 0.9f; platform[platformindex].ceilingCD[6].position.y = 0.4f + collisionboxsize; platform[platformindex].ceilingCD[6].position.z = -0.4f; platform[platformindex].ceilingCD[6].color = 0xFF0080FF;\
        platform[platformindex].ceilingCD[7].position.x = -0.9f; platform[platformindex].ceilingCD[7].position.y = 0.4f + collisionboxsize; platform[platformindex].ceilingCD[7].position.z = -0.4f; platform[platformindex].ceilingCD[7].color = 0xFF00FFFF;\
    }\
    {/*Move Data Reset*/\
        platform[platformindex].movedata.timer = 0;\
    }\
};

inline void setuptempleplatforms()
{
    for(bit32 p = 0; p < sizeof(platform)/sizeof(TemplePlatform); p++)
    {
        resettempleplatformposition(p);
        {//Setup Index Buffer
            bit32 i = 0; bit32 offset = 0;
            {//Floor
                //Back
                platform[p].index[i] = 1; platform[p].index[i+1] = 0; platform[p].index[i+2] = 4; i+=3;
                platform[p].index[i] = 1; platform[p].index[i+1] = 4; platform[p].index[i+2] = 5; i+=3;
                //Right
                platform[p].index[i] = 2; platform[p].index[i+1] = 1; platform[p].index[i+2] = 5; i+=3;
                platform[p].index[i] = 2; platform[p].index[i+1] = 5; platform[p].index[i+2] = 6; i+=3;
                //Front
                platform[p].index[i] = 3; platform[p].index[i+1] = 2; platform[p].index[i+2] = 6; i+=3;
                platform[p].index[i] = 3; platform[p].index[i+1] = 6; platform[p].index[i+2] = 7; i+=3;
                //Left
                platform[p].index[i] = 0; platform[p].index[i+1] = 3; platform[p].index[i+2] = 7; i+=3;
                platform[p].index[i] = 0; platform[p].index[i+1] = 7; platform[p].index[i+2] = 4; i+=3;
                //Top
                platform[p].index[i] = 0; platform[p].index[i+1] = 1; platform[p].index[i+2] = 2; i+=3;
                platform[p].index[i] = 0; platform[p].index[i+1] = 2; platform[p].index[i+2] = 3; i+=3;
                //Bottom
                platform[p].index[i] = 4; platform[p].index[i+1] = 5; platform[p].index[i+2] = 6; i+=3;
                platform[p].index[i] = 4; platform[p].index[i+1] = 6; platform[p].index[i+2] = 7; i+=3;
            }
            offset = 8;
            {//Ceiling
                //Back
                platform[p].index[i] = 1 + offset; platform[p].index[i+1] = 0 + offset; platform[p].index[i+2] = 4 + offset; i+=3;
                platform[p].index[i] = 1 + offset; platform[p].index[i+1] = 4 + offset; platform[p].index[i+2] = 5 + offset; i+=3;
                //Right
                platform[p].index[i] = 2 + offset; platform[p].index[i+1] = 1 + offset; platform[p].index[i+2] = 5 + offset; i+=3;
                platform[p].index[i] = 2 + offset; platform[p].index[i+1] = 5 + offset; platform[p].index[i+2] = 6 + offset; i+=3;
                //Front
                platform[p].index[i] = 3 + offset; platform[p].index[i+1] = 2 + offset; platform[p].index[i+2] = 6 + offset; i+=3;
                platform[p].index[i] = 3 + offset; platform[p].index[i+1] = 6 + offset; platform[p].index[i+2] = 7 + offset; i+=3;
                //Left
                platform[p].index[i] = 0 + offset; platform[p].index[i+1] = 3 + offset; platform[p].index[i+2] = 7 + offset; i+=3;
                platform[p].index[i] = 0 + offset; platform[p].index[i+1] = 7 + offset; platform[p].index[i+2] = 4 + offset; i+=3;
                //Top
                platform[p].index[i] = 0 + offset; platform[p].index[i+1] = 1 + offset; platform[p].index[i+2] = 2 + offset; i+=3;
                platform[p].index[i] = 0 + offset; platform[p].index[i+1] = 2 + offset; platform[p].index[i+2] = 3 + offset; i+=3;
                //Bottom
                platform[p].index[i] = 4 + offset; platform[p].index[i+1] = 5 + offset; platform[p].index[i+2] = 6 + offset; i+=3;
                platform[p].index[i] = 4 + offset; platform[p].index[i+1] = 6 + offset; platform[p].index[i+2] = 7 + offset; i+=3;
            }
        }
    }
    
    
    {//First Bunker Specific Data
        bit32 ID = 0;
        platform[ID].movedata.translationaxes;
        platform[ID].movedata.translationaxes.x = 0.008f; platform[ID].movedata.translationaxes.y = 0.001f; platform[ID].movedata.translationaxes.z = 0.0f;
    }
    {//Second Bunker Specific Data
        bit32 ID = 1;
        vec3 moveposition;
        moveposition.x = 0.0f; moveposition.y = 0.0f; moveposition.z = 1.5;
        {//Translate
            for(bit32 i = 0; i < sizeof(platform[ID].floorCD)/sizeof(Vertex) + sizeof(platform[ID].ceilingCD)/sizeof(Vertex); i++)
            {//This transforms all vertexes (including ceiling vertexes!)
                platform[ID].floorCD[i].position = platform[ID].floorCD[i].position + moveposition;
            }
        }
        {//Platform Move Data
            platform[ID].movedata.translationaxes.x = -0.008f; platform[ID].movedata.translationaxes.y = 0.0f; platform[ID].movedata.translationaxes.z = 0.0f;
        }
    }
}