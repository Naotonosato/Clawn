namespace clawn::utils
{
// Class MoveConstructible forces the child class to be move-constructible.
// Usage:
// class Child: public MoveConstructible<Child>
// {}; //<- class 'Child' shoud be move-constructible.

template <typename T>
class MoveConstructible
{
    public:
    MoveConstructible()
    {
        static_assert(std::is_move_constructible<T>(),
                      "Class that inherits `MoveConstructible` should be "
                      "move-constructible");
    }
};
}  // namespace clawn::utils