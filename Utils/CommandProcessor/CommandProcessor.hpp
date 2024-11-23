#pragma once

#include <variant>
#include <string>
#include <system_error>
#include "../Logger.hpp"
#include "../ErrorCodeDefinition.hpp"

namespace Utils
{

namespace CommandProcessor
{

template <class...Args>
class Processor
{
public:
    using Type = std::variant<Args...>;
                                 
    Processor();

    template<typename T>
    std::string execute(const std::string& str, T& conn_obj);

private:
    Type m_commands[std::variant_size_v<Type>];
};

//===============================================================
template<class ...Args>
inline bool check_command(const std::string& str, const std::variant<Args...>& obj)
{
    bool ok = 
        std::visit(
            [&str](const auto& obj)
            {
                return obj.is_mycommand(str);
            },
            obj
        );
    return ok;
}

//===============================================================
template<typename T, class ...Args>
inline std::string execute_impl(const std::string& str, T& conn_obj, std::variant<Args...>& obj)
{
    return
            std::visit(
                [&str, &conn_obj](auto& obj)
                {
                    return obj.execute(str, conn_obj);
                },
                obj
            );

}

//===============================================================
template<int I, typename T, class...Args>
void init_item(std::variant<Args...>* array)
{
    array[I] = T();
}

//===============================================================
template<int I,int N, class...Args>
void init_impl(std::variant<Args...>* array)
{
    init_item<I,std::variant_alternative_t<I,std::variant<Args...>>>(array);
    if constexpr (I < N)
    {
        init_impl<I+1, N>(array);
    }
}

//===============================================================
template <class...Args>
Processor<Args...>::Processor()
{
    init_impl<0,std::variant_size_v<Type> - 1>(m_commands);
}

//===============================================================
template <class...Args>
template<typename T>
std::string Processor<Args...>::execute(const std::string& str, T& conn_obj)
{
    Utils::Logger::singleton()("command processor: start execute command :", str);

    auto it = std::find_if(m_commands + 0, m_commands + std::variant_size_v<Type>, 
        [&str](const Type& obj)
        {
            bool ok = check_command(str, obj);
            return ok;
        } );
    std::string responce;
    if (it != m_commands + std::variant_size_v<Type>)
    {
        responce = execute_impl(str, conn_obj, *it);
    }
    else
    {
        Utils::Logger::singleton()("command processor: unrecorgnized command :", str);
        responce = SocketApp::to_responce_string(std::make_error_code(SocketApp::AppErrors::UnrecognizedCommand));
    }
    return responce;
}

}

}
