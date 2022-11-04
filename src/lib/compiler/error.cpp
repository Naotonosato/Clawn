#include <iostream>
#include "include/compiler/error.hpp"

namespace clawn::compiler
{
std::string Error::dump() const
{
    std::cout << "\033[31mError: " << message.to_string() << std::endl
              << location->get_token_info().get_token_string() << "' in line "
              << location->get_token_info().get_line_number() << "\033[m"
              << std::endl;
    return message.to_string();
}

Error Error::void_assignment(const std::shared_ptr<location::Location> location)
{
    return Error(location,
                 resource::Path("Void typed value cannot be assigned."));
}
Error Error::invalid_call(const std::shared_ptr<location::Location> location)
{
    return Error(location,
                 resource::Path("Cannnot call unsolved value as function."));
}
Error Error::invalid_dereferencing(
    const std::shared_ptr<location::Location> location)
{
    return Error(location, resource::Path("Access expression is only available "
                                          "against reference but not value."));
}
Error Error::invalid_arguments_number(
    const std::shared_ptr<location::Location> location, size_t required,
    size_t passed)
{
    return Error(location, resource::Path("Function requires " +
                                          std::to_string(required) + " but " +
                                          std::to_string(passed) +
                                          " arguments were passed."));
}
Error Error::invalid_argument_type(
    const std::shared_ptr<location::Location> location,
    const std::string& required, const std::string& passed, size_t place)
{
    return Error(location,
                 resource::Path("Funcion requires argument typed " + required +
                                " but " + passed + " type value was passed."));
}
Error Error::unsafe_accessing(
    const std::shared_ptr<location::Location> location,
    const std::string& element_name)
{
    return Error(location, resource::Path("Accessing " + element_name +
                                          " in this scope is not permitted."));
}
Error Error::no_such_member(const std::shared_ptr<location::Location> location,
                            const std::string& element_name)
{
    return Error(location,
                 resource::Path("There is no member named " + element_name));
}
Error Error::never_handled(const std::shared_ptr<location::Location> location,
                           const std::string& tag_name)
{
    return Error(location, resource::Path("Matching case " + tag_name +
                                          " is never handled."));
}
Error Error::invalid_match_case(
    const std::shared_ptr<location::Location> location,
    const std::string& tag_name)
{
    return Error(location,
                 resource::Path("There is no such a tag named " + tag_name));
}
Error Error::invalid_match_target(
    const std::shared_ptr<location::Location> location, const std::string& type)
{
    return Error(
        location,
        resource::Path(
            "Match expression has to be attached to union type value but not " +
            type));
}
Error Error::invalid_condition_type(
    const std::shared_ptr<location::Location> location, const std::string& type)
{
    return Error(
        location,
        resource::Path("If condition has to be typed boolean, not " + type));
}
Error Error::types_not_compatible(
    const std::shared_ptr<location::Location> location,
    const std::string& type1, const std::string& type2)
{
    return Error(location, resource::Path(type1 + " and " + type2 +
                                          " are not compatible."));
}

Error Error::invalid_C_type_description(
    const std::shared_ptr<location::Location> location,
    const std::string& description)
{
    return Error(location, resource::Path("Unknown C type descripter '" +
                                          description + "'"));
}

Error Error::cannot_open_file(const std::string& filename)
{
    auto location = std::make_shared<location::Location>(
        "", location::TokenInfo(0, 0, ""), hierarchy::Hierarchy::get_root());
    return Error(location, resource::Path("Cannot open file: " + filename));
}
}  // namespace clawn::compiler