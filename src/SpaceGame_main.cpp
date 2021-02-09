bool32 gameisrunning = true; //NOTE: global: game is running.
bit64 main_lastframeTIME = 0; //NOTE global: main thread's last frame time.

inline void spacegamemain()
{
    bit32 backbuffercolor = 0xFF000000;
    f32 gravity = .01f;
    for(;gameisrunning;)
    {
        if(player->position.y < -1.0f)
        {//If player falls
            resettempleplatformposition(0);
            {//Reset player if they fall too low.
                resetplayerlaunch();
                player->rotation = identity3x3(); player->theta = 0.0f;
                player->onplatform = 0;
                f32 offset = .01f;
                player->position.x = platform[0].floorCD[0].position.x + offset;
                player->position.y = platform[0].floorCD[0].position.y+.01f;
                player->position.z = platform[0].floorCD[0].position.z - offset;
            }
            player->numberoftimes_landed = 0; //Reset player's score.
        }
        
        {//Move platform(s)
            for(bit32 p = 0; p < sizeof(platform)/sizeof(TemplePlatform); p++)
            {
                if(p != player->onplatform)
                {
                    bit32 increment = 1;
                    platform[p].movedata.timer += increment; 
                    if(platform[p].movedata.timer >= 30){ platform[p].movedata.timer = 0; platform[p].movedata.translationaxes = -platform[p].movedata.translationaxes; }
                    for(bit32 i = 0; i < sizeof(platform[p].floorCD)/sizeof(Vertex) + sizeof(platform[p].ceilingCD)/sizeof(Vertex); i++)
                    {//This transforms all vertexes (including ceiling vertexes!)
                        platform[p].floorCD[i].position =  platform[p].floorCD[i].position + platform[p].movedata.translationaxes;
                    }
                }
            }
        }
        
        if(player->manipulationInfo != playerMIDAIR)
        {//Control Camera()
            if(platform_getinput())
            { 
                if(player->manipulationInfo == 2)
                {
                    player->baseposition = player->position;
                    player->launchDirection = player->baseposition + (player->rotation * player->launchDirection);
                    player->manipulationInfo = playerMIDAIR;
                    goto MIDAIR;
                }
                else if(player->manipulationInfo == 1) {player->manipulationInfo  = 2;} 
                else{player->manipulationInfo = 1;} 
            }
            f32 increase = 0.01f;
            if(player->manipulationInfo == 0)  { player->theta += increase;  if(player->theta >= PI32*2){ player->theta = 0.0f; }  player->rotation = rotate3x3Y(player->theta); }
            else if(player->manipulationInfo == 1) {  player->launchDirection.y += increase; if(player->launchDirection.y >= 1.0f){ player->launchDirection.y = 0.0f; } }
            else if (player->manipulationInfo == 2) { player->launchDirection.z += 0.01f; if(player->launchDirection.z >= 1.0f){player->launchDirection.z = 0.1f;} }
        }
        else
        {  MIDAIR:
            //Old "maybe good" rigid body physics code for movement.
            //vec3 acceleration = player->launchDirection; vec3 velocity = player->velocity; vec3 oldposition = player->position;
            //f32 time = platform_querycurrenttime(main_lastframeTIME); 
            //player->position += (0.5f*acceleration*(time*time)) + velocity*time + oldposition;
            //player->velocity = acceleration * time + velocity;
            player->lerp+=0.1f; 
            if(player->lerp >= 1.1f){player->lerp = 1.1f;}
            else{player->position = lerp_vec3(player->baseposition, player->launchDirection, player->lerp);}
            //Apply gravity to the interpolated values that player is pulling from.
        }
        
        {//Apply gravity
            player->position.y -= gravity;
        }
        
        for(bit32 p = 0; p < sizeof(platform)/sizeof(TemplePlatform); p++)
        {//Collision "GJK" on the ingame platforms.
            bool32 floorchecked = false;
            bit32 offset = 0; Vertex* collisiondata = nullptr; bit32 numberofvertexes = 0;
            checkcollision:
            if(floorchecked) { collisiondata = platform[p].ceilingCD; numberofvertexes = sizeof(platform[p].ceilingCD)/sizeof(Vertex); }
            else { collisiondata = platform[p].floorCD; numberofvertexes = sizeof(platform[p].floorCD)/sizeof(Vertex); }
            
            bit32 S = 0;
            {//Find start vertex in mesh
                f32 mindot = dot_vec3(collisiondata[S].position - player->position, collisiondata[S].position - player->position);
                for(bit32 i = 0; i < numberofvertexes; i++)
                {
                    vec3 PM = collisiondata[i].position - player->position;
                    f32 dot = dot_vec3(PM, PM);
                    if(dot < mindot) { mindot = dot; S = i; }
                }
            }
#if collisiondebug
            if(!collisionDEBUGpassed){ collisionDEBUG_multiplier++; collisionDEBUG_currentSBI = 0;}
#endif
            vec3 simplex_buffer[4]; bit32 sbi = 0;
            simplex_buffer[sbi] = support(collisiondata[S].position - player->position, collisiondata, player->position, numberofvertexes);
            vec3 searchdir = -simplex_buffer[sbi]; sbi++;
#if collisiondebug
            collisionDEBUG_currentSBI = sbi;
#endif
            
            for( ; sbi<4 ; )
            {
                GetNextSimplexPoint:
                {//Get next point in simplex()
                    vec3 supportvector = support(searchdir, collisiondata, player->position, numberofvertexes);
                    if(dot_vec3(supportvector, searchdir) < 0){ goto EndOfSimplex; } //Early out because you could not contain the next "most close to origin" vector.
                    simplex_buffer[sbi] = supportvector; sbi++;
#if collisiondebug
                    collisionDEBUG_currentSBI = sbi;
#endif
                }
                
                //TODO(Andrew) 2/4/21 ABSOLUTELY RECHECK / REDO THIS GJK IMPLEMENT AS IT SHOULD BE INCORRECT!
                {//dosimplex() "Dependant on the shape of the simplex, choose a particular set of plane tests to find out if the origin is enclosed in a tetrahedron".
                    if(sbi == 2) //line
                    {
                        vec3 AO = -simplex_buffer[1];
                        vec3 AB = simplex_buffer[0] - simplex_buffer[1];
                        searchdir = cross(cross(AB, AO), AB);
                    }
                    else if(sbi == 3) //triangle
                    {
                        vec3 AO = -simplex_buffer[2];
                        vec3 AB = simplex_buffer[1] - simplex_buffer[2];
                        vec3 AC = simplex_buffer[0] - simplex_buffer[2];
                        vec3 ABC = cross(AB, AC);
                        if(dot_vec3(cross(ABC, AC), AO) > 0)
                        {
                            if(dot_vec3(AC, AO) > 0) { searchdir = cross(cross(AC, AO), AC); }
                            else
                            {
                                if(dot_vec3(AB, AO) > 0){ searchdir = cross(cross(AB, AO), AB); }
                                else{ searchdir = AO; } 
                            }
                        }
                        else
                        {
                            if(dot_vec3(cross(AB, ABC), AO) > 0)
                            {
                                if(dot_vec3(AB, AO) > 0) { searchdir = cross(cross(AB, AO), AB); }
                                else { searchdir = AO; }
                            }
                            else
                            {
                                if(dot_vec3(ABC, AO) > 0) { searchdir = ABC; }
                                else { searchdir = -ABC; }
                            }
                        }
                    }
                    else if(sbi == 4)
                    {
                        vec3 AO = -simplex_buffer[3];
                        vec3 AB = simplex_buffer[2] - simplex_buffer[3];
                        vec3 AC = simplex_buffer[1] - simplex_buffer[3];
                        vec3 AD = simplex_buffer[0] - simplex_buffer[3];
                        vec3 ABC = cross(AB, AC);
                        vec3 ACD = cross(AC, AD);
                        vec3 ADB = cross(AD, AB);
                        
                        //Check "seams" to early out, if possible.
                        if(dot_vec3(AO, AB) < 0)
                        {
                            if(dot_vec3(AO, AC) < 0)
                            {
                                if(dot_vec3(AO, AD) < 0){  goto EndOfSimplex; } //Is in the voronoi region behind A (above the tetrahedron), so, failed collision.
                            }
                        }
                        
                        //Edge AB
                        if(dot_vec3(AO, AB) > 0)
                        {
                            vec3 BO = -simplex_buffer[2]; vec3 BA = simplex_buffer[3] - simplex_buffer[2]; 
                            if(dot_vec3(BO, BA) > 0) //This backwards seam check might not be required; The same goes for the other edges that do this.
                            {
                                if(dot_vec3(AO, cross(AB, ABC)) > 0)
                                {
                                    if(dot_vec3(AO, cross(ADB, AB)) > 0)
                                    { //100% outside the tetrahedron, so try to see if there's another vector that would enclose the origin. (try to recover)
                                        searchdir = cross(cross(AB, AO), AB);
                                        vec3 sqrtaprxABC = ABC - searchdir; vec3 sqrtaprxADB = ADB - searchdir;
                                        if(dot_vec3(sqrtaprxABC, sqrtaprxABC) < dot_vec3(sqrtaprxADB, sqrtaprxADB))
                                        { simplex_buffer[0] = simplex_buffer[1]; simplex_buffer[1] = simplex_buffer[2]; simplex_buffer[2] = simplex_buffer[3]; }
                                        else{ simplex_buffer[1] = simplex_buffer[0]; simplex_buffer[0] = simplex_buffer[2]; simplex_buffer[2] = simplex_buffer[3]; }
                                        sbi--; 
                                        goto GetNextSimplexPoint;
                                    }
                                }
                            }
                        }
                        
                        //Edge_AC
                        if(dot_vec3(AO, AC) > 0)
                        {
                            vec3 CO = -simplex_buffer[1]; vec3 CA = simplex_buffer[3] - simplex_buffer[1];
                            if(dot_vec3(CO, CA) > 0)
                            {
                                if(dot_vec3(AO, cross(AC, ACD)) > 0)
                                {
                                    if(dot_vec3(AO, cross(ABC, AC)) > 0)
                                    { //Same case as B, 100% outside the tetrahedron, so see if there's another vector that would enclose the origin. (try to recover)
                                        searchdir = cross(cross(AC, AO), AC);
                                        vec3 sqrtaprxACD = ACD - searchdir; vec3 sqrtaprxABC = ABC - searchdir;
                                        if(dot_vec3(sqrtaprxACD, sqrtaprxACD) < dot_vec3(sqrtaprxABC, sqrtaprxABC)) 
                                        { simplex_buffer[2] = simplex_buffer[3]; }
                                        else { simplex_buffer[0] = simplex_buffer[1]; simplex_buffer[1] = simplex_buffer[2]; simplex_buffer[2] = simplex_buffer[3]; } //ABC side
                                        sbi--; 
                                        goto GetNextSimplexPoint;
                                    }
                                }
                            }
                        }
                        
                        //Edge_AD
                        if(dot_vec3(AO, AD) > 0)
                        {
                            vec3 DO = -simplex_buffer[0]; vec3 DA = simplex_buffer[3] - simplex_buffer[0];
                            if(dot_vec3(DO, DA) > 0)
                            {
                                if(dot_vec3(AO, cross(AD, ADB)) > 0)
                                {
                                    if(dot_vec3(AO, cross(ACD, AD)) > 0)
                                    {//Same as B and C's outside tetrahedron cases. (try to recover)
                                        searchdir = cross(cross(AD, AO), AD);
                                        vec3 sqrtaprxADB = ADB - searchdir; vec3 sqrtaprxACD = ACD - searchdir;
                                        if(dot_vec3(sqrtaprxADB, sqrtaprxADB) < dot_vec3(sqrtaprxACD, sqrtaprxACD))
                                        { simplex_buffer[1] = simplex_buffer[0]; simplex_buffer[0] = simplex_buffer[2]; simplex_buffer[2] = simplex_buffer[3]; }
                                        else { simplex_buffer[2] = simplex_buffer[3]; }
                                        sbi--;
                                        goto GetNextSimplexPoint;
                                    }
                                }
                            }
                        }
                        
                        //face_ABC
                        if(dot_vec3(ABC, AO) < 0) { simplex_buffer[0]  = simplex_buffer[1]; simplex_buffer[1] = simplex_buffer[2]; simplex_buffer[2] = simplex_buffer[3]; searchdir = -ABC; sbi--; }
                        else if (dot_vec3(ACD, AO) < 0) { simplex_buffer[2] = simplex_buffer[3]; searchdir = -ACD; sbi--; }
                        else if(dot_vec3(ADB, AO) < 0) { simplex_buffer[1] = simplex_buffer[0]; simplex_buffer[0] = simplex_buffer[2]; simplex_buffer[2] = simplex_buffer[3]; searchdir = -ADB; sbi--; }
                        else 
                        {//HANDLE COLLSIION BECAUSE THERE WAS A COLLISION!
#if collisiondebug
                            collisionDEBUGpassed = true;
#endif
                            player->position.y += gravity;
                            if(player->manipulationInfo == playerMIDAIR)  
                            { 
                                if(player->onplatform != p){player->numberoftimes_landed++;}
                                player->onplatform = p;  resetplayerlaunch();
                            }
                            goto EndOfSimplex;
                        }
                    }
                    else
                    {
                        goto EndOfSimplex;
                    }
                }
                
            }//End of simplex loop
            EndOfSimplex:
            if(!floorchecked){ floorchecked = true; goto checkcollision; }
        }
        
        platform_render();
        platform_printinfo();
        platform_sleep(main_lastframeTIME);
        platform_bufferswap(backbuffercolor);
    } //End of game loop
}