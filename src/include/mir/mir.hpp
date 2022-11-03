#include <memory>
#include <variant>
#include <map>
#include <set>
#include <type_traits>
#include "include/hierarchy/hierarchy.hpp"
#include "include/utils/unique_number.hpp"
#include "include/utils/variant_wrapper.hpp"

namespace clawn
{
namespace requirement
{
class Type;
class TypeEnvironment;
}  // namespace requirement
namespace mir
{
using MIRID = size_t;
using Scope = hierarchy::HierarchyID;

// Constant ⊂ Value, Value,Jump,ConditionalJump ⊂ MIR
class MIR;
class Value;
class Constant;
class Context;

class MIRBase
{
    public:
    MIRBase(const MIRBase&) = default;
    MIRBase(MIRBase&&) = default;

    protected:
    MIRID id;
    std::shared_ptr<Context> context;
    Scope scope;
    std::string name;

    MIRBase(std::shared_ptr<Context> context, const Scope& scope,
            const std::string& name = "")
        : id(utils::get_unique_number()),
          context(context),
          scope(scope),
          name(name)
    {
    }

    public:
    using kind = MIR;
    MIRID get_id() const;
    const std::string& get_name() const;
    const Scope get_scope() const;
    const Context& get_context() const;
};

class ValueBase : public MIRBase
{
    protected:
    ValueBase(std::shared_ptr<Context> context, const Scope& scope,
              const std::string& name = "")
        : MIRBase(context, scope, name)
    {
    }

    public:
    using kind = Value;
    virtual std::shared_ptr<requirement::Type> get_type() const = 0;
};

template <typename ConstantValueType>
class ConstantBase : public ValueBase
{
    protected:
    ConstantValueType value;
    ConstantBase(std::shared_ptr<Context> context, const Scope& scope,
                 const ConstantValueType& value, const std::string& name = "")
        : ValueBase(context, scope, name), value(value)
    {
    }

    public:
    using kind = Constant;
    ConstantValueType get_value() const { return value; }
};

class Integer : public ConstantBase<long long>
{
    protected:
    Integer(std::shared_ptr<Context> context, const Scope& scope,
            long long value, const std::string& name = "")
        : ConstantBase(context, scope, value, name)
    {
    }

    public:
    std::shared_ptr<requirement::Type> get_type() const override;
};

class RealNumber : public ConstantBase<long double>
{
    protected:
    RealNumber(std::shared_ptr<Context> context, const Scope& scope,
               long double value, const std::string& name = "")
        : ConstantBase(context, scope, value, name)
    {
    }

    public:
    std::shared_ptr<requirement::Type> get_type() const override;
};

class StringPointer : public ConstantBase<std::string>
{
    std::string value;

    protected:
    StringPointer(std::shared_ptr<Context> context, const Scope& scope,
                  const std::string& value, const std::string& name = "")
        : ConstantBase(context, scope, value, name)
    {
    }

    public:
    std::shared_ptr<requirement::Type> get_type() const override;
};

class ComparePrimitive : public ValueBase
{
    std::pair<std::shared_ptr<Value>, std::shared_ptr<Value>> values;

    protected:
    ComparePrimitive(
        std::shared_ptr<Context> context, const Scope& scope,
        const std::pair<std::shared_ptr<Value>, std::shared_ptr<Value>>& values,
        const std::string& name = "")
        : ValueBase(context, scope, name), values(values)
    {
    }

    public:
    std::shared_ptr<requirement::Type> get_type() const override;
    const std::pair<std::shared_ptr<Value>, std::shared_ptr<Value>> get_values()
        const;
};

class Allocate : public ValueBase
{
    std::shared_ptr<requirement::Type> type;
    bool global;

    protected:
    Allocate(std::shared_ptr<Context> context, const Scope& scope,
             std::shared_ptr<requirement::Type> type, bool global = false,
             const std::string& name = "")
        : ValueBase(context, scope, name), type(type), global(global)
    {
    }

