struct Player
{
    bit32 manipulationInfo;
    mat3x3 rotation; f32 theta;
    f32 lerp;
    vec3 position;
    vec3 launchDirection;
    vec3 baseposition;
    bit16 onplatform;
    bit16 numberoftimes_landed;
};
Player* player; //NOTE: global: player struct
#define cameraMAXY (PI32*3)/2
#define playerMIDAIR 3

inline void resetplayerlaunch()
{
    player->manipulationInfo = 0;
    player->launchDirection.x = 0.0f; player->launchDirection.y = 0.0f; player->launchDirection.z = 0.0f;
    player->baseposition = player->position;
    player->lerp = 0.0f;
}

inline void setupplayerdata()
{
    player = (Player*)recordpush(sizeof(Player));
    resetplayerlaunch();
    
    player->onplatform = 0;
    player->numberoftimes_landed = 0;
    player->position.x = 0; player->position.y = 0; player->position.z = -0.3;
    player->baseposition = player->position;
    player->lerp = 1.1f;
    player->manipulationInfo = playerMIDAIR;
}