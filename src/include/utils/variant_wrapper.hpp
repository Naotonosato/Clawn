#pragma once
#include <cassert>
#include <string>
#include <variant>
#include <type_traits>
#include <experimental/type_traits>
#include <iostream>

namespace clawn::utils
{
template <template <typename...> class C>
struct is_base_of_any_helper
{
    template <typename... T>
    std::true_type operator()(const C<T...>*) const;
    std::false_type operator()(...) const;
};

template <template <typename...> class C, typename T>
using is_base_of_any =
    decltype(is_base_of_any_helper<C>{}(std::declval<const T*>()));

template <class T, class Arg>
using const_visit_function_t =
    decltype(std::declval<const T&>().visit(std::declval<const Arg&>()));

template <typename T, typename Arg>
using has_const_visit_function =
    std::experimental::is_detected<const_visit_function_t, T, Arg>;

template <typename Type, typename... Rest>
struct first_type_of_variant
{
    using type = Type;
};

template <class VisitorClass, class VariantWrapperClass>
class VisitorWrapper;

template <typename... ElementTypes>
class VariantWrapper
{
    public:
    using variant_type = std::variant<ElementTypes...>;

    protected:
    variant_type content;

    template <typename Base>
    struct CreateHelper : Base
    {
        template <typename... Args>
        CreateHelper(Args&&... args) : Base(std::forward<Args>(args)...)
        {
        }
    };

    template <class T>
    VariantWrapper(const T& content) : content(content)
    {
    }

    template <class T>
    VariantWrapper(T&& content) : content(std::move(content))
    {
    }

    public:
    template <typename VisitorType>
    auto accept(const VisitorType& visitor) const
    {
        return std::visit(visitor, content);
    }

    template <class VisitorClass, typename T>
    auto accept(const VisitorWrapper<VisitorClass, T>& visitor) const
    {
        return std::visit(
            [this, &visitor](const auto& arg) {
                return static_cast<const VisitorClass*>(&visitor)->visit(arg);
            },
            content);
    }

    const variant_type& get_variant() const { return content; }

    variant_type& get_mutable_variant() { return content; }

    template <typename ContentType>
    bool is_type() const
    {
        return std::holds_alternative<ContentType>(content);
    }

    template <typename ContentType>
    const ContentType& as() const
    {
        assert(std::holds_alternative<ContentType>(content));
        return std::get<ContentType>(content);
    }

    template <typename ContentType>
    ContentType& as_mutable()
    {
        assert(std::holds_alternative<ContentType>(content));
        return std::get<ContentType>(content);
    }
};

// VisitorWrapper class forces derived class to implement visitor function takes
// each kind of arguments;
template <class VisitorClass, class VariantWrapperClass>
class VisitorWrapper
{
    private:
    template <typename Types>
    struct Verify;

    template <typename... Elements>
    struct Verify<std::variant<Elements...>>
    {
        template <typename Element, typename... Rest>
        constexpr static auto verify()
        {
            if constexpr (sizeof...(Rest) != 0)
            {
                static_assert(
                    has_const_visit_function<VisitorClass, Element>::value,
                    "Class that inherits from VisitorClass must "
                    "implement visit(arg) for each tag of variant.");
                return verify<Rest...>();
            }
            else
            {
                static_assert(
                    has_const_visit_function<VisitorClass, Element>::value,
                    "Class that inherits from VisitorClass must "
                    "implement visit(arg) for each tag of variant.");
                has_const_visit_function<VisitorClass, Element> result;
                return result;
            }
        }
        using verification = decltype(verify<Elements...>());
    };

    public:
    VisitorWrapper()
    {
        static_assert(
            is_base_of_any<VariantWrapper, VariantWrapperClass>::value,
            "The second parameter of template class VisitorWrapper must be "
            "inherited from VariantWrapper class.");
        using variant_type = typename VariantWrapperClass::variant_type;
        constexpr bool trigger [[maybe_unused]] =
            Verify<variant_type>::verification::value;
    }

    template <typename... TypeArguments>
    auto visit(const VariantWrapper<TypeArguments...>& variant_) const
    {
        auto& variant = variant_.get_variant();
        return std::visit(
            [this](const auto& arg) {
                static_cast<VisitorClass*>(this)->visit(arg);
            },
            variant);
    }

    template <typename ArgumentType>
    auto operator()(const ArgumentType& arg) const
    {
        return static_cast<const VisitorClass*>(this)->visit(arg);
    }
};

}  // namespace clawn::utils