    public:
    std::shared_ptr<requirement::Type> get_type() const override;
    std::shared_ptr<requirement::Type> get_type_to_allocate() const;
    bool is_global() const;
};

class Load : public ValueBase
{
    const std::shared_ptr<Value> address;

    protected:
    Load(std::shared_ptr<Context> context, const Scope& scope,
         std::shared_ptr<Value> address, const std::string& name = "")
        : ValueBase(context, scope, name), address(address)
    {
    }

    public:
    std::shared_ptr<requirement::Type> get_type() const override;
    std::shared_ptr<Value> get_address() const;
};

class Store : public ValueBase
{
    std::shared_ptr<Value> address;
    std::shared_ptr<Value> value;

    protected:
    Store(std::shared_ptr<Context> context, const Scope& scope,
          std::shared_ptr<Value> address, std::shared_ptr<Value> value,
          const std::string& name = "")
        : ValueBase(context, scope, name), address(address), value(value)
    {
    }

    public:
    std::shared_ptr<requirement::Type> get_type() const override;
    std::shared_ptr<Value> get_address() const;
    std::shared_ptr<Value> get_value() const;
};

class Cast : public ValueBase
{
    std::shared_ptr<Value> value;
    std::shared_ptr<requirement::Type> cast_to;

    protected:
    Cast(std::shared_ptr<Context> context, const Scope& scope,
         std::shared_ptr<Value> value,
         std::shared_ptr<requirement::Type> cast_to,
         const std::string& name = "")
        : ValueBase(context, scope, name), value(value), cast_to(cast_to)
    {
    }

    public:
    std::shared_ptr<requirement::Type> get_type() const override;
    std::shared_ptr<Value> get_value() const;
    std::shared_ptr<requirement::Type> get_cast_to() const;
};

class SizeOf : public ValueBase
{
    std::shared_ptr<requirement::Type> type;

    protected:
    SizeOf(std::shared_ptr<Context> context, const Scope& scope,
           std::shared_ptr<requirement::Type> type,
           const std::string& name = "")
        : ValueBase(context, scope, name), type(type)
    {
    }

    public:
    std::shared_ptr<requirement::Type> get_target_type() const;
    std::shared_ptr<requirement::Type> get_type() const override;
};

class GetElementPointer : public ValueBase
{
    std::shared_ptr<Value> from;
    unsigned int index;

    protected:
    GetElementPointer(std::shared_ptr<Context> context, const Scope& scope,
                      std::shared_ptr<Value> from, unsigned int index,
                      const std::string& name = "")
        : ValueBase(context, scope, name), from(from), index(index)
    {
    }

    public:
    std::shared_ptr<requirement::Type> get_type() const override;
    std::shared_ptr<Value> get_from() const;
    unsigned int get_index() const;
};

class FunctionCall : public ValueBase
{
    std::shared_ptr<Value> function;
    std::vector<std::shared_ptr<Value>> arguments;

    protected:
    FunctionCall(std::shared_ptr<Context> context, const Scope& scope,
                 std::shared_ptr<Value> function,
                 const std::vector<std::shared_ptr<Value>>& arguments,
                 const std::string& name = "")
        : ValueBase(context, scope, name),
          function(function),
          arguments(arguments)
    {
    }

    public:
    std::shared_ptr<requirement::Type> get_type() const override;
    std::shared_ptr<Value> get_function() const;
    const std::vector<std::shared_ptr<Value>>& get_arguments() const;
};

class BasicBlock;
class Builder;

class Function : public ValueBase
{
    std::string name;
    std::shared_ptr<requirement::Type> type;
    std::shared_ptr<std::vector<std::shared_ptr<BasicBlock>>> body;

    protected:
    Function(std::shared_ptr<Context> context, const Scope& scope,
             const std::string& name, std::shared_ptr<requirement::Type> type);

