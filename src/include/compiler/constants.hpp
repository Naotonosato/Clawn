#pragma once
#include <string>

namespace clawn::compiler
{
namespace constants
{
const std::string OPERATOR_METHOD_PREFIX = "operator@";
const std::string OPERATOR_PLUS_NAME = "PLUS";
const std::string OPERATOR_MINUS_NAME = "MINUS";
const std::string OPERATOR_ASTERISK_NAME = "ASTERISK";
const std::string OPERATOR_SLASH_NAME = "SLASH";
const std::string OPERATOR_AND = "AND";
constexpr unsigned int VARIOUS_TAGS_LIMIT = 10;
}  // namespace constants
}  // namespace clawn::compiler