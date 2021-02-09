struct Vertex
{
    vec3 position;
    bit32 color;
};

struct Bunker
{
    Vertex vertex[12];
    bit16 index[54];
    vec3 floorcollisiondata[8];
    vec3 ceilingcollisiondata[8];
};

#define maxbunkers 2
struct BunkerManager
{
    bit32 active; bit32 padding;
    Bunker bunker[maxbunkers];
};

#define resetbunkergraphics()\


inline void setupbunkerdata(Bunker* bunker)
{
    {//Collision Data
        f32 collisionboxheight = 0.1f;
        {//Floor
            //Upper
            bunker->floorcollisiondata[0].x = -0.4f; bunker->floorcollisiondata[0].y = -0.4f; bunker->floorcollisiondata[0].position.z = 0.4f;
            
            bunker->floorcollisiondata[1] = bunker->vertex[3].position;
            bunker->floorcollisiondata[2] = bunker->vertex[4].position;
            bunker->floorcollisiondata[3] = bunker->vertex[2].position;
            //Lower
            bunker->floorcollisiondata[4] = bunker->vertex[0].position - collisionboxheight;
            bunker->floorcollisiondata[5] = bunker->vertex[3].position - collisionboxheight;
            bunker->floorcollisiondata[6] = bunker->vertex[4].position - collisionboxheight;
            bunker->floorcollisiondata[7] = bunker->vertex[2].position - collisionboxheight;
        }
        {//Ceiling
            //Upper
            bunker->ceilingcollisiondata[8] = bunker->vertex[6].position;
            bunker->ceilingcollisiondata[9] = bunker->vertex[9].position;
            bunker->ceilingcollisiondata[10] = bunker->vertex[10].position;
            bunker->ceilingcollisiondata[11] = bunker->vertex[8].position;
            //Lower
            bunker->ceilingcollisiondata[12] = bunker->vertex[6].position + collisionboxheight;
            bunker->ceilingcollisiondata[13] = bunker->vertex[9].position + collisionboxheight;
            bunker->ceilingcollisiondata[14] = bunker->vertex[10].position + collisionboxheight;
            bunker->ceilingcollisiondata[15] = bunker->vertex[8].position + collisionboxheight;
        }
    }
    
    {//GraphicData()
        {//NOTE: Base vertex setup
            //TODO(Andrew) BROKEN ATM 4/9/20
            bunker->vertex[0].position = bunker->floorcollisiondata[0];
            bunker->vertex[1].position = bunker->floorcollisiondata[0];  bunker->vertex[1].position.z -= 0.8f;
            bunker->vertex[2].position.x = -0.9f; bunker->vertex[2].position.y = -0.4f; bunker->vertex[2].position.z = -0.4f;
            bunker->vertex[3].position.x = 0.4f; bunker->vertex[3].position.y = -0.4f; bunker->vertex[3].position.z = 0.4f;
            bunker->vertex[4].position.x = 0.9f; bunker->vertex[4].position.y = -0.4f; bunker->vertex[4].position.z = -0.4f;
            bunker->vertex[5].position.x = 0.4f; bunker->vertex[5].position.y = -0.4f; bunker->vertex[5].position.z = -0.4f;
            //Lower
            bunker->vertex[6].position.x = -0.4f; bunker->vertex[6].position.y = 0.4f; bunker->vertex[6].position.z = 0.4f;
            bunker->vertex[7].position.x = -0.4f; bunker->vertex[7].position.y = 0.4f; bunker->vertex[7].position.z = -0.4f;
            bunker->vertex[8].position.x = -0.9f; bunker->vertex[8].position.y = 0.4f; bunker->vertex[8].position.z = -0.4f;
            bunker->vertex[9].position.x = 0.4f; bunker->vertex[9].position.y = 0.4f; bunker->vertex[9].position.z = 0.4f;
            bunker->vertex[10].position.x = 0.9f; bunker->vertex[10].position.y = 0.4f; bunker->vertex[10].position.z = -0.4f;
            bunker->vertex[11].position.x = 0.4f; bunker->vertex[11].position.y = 0.4f; bunker->vertex[11].position.z = -0.4f;
        }
        {//NOTE: Base vertex color setup
            bunker->vertex[0].color = 0xFF0000FF; //Red
            bunker->vertex[1].color = 0xFF0080FF; //Orange
            bunker->vertex[2].color = 0xFF00FFFF; //Yellow
            bunker->vertex[3].color = 0xFF00FF80; //Lime Green
            bunker->vertex[4].color = 0xFF00FF00; //Green
            bunker->vertex[5].color = 0xFF00FF80; //Light Green
            bunker->vertex[6].color = 0xFFFFFF00; //Cyan
            bunker->vertex[7].color = 0xFFFF8000; //Light Blue
            bunker->vertex[8].color = 0xFFFF0000; //Blue
            bunker->vertex[9].color = 0xFFFF007F; //Royal Purple
            bunker->vertex[10].color = 0xFFFF00FF; //Saint's purple
            bunker->vertex[11].color = 0xFF7F00FF; //Weird red pink
            bunker->vertex[11].color = 0xFF7F00FF; //Weird red pink
        }
        
        //INDEX
        bit32 i = 0;
        //Floor
        bunker->index[i] = 0; bunker->index[i+1] = 1; bunker->index[i+2] = 2; i+=3; 
        bunker->index[i] = 3; bunker->index[i+1] = 1; bunker->index[i+2] = 0; i+=3; 
        bunker->index[i] = 3; bunker->index[i+1] = 5; bunker->index[i+2] = 1; i+=3; 
        bunker->index[i] = 3; bunker->index[i+1] = 4; bunker->index[i+2] = 5; i+=3;
        //Left side
        bunker->index[i] = 0; bunker->index[i+1] = 2; bunker->index[i+2] = 6; i+=3; 
        bunker->index[i] = 6; bunker->index[i+1] = 2; bunker->index[i+2] = 8; i+=3; 
        bunker->index[i] = 7; bunker->index[i+1] = 2; bunker->index[i+2] = 8; i+=3; 
        bunker->index[i] = 1; bunker->index[i+1] = 2; bunker->index[i+2] = 7; i+=3; 
        //Back wall
        bunker->index[i] = 0; bunker->index[i+1] = 9; bunker->index[i+2] = 3; i+=3; 
        bunker->index[i] = 9; bunker->index[i+1] = 0; bunker->index[i+2] = 6; i+=3; 
        //Right Side
        bunker->index[i] = 3; bunker->index[i+1] = 10; bunker->index[i+2] = 4; i+=3; 
        bunker->index[i] = 10; bunker->index[i+1] = 3; bunker->index[i+2] = 9; i+=3; 
        bunker->index[i] = 5; bunker->index[i+1] = 10; bunker->index[i+2] = 4; i+=3; 
        bunker->index[i] = 10; bunker->index[i+1] = 5; bunker->index[i+2] = 11; i+=3; 
        //Ceiling
        bunker->index[i] = 6; bunker->index[i+1] = 7; bunker->index[i+2] = 8; i+=3; 
        bunker->index[i] = 9; bunker->index[i+1] = 7; bunker->index[i+2] = 6; i+=3; 
        bunker->index[i] = 9; bunker->index[i+1] = 11; bunker->index[i+2] = 7; i+=3; 
        bunker->index[i] = 9; bunker->index[i+1] = 10; bunker->index[i+2] = 11; i+=3; 
    }
}


BunkerManager* bunkermanager; //NOTE: global bunker manager
inline void setupbunkermanagerdata()
{
    bunkermanager = (BunkerManager*)recordpush(sizeof(bunkermanager));
    for(bunkermanager->active = 0; bunkermanager->active < maxbunkers; bunkermanager->active++ )
    {
        setupbunkerdata(&bunkermanager->bunker[bunkermanager->active]);
    }
    //NOTE: setup preliminary bunkers
    {//Starting bunker
        mat3x3 R = rotate3x3Y(Radians(180.0f));
        for(bit32 i = 0; i < sizeof(bunkermanager->bunker[0].vertex)/sizeof(Vertex); i++)
        {
            bunkermanager->bunker[0].vertex[i].position = R * bunkermanager->bunker[0].vertex[i].position;
            bunkermanager->bunker[0].vertex[i].position.z-=0.4f;
        }
    }
    
    for(bit32 i = 0; i < sizeof(bunkermanager->bunker[1].vertex)/sizeof(Vertex); i++)
    { //Across from start
        bunkermanager->bunker[1].vertex[i].position.z+=2.0f;
    }
}