    public:
    struct Analysis
    {
        std::set<mir::MIRID> allocations_may_be_used_after_function;
        std::set<mir::MIRID> callings_to_free_associated_memories;
        Analysis(
            const std::set<mir::MIRID>& allocations_may_be_used_after_function,
            const std::set<mir::MIRID>& callings_to_free_associated_memories)
            : allocations_may_be_used_after_function(
                  allocations_may_be_used_after_function),
              callings_to_free_associated_memories(
                  callings_to_free_associated_memories)
        {
        }
    };
    std::shared_ptr<requirement::Type> get_type() const override;
    const std::string& get_name() const;
    const std::vector<std::shared_ptr<BasicBlock>>& get_body() const;
    void add_block(std::shared_ptr<BasicBlock> block);
    Analysis analyze() const;
};

class Argument : public ValueBase
{
    std::shared_ptr<Function> function;
    unsigned int index;

    protected:
    Argument(std::shared_ptr<Context> context, const Scope& scope,
             std::shared_ptr<Function> function, const unsigned int index,
             const std::string& name = "")
        : ValueBase(context, scope, name), function(function), index(index)
    {
    }

    public:
    std::shared_ptr<requirement::Type> get_type() const override;
    const Function& get_function() const;
    unsigned int get_index() const;
};

class BasicBlock
{
    unsigned long id;
    std::string name;
    std::vector<std::shared_ptr<MIR>> mirs;
    std::shared_ptr<Function> owner;

    protected:
    BasicBlock(std::shared_ptr<Context> context,
               std::shared_ptr<Function> owner, const std::string& name);

    public:
    unsigned long get_id() const;
    void insert(std::shared_ptr<MIR> mir);
    std::shared_ptr<Function> get_owner() const;
    const std::string& get_name() const;
    const std::vector<std::shared_ptr<MIR>>& get_mirs() const;
    std::vector<std::shared_ptr<MIR>>& get_mirs_mutable();
    static std::shared_ptr<BasicBlock> create(std::shared_ptr<Context> context,
                                              std::shared_ptr<Function> owner,
                                              const std::string& name);
};

class Return : public MIRBase
{
    std::shared_ptr<Value> expression;

    protected:
    Return(std::shared_ptr<Context> context, const Scope& scope,
           std::shared_ptr<Value> expression)
        : MIRBase(context, scope), expression(expression)
    {
    }

    public:
    std::shared_ptr<Value> get_expression() const;
};

class Jump : public MIRBase
{
    std::shared_ptr<BasicBlock> destination;

    protected:
    Jump(std::shared_ptr<Context> context, const Scope& scope,
         std::shared_ptr<BasicBlock> destination)
        : MIRBase(context, scope), destination(destination)
    {
    }

    public:
    std::shared_ptr<BasicBlock> get_destination() const;
};

class ConditionalJump : public MIRBase
{
    std::shared_ptr<BasicBlock> destination_on_true;
    std::shared_ptr<BasicBlock> destination_on_false;
    std::shared_ptr<Value> condition;

    protected:
    ConditionalJump(std::shared_ptr<Context> context, const Scope& scope,
                    std::shared_ptr<BasicBlock> destination_on_true,
                    std::shared_ptr<BasicBlock> destination_on_false,
                    std::shared_ptr<Value> condition)
        : MIRBase(context, scope),
          destination_on_true(destination_on_true),
          destination_on_false(destination_on_false),
          condition(condition)
    {
    }

    public:
    std::shared_ptr<BasicBlock> get_destination_on_true() const;
    std::shared_ptr<BasicBlock> get_destination_on_false() const;
    std::shared_ptr<Value> get_condition() const;
};

class Context
{
    std::shared_ptr<requirement::TypeEnvironment> type_environment;
    std::map<std::string, std::shared_ptr<Function>> functions;
    std::map<MIRID, std::shared_ptr<MIR>> mir_map;
    std::map<std::string, std::shared_ptr<mir::Value>> variable_addresses;

