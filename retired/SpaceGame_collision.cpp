inline vec3 support(vec3 searchdir, vertex* A, vec3 playerpos, bit32 sizeofA_VEC3)
{ //NOTE: This isn't a "correct" support routine. A "correct" support routine would do a similar operation on a full mesh for the player, but since I am just a point, I do it this way.
    bit32 f = 0; 
    {//A
        f32 maxdistance = dot_vec3(searchdir, A[0].Position);
        for(bit32 i = f; i < sizeofA_VEC3; i++)
        {
            f32 dot = dot_vec3(searchdir, A[i].Position);
            if(dot > maxdistance){ maxdistance = dot; f = i; }
        }
    }
    
#if collisiondebug
    if(!collisionDEBUGpassed) { collisionDEBUG_finalsimplexindicies[collisionDEBUG_currentSBI] = f+ (collisionDEBUG_multiplier*sizeofA_VEC3); }
#endif
    
    return A[f].Position - playerpos;
}