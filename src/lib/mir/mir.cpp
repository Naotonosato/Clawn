#include "include/mir/mir.hpp"
#include "include/requirement/type.hpp"

namespace clawn::mir
{
static std::map<MIRID, std::string> stringified;

std::string repeat_space(unsigned int count)
{
    std::string result = "";
    for (unsigned int i = 0; i < count; ++i)
    {
        result += " ";
    }
    return result;
}

MIRID MIRBase::get_id() const { return id; }

const std::string& MIRBase::get_name() const { return name; }

const Context& MIRBase::get_context() const { return *context; }

std::shared_ptr<requirement::Type> Integer::get_type() const
{
    return context->get_type_environment().get_integer_type();
}
std::shared_ptr<requirement::Type> RealNumber::get_type() const
{
    return context->get_type_environment().get_float_type();
}
std::shared_ptr<requirement::Type> StringPointer::get_type() const
{
    //throw std::runtime_error("not implemented or obsoluted.");
    return context->get_type_environment().get_integer_type()->get_pointer_to();
}

std::shared_ptr<requirement::Type> ComparePrimitive::get_type() const
{
    return values.first->get_type();
}
const std::pair<std::shared_ptr<Value>, std::shared_ptr<Value>>
ComparePrimitive::get_values() const
{
    return values;
}

std::shared_ptr<requirement::Type> Allocate::get_type() const
{
    return requirement::Type::create<requirement::ReferenceType>(
        type->get_type_solver(), type);
}

std::shared_ptr<requirement::Type> Allocate::get_type_to_allocate() const
{
    return type;
}

bool Allocate::is_global() const { return global; }

std::shared_ptr<requirement::Type> Load::get_type() const
{
    return address->get_type()
        ->as<requirement::ReferenceType>()
        .get_reference_to();
}

std::shared_ptr<Value> Load::get_address() const { return address; }

std::shared_ptr<requirement::Type> Store::get_type() const
{
    return value->get_type();
}

std::shared_ptr<Value> Store::get_address() const { return address; }
std::shared_ptr<Value> Store::get_value() const { return value; }

std::shared_ptr<requirement::Type> Cast::get_type() const { return cast_to; }
std::shared_ptr<Value> Cast::get_value() const { return value; }
std::shared_ptr<requirement::Type> Cast::get_cast_to() const { return cast_to; }

std::shared_ptr<requirement::Type> SizeOf::get_target_type() const
{
    return type;
}

std::shared_ptr<requirement::Type> SizeOf::get_type() const
{
    return context->get_type_environment().get_integer_type();
}

std::shared_ptr<requirement::Type> GetElementPointer::get_type() const
{
    assert(from->get_type()->is_type<requirement::ReferenceType>() &&
           "GetElementPointer should be attached to pointer.");

    return utils::Match{from->get_type()
                            ->as<requirement::ReferenceType>()
                            .get_reference_to()
                            ->get_variant()}(
        utils::Type<requirement::StructureType>(),
        [this](const requirement::StructureType& type) {
            return type.get_element_type(index)->get_pointer_to();
        },
        utils::Type<requirement::UnionType>(),
        [this](const requirement::UnionType& type) {
            return type.get_element_type(index)->get_pointer_to();
        },
        utils::Default(),
        [](const auto&) {
            assert(false &&
                   "GetElementPointer::from shold typed pointer to structure "
                   "or union.");
            return std::shared_ptr<requirement::Type>(nullptr);
        });
}

std::shared_ptr<Value> GetElementPointer::get_from() const { return from; }

unsigned int GetElementPointer::get_index() const { return index; }

std::shared_ptr<requirement::Type> FunctionCall::get_type() const
{
    return function->get_type()
        ->as<requirement::FunctionType>()
        .get_return_type();
}

std::shared_ptr<Value> FunctionCall::get_function() const { return function; }

const std::vector<std::shared_ptr<Value>>& FunctionCall::get_arguments() const
{
    return arguments;
}

Function::Function(std::shared_ptr<Context> context, const Scope& scope,
                   const std::string& name,
                   std::shared_ptr<requirement::Type> type)
    : ValueBase(context, scope, name),
      name(name),
      type(type),
      body(std::make_shared<std::vector<std::shared_ptr<BasicBlock>>>())
{
}

std::shared_ptr<requirement::Type> Function::get_type() const { return type; }

const std::string& Function::get_name() const { return name; }

const std::vector<std::shared_ptr<BasicBlock>>& Function::get_body() const
{
    return *body;
}

void Function::add_block(std::shared_ptr<BasicBlock> block)
{
    body->push_back(block);
}

std::shared_ptr<requirement::Type> Argument::get_type() const
{
    return function->get_type()
        ->as<requirement::FunctionType>()
        .get_argument_types()[index];
}

unsigned int Argument::get_index() const { return index; }

const Function& Argument::get_function() const { return *function; }

BasicBlock::BasicBlock(std::shared_ptr<Context> context,
                       std::shared_ptr<Function> owner, const std::string& name)
    : id(utils::get_unique_number()), owner(owner), name(name)
{
}

template <typename Base>
struct CreateHelper : Base
{
    template <typename... Args>
    CreateHelper(Args&&... args) : Base(std::forward<Args>(args)...)
    {
    }
};
std::shared_ptr<BasicBlock> BasicBlock::create(std::shared_ptr<Context> context,
                                               std::shared_ptr<Function> owner,
                                               const std::string& name)
{
    auto block =
        std::make_shared<CreateHelper<BasicBlock>>(context, owner, name);
    owner->add_block(block);
    return block;
}

unsigned long BasicBlock::get_id() const { return id; }

void BasicBlock::insert(const std::shared_ptr<MIR> mir) { mirs.push_back(mir); }

const std::string& BasicBlock::get_name() const { return name; }

std::shared_ptr<Function> BasicBlock::get_owner() const { return owner; }

const std::vector<std::shared_ptr<MIR>>& BasicBlock::get_mirs() const
{
    return mirs;
}

std::vector<std::shared_ptr<MIR>>& BasicBlock::get_mirs_mutable()
{
    return mirs;
}

std::shared_ptr<requirement::Type> Constant::get_type() const
{
    return std::visit([](const auto& value) { return value.get_type(); },
                      content);
}

MIRID Constant::get_id() const
{
    return std::visit([](const auto& value) { return value.get_id(); },
                      content);
}

std::string Constant::to_string(unsigned short indent) const
{
    return repeat_space(indent) +
           utils::Match{content}(
               utils::Type<Integer>(),
               [](const Integer& integer) {
                   return std::to_string(integer.get_value());
               },
               utils::Type<RealNumber>(),
               [](const RealNumber& real_number) {
                   return std::to_string(real_number.get_value());
               },
               utils::Type<StringPointer>(),
               [](const StringPointer& string) { return string.get_value(); },
               utils::Default(), [](const auto&) { return std::string(); });
}

std::string Constant::get_name() const
{
    return std::visit([](const auto& has_name) { return has_name.get_name(); },
                      content);
}

std::shared_ptr<requirement::Type> Value::get_type() const
{
    return std::visit([](const auto& value) { return value.get_type(); },
                      content);
}

MIRID Value::get_id() const
{
    return std::visit([](const auto& value) { return value.get_id(); },
                      content);
}

std::string Value::to_string(unsigned short indent) const
{
    std::string text = repeat_space(indent);
    if (is_type<Constant>())
    {
        text += as<Constant>().to_string();
        return text;
    }
    auto define_and_get_id_string = [&text,
                                     indent](std::shared_ptr<Value> value) {
        return value->to_string(indent);
        if (!stringified.count(value->get_id()))
        {
            // text += value->to_string() + "\n" + repeat_space(indent);
            stringified.insert(std::make_pair(value->get_id(), ""));
        }
        if (value->is_type<Constant>())
        {
            return value->as<Constant>().to_string();
        }
        return value->get_name() + "@" + std::to_string(value->get_id());
    };
    if (stringified.count(get_id()))
    {
        text += get_name() + "@" + std::to_string(get_id());
        return text;
    }
    text +=
        get_name() + "@" + std::to_string(get_id()) + " = " +
        utils::Match{content}(
            utils::Type<Constant>(),
            [define_and_get_id_string, indent](const Constant& constant) {
                return constant.to_string(indent);
            },
            utils::Type<ComparePrimitive>(),
            [define_and_get_id_string,
             indent](const ComparePrimitive& compare_primitive) {
                return "compare " +
                       define_and_get_id_string(
                           compare_primitive.get_values().first) +
                       ", " +
                       define_and_get_id_string(
                           compare_primitive.get_values().second);
            },
            utils::Type<Allocate>(),
            [define_and_get_id_string, indent](const Allocate& allocate) {
                return "allocate " +
                       allocate.get_type_to_allocate()->to_string();
            },
            utils::Type<Load>(),
            [define_and_get_id_string, indent](const Load& load) {
                return "load " + define_and_get_id_string(load.get_address());
            },
            utils::Type<Store>(),
            [define_and_get_id_string, indent](const Store& store) {
                return "store " + define_and_get_id_string(store.get_value()) +
                       " to " + define_and_get_id_string(store.get_address());
            },
            utils::Type<Cast>(),
            [define_and_get_id_string, indent](const Cast& cast) {
                return "cast " + define_and_get_id_string(cast.get_value()) +
                       " to " + cast.get_cast_to()->to_string();
            },
            utils::Type<SizeOf>(),
            [define_and_get_id_string, indent](const SizeOf& size_of) {
                return "sizeof " + size_of.get_target_type()->to_string();
            },
            utils::Type<GetElementPointer>(),
            [define_and_get_id_string,
             indent](const GetElementPointer& get_element_pointer) {
                return "get " +
                       std::to_string(get_element_pointer.get_index()) +
                       " of " +
                       define_and_get_id_string(get_element_pointer.get_from());
            },
            utils::Type<FunctionCall>(),
            [define_and_get_id_string, indent](const FunctionCall& call) {
                std::string arguments;
                for (auto& argument : call.get_arguments())
                {
                    arguments += define_and_get_id_string(argument) + ", ";
                }
                if (!arguments.empty())
                {
                    arguments.pop_back();
                    arguments.pop_back();
                }
                return "call " + define_and_get_id_string(call.get_function()) +
                       " with " + arguments;
            },
            utils::Type<Function>(),
            [](const Function& function) { return function.get_name(); },
            utils::Type<Argument>(),
            [](const Argument& argument) {
                return "argument" + std::to_string(argument.get_index());
            },
            utils::Default(), [](const auto&) { return std::string("?"); });
    stringified.insert(std::make_pair(get_id(), ""));
    return text;
}

std::string Value::get_name() const
{
    return std::visit([](const auto& has_name) { return has_name.get_name(); },
                      content);
}

std::shared_ptr<BasicBlock> Jump::get_destination() const
{
    return destination;
}

std::shared_ptr<BasicBlock> ConditionalJump::get_destination_on_true() const
{
    return destination_on_true;
}

std::shared_ptr<BasicBlock> ConditionalJump::get_destination_on_false() const
{
    return destination_on_false;
}

std::shared_ptr<Value> ConditionalJump::get_condition() const
{
    return condition;
}

std::shared_ptr<Value> Return::get_expression() const { return expression; }

void Context::register_mir(MIRID id, std::shared_ptr<MIR> mir)
{
    mir_map[id] = mir;
}

void Context::register_function(std::shared_ptr<Function> function)
{
    functions.insert(std::make_pair(function->get_name(), function));
}

std::shared_ptr<MIR> Context::get_mir(MIRID id) const { return mir_map.at(id); }

requirement::TypeEnvironment& Context::get_type_environment()
{
    return *type_environment;
}

std::map<std::string, std::shared_ptr<mir::Value>>&
Context::get_variable_addresses()
{
    return variable_addresses;
}

std::vector<std::shared_ptr<Function>> Context::get_functions() const
{
    std::vector<std::shared_ptr<Function>> result;
    for (auto& function_info : functions)
    {
        result.push_back(function_info.second);
    }
    return result;
}

MIRID MIR::get_id() const
{
    return std::visit([](const auto& mir) { return mir.get_id(); }, content);
}

std::string MIR::to_string(unsigned short indent) const
{
    return utils::Match{content}(
        utils::Type<Value>(),
        [indent](const Value& value) {
            // if (stringified.count(value.get_id())){return value.to_string();}
            // return "%" + std::to_string(value.get_id()) + " = " +
            // stringified[value.get_id()] = value.to_string();
            return value.to_string(indent);
        },
        utils::Type<Jump>(),
        [indent](const Jump& jump) {
            auto destination = jump.get_destination();
            return repeat_space(indent) + "jump to " +
                   std::to_string(destination->get_id()) + ": " +
                   destination->get_name();
        },
        utils::Type<ConditionalJump>(),
        [indent](const ConditionalJump& jump) {
            auto on_true = jump.get_destination_on_true();
            auto on_false = jump.get_destination_on_false();
            return repeat_space(indent) + "jump to " +
                   std::to_string(on_true->get_id()) + ": " +
                   on_true->get_name() + " if " +
                   jump.get_condition()->to_string() + "\n else " +
                   std::to_string(on_false->get_id()) + ": " +
                   on_false->get_name();
        },
        utils::Type<Return>(),
        [indent](const Return& returning) {
            return repeat_space(indent) + "return" +
                   returning.get_expression()->to_string();
        },
        utils::Default(), [](const auto&) { return std::string(); });
}

std::string MIR::get_name() const
{
    return utils::Match{content}(
        utils::Type<Value>(),
        [](const Value& value) { return value.get_name(); }, utils::Default(),
        [](const auto&) { return std::string(); });
}

template <typename T>
T insert(BasicBlock& block, T mir)
{
    block.insert(MIR::from(mir));
    return mir;
}

Builder::Builder(std::shared_ptr<Context> context, const Scope& global_scope,
                 const std::string& global_function_name)
    : context(context)
{
    auto& type_environment = context->get_type_environment();
    auto global_function_type =
        requirement::Type::create<requirement::FunctionType>(
            type_environment.get_solver(), global_function_name,
            std::vector<std::shared_ptr<requirement::Type>>{},
            std::vector<std::string>{}, type_environment.get_integer_type());
    auto global_function = MIR::create<Function, Function>(
        context, global_scope, global_function_name, global_function_type);
    context->register_function(global_function);
    current_block =
        mir::BasicBlock::create(context, global_function, "global:initialize");
}

std::shared_ptr<Value> Builder::create_compare_primitive(
    const Scope& scope, std::shared_ptr<Value> value,
    std::shared_ptr<Value> other, const std::string& name)
{
    return insert(*current_block,
                  MIR::create<ComparePrimitive>(
                      context, scope, std::make_pair(value, other), name));
}

std::shared_ptr<Value> Builder::create_allocate(
    const Scope& scope, std::shared_ptr<requirement::Type> type,
    const std::string& name)
{
    return insert(*current_block,
                  MIR::create<Allocate>(context, scope, type, false, name));
}

std::shared_ptr<Value> Builder::create_global_allocate(
    const Scope& scope, std::shared_ptr<requirement::Type> type,
    const std::string& name)
{
    return insert(*current_block,
                  MIR::create<Allocate>(context, scope, type, true, name));
}

std::shared_ptr<Value> Builder::create_load(const Scope& scope,
                                            std::shared_ptr<Value> address,
                                            const std::string& name)
{
    return insert(*current_block,
                  MIR::create<Load>(context, scope, address, name));
}

std::shared_ptr<Value> Builder::create_store(const Scope& scope,
                                             std::shared_ptr<Value> address,
                                             std::shared_ptr<Value> value,
                                             const std::string& name)
{
    return insert(*current_block,
                  MIR::create<Store>(context, scope, address, value, name));
}

std::shared_ptr<Value> Builder::create_cast(
    const Scope& scope, std::shared_ptr<Value> value,
    std::shared_ptr<requirement::Type> cast_to, const std::string& name)
{
    return insert(*current_block,
                  MIR::create<Cast>(context, scope, value, cast_to, name));
}

std::shared_ptr<Value> Builder::create_sizeof(
    const Scope& scope, std::shared_ptr<requirement::Type> type,
    const std::string& name)
{
    return insert(*current_block,
                  MIR::create<SizeOf>(context, scope, type, name));
}

std::shared_ptr<Function> Builder::create_function(
    const Scope& scope, const std::string& name,
    std::shared_ptr<requirement::Type> type)
{
    auto function = MIR::create<Function, Function>(context, scope, name, type);
    context->register_function(function);

    return function;
}

std::shared_ptr<Value> Builder::create_argument(
    const Scope& scope, std::shared_ptr<Function> function, unsigned int index,
    const std::string& name)
{
    return insert(*current_block,
                  MIR::create<Argument>(context, scope, function, index, name));
}

std::shared_ptr<Value> Builder::create_get_element_pointer(
    const Scope& scope, std::shared_ptr<Value> from, unsigned int index,
    const std::string& name)
{
    return insert(*current_block, MIR::create<GetElementPointer>(
                                      context, scope, from, index, name));
}

std::shared_ptr<Value> Builder::create_call(
    const Scope& scope, std::shared_ptr<Value> function,
    const std::vector<std::shared_ptr<Value>>& arguments,
    const std::string& name)
{
    return insert(
        *current_block,
        MIR::create<FunctionCall>(context, scope, function, arguments, name));
}

std::shared_ptr<MIR> Builder::create_jump(
    const Scope& scope, std::shared_ptr<BasicBlock> destination)
{
    return insert(*current_block,
                  MIR::create<Jump>(context, scope, destination));
}

std::shared_ptr<MIR> Builder::create_conditional_jump(
    const Scope& scope, std::shared_ptr<BasicBlock> on_true,
    std::shared_ptr<BasicBlock> on_false, std::shared_ptr<Value> condition)
{
    return insert(*current_block,
                  MIR::create<ConditionalJump>(context, scope, on_true,
                                               on_false, condition));
}

std::shared_ptr<MIR> Builder::create_return(const Scope& scope,
                                            std::shared_ptr<Value> expression)
{
    return insert(*current_block,
                  MIR::create<Return>(context, scope, expression));
}

std::shared_ptr<BasicBlock> Builder::get_current_block() const
{
    return current_block;
}

void Builder::set_current_block(std::shared_ptr<BasicBlock> new_block)
{
    current_block = new_block;
}

std::shared_ptr<Context> Builder::get_context() const { return context; }

}  // namespace clawn::mir