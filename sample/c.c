typedef struct Position {
    double x;
    double y;
} Position;

Position get_position()
{
    Position pos;
    pos.x = 0.0;
    pos.y = 0.0;
    return pos;
}