    public:
    Context(std::shared_ptr<requirement::TypeEnvironment> type_environment)
        : type_environment(type_environment)
    {
    }
    void register_function(std::shared_ptr<Function> function);
    void register_mir(MIRID, std::shared_ptr<MIR> mir);
    std::shared_ptr<MIR> get_mir(MIRID) const;
    requirement::TypeEnvironment& get_type_environment();
    std::vector<std::shared_ptr<Function>> get_functions() const;
    std::map<std::string, std::shared_ptr<mir::Value>>&
    get_variable_addresses();
};

class Constant
    : public utils::VariantWrapper<Integer, RealNumber, StringPointer>
{
    using parent_type =
        utils::VariantWrapper<Integer, RealNumber, StringPointer>;

    public:
    template <typename T>
    Constant(const T& content) : parent_type(content)
    {
    }

    Constant(const Constant&) = default;
    std::shared_ptr<requirement::Type> get_type() const;
    MIRID get_id() const;
    std::string to_string(unsigned short indent = 0) const;
    std::string get_name() const;
};

class Value
    : public utils::VariantWrapper<Constant, ComparePrimitive, Allocate, Load,
                                   Store, Cast, SizeOf, GetElementPointer,
                                   FunctionCall, Function, Argument>
{
    using parent_type =
        utils::VariantWrapper<Constant, ComparePrimitive, Allocate, Load, Store,
                              Cast, SizeOf, GetElementPointer, FunctionCall,
                              Function, Argument>;

    public:
    Value(const Value&) = default;
    Value(const Constant& content) : parent_type(content) {}

    template <typename T>
    Value(const T& content) : parent_type(content)
    {
    }

    template <typename T>
    static std::shared_ptr<Value> from(std::shared_ptr<T> pointer)
    {
        return std::make_shared<Value>(*pointer);
    }

    std::shared_ptr<requirement::Type> get_type() const;
    MIRID get_id() const;
    std::string to_string(unsigned short indent = 0) const;
    std::string get_name() const;
};

class MIR : public utils::VariantWrapper<Value, Jump, ConditionalJump, Return>
{
    using parent_type =
        utils::VariantWrapper<Value, Jump, ConditionalJump, Return>;

    public:
    MIR() = delete;

    MIR(const MIR&) = default;

    MIR(const Jump& jump) : parent_type(jump) {}

    MIR(const ConditionalJump& jump) : parent_type(jump) {}

    MIR(const Return& return_) : parent_type(return_) {}

    MIR(const Constant& constant) : parent_type(constant.get_variant()) {}

    MIR(const Value& value) : parent_type(value.get_variant()) {}

    template <typename MIRType, typename... Args>
    static std::shared_ptr<typename MIRType::kind> create(
        std::shared_ptr<Context> context, Args&&... args)
    {
        auto mir = std::make_shared<CreateHelper<typename MIRType::kind>>(
            CreateHelper<MIRType>(context, std::forward<Args>(args)...));
        if constexpr (std::is_same<typename MIRType::kind, MIR>::value)
        {
            return mir;
        }
        else
        {
            context->register_mir(mir->get_id(),
                                  std::make_shared<MIR>(MIR(*mir)));
            return mir;
        }
    }

    template <typename MIRType, typename ContainerType, typename... Args>
    static std::shared_ptr<ContainerType> create(
        std::shared_ptr<Context> context, Args&&... args)
    {
        if constexpr (std::is_same<MIRType, ContainerType>::value)
        {
            auto mir = std::make_shared<CreateHelper<MIRType>>(
                context, std::forward<Args>(args)...);
            context->register_mir(mir->get_id(),
                                  std::make_shared<MIR>(MIR(Value(*mir))));
            return mir;
        }
        else
        {
            auto mir = std::make_shared<CreateHelper<ContainerType>>(
                CreateHelper<MIRType>(context, std::forward<Args>(args)...));
            context->register_mir(mir->get_id(),
                                  std::make_shared<MIR>(MIR(*mir)));
            return mir;
        }
    }
    MIRID get_id() const;
    const Scope get_scope() const;
    template <typename T>
    static std::shared_ptr<MIR> from(std::shared_ptr<T> pointer)
    {
        if constexpr (std::is_same<T, MIR>::value)
        {
            return pointer;
        }
        else
        {
            constexpr bool is_mir_container =
                std::is_same<T, Value>::value || std::is_same<T, Jump>::value ||
                std::is_same<T, ConditionalJump>::value;
            if constexpr (is_mir_container)
            {
                return std::make_shared<MIR>(*pointer);
            }
            else
            {
                return std::make_shared<MIR>(Value(*pointer));
            }
        }
    }

