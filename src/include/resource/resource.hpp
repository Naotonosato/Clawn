#pragma once
#include <string>

namespace clawn::resource
{
class Path
{
    private:
    const std::string path;

    public:
    Path() = default;
    Path(const Path&) = default;
    Path(const std::string& path) : path(path) {}
    std::string to_string() const;
};
}  // namespace clawn::resource