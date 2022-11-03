#pragma once
#include <optional>
#include "include/location/location.hpp"
#include "include/resource/resource.hpp"

namespace clawn::compiler
{
// Error class defines structure for error messages in compilation.
// This class can be nested. This can be used for trace where error caused.
class Error
{
    private:
    std::shared_ptr<location::Location> location;
    std::optional<std::shared_ptr<Error>> child;
    resource::Path message;

    public:
    Error(const Error&) = default;
    Error() = delete;
    Error(const std::shared_ptr<location::Location> location,
          const resource::Path& error_message)
        : location(location), child(std::nullopt), message(error_message)
    {
    }
    Error(const std::shared_ptr<location::Location> location,
          const std::shared_ptr<Error> child,
          const resource::Path& error_message)
        : location(location), child(child), message(error_message)
    {
    }
    std::string dump() const;
    static Error void_assignment(
        const std::shared_ptr<location::Location> location);
    static Error invalid_call(
        const std::shared_ptr<location::Location> location);
    static Error invalid_dereferencing(
        const std::shared_ptr<location::Location> location);
    static Error invalid_arguments_number(
        const std::shared_ptr<location::Location> location, size_t required,
        size_t passed);
    static Error invalid_argument_type(
        const std::shared_ptr<location::Location> location,
        const std::string& required, const std::string& passed, size_t place);
    static Error unsafe_accessing(
        const std::shared_ptr<location::Location> location,
        const std::string& element_name);
    static Error no_such_member(
        const std::shared_ptr<location::Location> location,
        const std::string& element_name);
    static Error never_handled(
        const std::shared_ptr<location::Location> location,
        const std::string& tag_name);
    static Error invalid_match_case(
        const std::shared_ptr<location::Location> location,
        const std::string& tag_name);
    static Error invalid_match_target(
        const std::shared_ptr<location::Location> location,
        const std::string& type);
    static Error invalid_condition_type(
        const std::shared_ptr<location::Location> location,
        const std::string& type);
    static Error types_not_compatible(
        const std::shared_ptr<location::Location> location,
        const std::string& type1, const std::string& type2);

    static Error invalid_C_type_description(
        const std::shared_ptr<location::Location> location,
        const std::string& description);

    static Error cannot_open_file(const std::string& filename);
};
}  // namespace clawn::compiler