    std::string to_string(unsigned short indent = 0) const;
    std::string get_name() const;
};

class Builder
{
    std::shared_ptr<Context> context;
    std::shared_ptr<BasicBlock> current_block;

    public:
    Builder(std::shared_ptr<Context> context, const Scope& global_scope,
            const std::string& global_function_name = "main");

    std::shared_ptr<Value> create_compare_primitive(
        const Scope& scope, std::shared_ptr<Value> value,
        std::shared_ptr<Value> other, const std::string& name = "");

    std::shared_ptr<Value> create_allocate(
        const Scope& scope, std::shared_ptr<requirement::Type> type,
        const std::string& name = "");
    std::shared_ptr<Value> create_global_allocate(
        const Scope& scope, std::shared_ptr<requirement::Type> type,
        const std::string& name = "");
    std::shared_ptr<Value> create_load(const Scope& scope,
                                       std::shared_ptr<Value> address,
                                       const std::string& name = "");
    std::shared_ptr<Value> create_store(const Scope& scope,
                                        std::shared_ptr<Value> address,
                                        std::shared_ptr<Value> value,
                                        const std::string& name = "");
    std::shared_ptr<Value> create_cast(
        const Scope& scope, std::shared_ptr<Value> value,
        std::shared_ptr<requirement::Type> cast_to,
        const std::string& name = "");
    std::shared_ptr<Value> create_sizeof(
        const Scope& scope, std::shared_ptr<requirement::Type> type,
        const std::string& name = "");
    std::shared_ptr<Function> create_function(
        const Scope& scope, const std::string& name,
        std::shared_ptr<requirement::Type> type);
    std::shared_ptr<Value> create_argument(const Scope& scope,
                                           std::shared_ptr<Function> function,
                                           unsigned int index,
                                           const std::string& name = "");
    std::shared_ptr<Value> create_get_element_pointer(
        const Scope& scope, std::shared_ptr<Value> from, unsigned int index,
        const std::string& name = "");
    std::shared_ptr<Value> create_call(
        const Scope& scope, std::shared_ptr<Value> function,
        const std::vector<std::shared_ptr<Value>>& arguments,
        const std::string& name = "");
    std::shared_ptr<MIR> create_jump(const Scope& scope,
                                     std::shared_ptr<BasicBlock> destination);
    std::shared_ptr<MIR> create_conditional_jump(
        const Scope& scope, std::shared_ptr<BasicBlock> on_true,
        std::shared_ptr<BasicBlock> on_false, std::shared_ptr<Value> condition);
    std::shared_ptr<MIR> create_return(const Scope& scope,
                                       std::shared_ptr<Value> expression);
    std::shared_ptr<BasicBlock> get_current_block() const;
    void set_current_block(std::shared_ptr<BasicBlock> block);
    std::shared_ptr<Context> get_context() const;
};

template <class VisitorClass>
using MIRVisitor = utils::VisitorWrapper<VisitorClass, MIR>;
template <class VisitorClass>
using ValueVisitor = utils::VisitorWrapper<VisitorClass, Value>;
template <class VisitorClass>
using ConstantVisitor = utils::VisitorWrapper<VisitorClass, Constant>;
}  // namespace mir
}  // namespace clawn