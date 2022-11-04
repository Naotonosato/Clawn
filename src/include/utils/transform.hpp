#include <vector>
#include <type_traits>
#include <algorithm>

namespace clawn::utils
{
template <typename Input, typename Functor,
          typename Output = typename std::result_of<Functor(Input)>::type>
std::vector<Output> get_transformed(const std::vector<Input>& inputs,
                                    Functor func)
{
    std::vector<Output> outputs;
    std::transform(inputs.cbegin(), inputs.cend(), std::back_inserter(outputs),
                   func);
    return outputs;
}
}  // namespace clawn::utils