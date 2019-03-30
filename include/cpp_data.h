#pragma once
#include "aux_aux.h"
#include <array>
#include <charconv>
#include <string_view>
#include <sstream>
#include <iomanip>
#include <future>
#include <variant>

namespace data
{
    #include "cpp_data_alignment.h"
    #include "cpp_data_inheritance.h"
    #include "cpp_data_inheritance_is_the.h"
    #include "cpp_data_initialization.h"

    // Michael Park - Enhanced Support for Value Semantics in C++17 [CppCon 2017]

    struct Cat   { /* ... */ };
    struct Dog   { /* ... */ };
    struct Horse { /* ... */ };

    using Animal = std::variant<Cat, Dog, Horse>;

    std::string get_sound(const Animal &animal) {
        struct GetSound {
            std::string operator()(const Cat   &) const { return "meow";  }
            std::string operator()(const Dog   &) const { return "woof";  }
            std::string operator()(const Horse &) const { return "neigh"; }
        };
        return std::visit(GetSound{}, animal);
    }

    TEST_ON
    {
        cout << get_sound(Cat  {}) << endl;
        cout << get_sound(Dog  {}) << endl;
        cout << get_sound(Horse{}) << endl;
    };

    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

    TEST_ON // https://en.cppreference.com/w/cpp/utility/variant/visit
    {
        using var_t = std::variant<int, long, double, std::string>;

        std::vector<var_t> vec = {10, 15l, 1.5, "hello"};

        for (auto& v: vec) {
            std::visit(overloaded {
                [](auto arg) { std::cout << arg << ' '; },
                [](double arg) { std::cout << std::fixed << arg << ' '; },
                [](const std::string& arg) { std::cout << std::quoted(arg) << ' '; },
            }, v);
        }
    };

    /*
    // Michael Park - Enhanced Support for Value Semantics in C++17 [CppCon 2017]

    // Pre C++17 way

    // Visitor Pattern (Ceremony)
    struct Cat; struct Dog; struct Horse;
    struct Animal {
    struct Vis {
    virtual void operator()(const Cat &) const = 0;
    virtual void operator()(const Dog &) const = 0;
    virtual void operator()(const Horse &) const = 0;
    };
    virtual ~Animal() = default;
    virtual void accept(const Vis &) const = 0;
    };
    struct Cat final : Animal {
    void accept(const Vis &vis) const override { vis(*this); }
    };
    struct Dog final : Animal {
    void accept(const Vis &vis) const override { vis(*this); }
    };
    struct Horse final : Animal {
    void accept(const Vis &vis) const override { vis(*this); }
    };
    // Visitor Pattern (Usage)
    string get_sound(const Animal &animal) {
    struct GetSound : Animal::Vis {
    void operator()(const Cat &) const override {
    result = "meow";
    }
    void operator()(const Dog &) const override {
    result = "woof";
    }
    void operator()(const Horse &) const override {
    result = "neigh";
    }
    string &result;
    };
    string result;
    animal.accept(GetSound{result});
    return result;
    }
    */

}