enum input_state
{
    Input_Rotated,
    Input_SideMoved,
    Input_UpMoved,
    Input_InMoved,
    Input_Power,
    Input_End,
};
struct game_input
{
    bit32 ButtonDown; //NOTE: Can only be held for 4,294,967,295 frames. If you hold more than that, you'll just wrap and be valid for another frame. lol
    bool32 StartMove;
    input_state State;
};