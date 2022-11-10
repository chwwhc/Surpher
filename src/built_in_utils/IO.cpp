#include <iostream>
#include <algorithm>

#include "IO.hpp"

uint32_t FileOpen::arity()
{
    return 2;
}
std::any FileOpen::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto file_path = arguments[0], mode = arguments[1];
    std::string file_path_str = std::any_cast<std::string &>(file_path);
    std::string mode_str = std::any_cast<std::string &>(mode);
    std::shared_ptr<std::fstream> f = std::make_shared<std::fstream>();
    if (mode_str == "r")
    {
        f->open(file_path_str, std::ios::binary | std::ios::in);
    }
    else if (mode_str == "w")
    {
        f->open(file_path_str, std::ios::binary | std::ios::out);
    }
    else if (mode_str == "rw")
    {
        f->open(file_path_str, std::ios::binary | std::ios::in | std::ios::out);
    }
    else
    {
        throw RuntimeError(paren, "Invalid usage of \"fileOpen\". Usage: fileOpen(<file path>, <mode: \"r\" or \"w\" or \"rw\">).");
    }

    return f;
}

uint32_t Write::arity()
{
    return 2;
}

std::any Write::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    if (arguments[0].type() == typeid(std::shared_ptr<std::fstream>) && (arguments[1].type() == typeid(std::string) || arguments[1].type() == typeid(SurpherArrayPtr)))
    {
        auto any_file_ptr = arguments[0], any_data = arguments[1];
        auto file_ptr = std::any_cast<std::shared_ptr<std::fstream>>(any_file_ptr);

        if (any_data.type() == typeid(std::string))
        {
            *file_ptr << std::any_cast<std::string &>(any_data);
        }
        else
        {
            auto arg_arr = *std::any_cast<SurpherArrayPtr>(any_data);
            if (std::find_if(arg_arr.begin(), arg_arr.end(), [](const std::any &a)
                             { return a.type() != typeid(long double); }) == arg_arr.end())
            {
                char buffer[arg_arr.size()];
                for (size_t i = 0; i < arg_arr.size(); i++)
                {
                    buffer[i] = static_cast<char>(std::any_cast<long double &>(arg_arr[i]));
                }
                file_ptr->write(buffer, arg_arr.size());
            }
        }
    }
    else
    {
        throw RuntimeError(paren, "Invalid usage of \"fileWrite\". Usage: fileWrite(file, data).");
    }

    return {};
}

uint32_t ReadAll::arity()
{
    return 1;
}

std::any ReadAll::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    if (arguments[0].type() == typeid(std::shared_ptr<std::fstream>))
    {
        auto any_file_ptr = arguments[0];
        auto file_ptr = std::any_cast<std::shared_ptr<std::fstream>>(any_file_ptr);

        std::ostringstream out_buf;
        out_buf << file_ptr->rdbuf();

        return out_buf.str();
    }
    throw RuntimeError(paren, "Invalid usage of \"fileReadAll\". Usage: fileReadAll(<file path>).");
}

uint32_t ReadSome::arity()
{
    return 2;
}

std::any ReadSome::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    if (arguments[0].type() == typeid(std::shared_ptr<std::fstream>) && arguments[1].type() == typeid(long double))
    {
        auto any_file_ptr = arguments[0];
        auto file_ptr = std::any_cast<std::shared_ptr<std::fstream>>(any_file_ptr);

        auto any_size = arguments[1];
        auto size = std::any_cast<long double &>(any_size);
        auto size_int = static_cast<int64_t>(size);

        char buffer[size_int];
        file_ptr->read(buffer, size_int);

        return std::string{buffer};
    }
    throw RuntimeError(paren, "Invalid usage of \"fileReadSome\". Usage: fileReadSome(<file path>, size).");
}

uint32_t FileClose::arity()
{
    return 1;
}

std::any FileClose::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    if (arguments[0].type() == typeid(std::shared_ptr<std::fstream>))
    {
        auto any_file_ptr = arguments[0];
        auto file_ptr = std::any_cast<std::shared_ptr<std::fstream>>(any_file_ptr);

        file_ptr->close();

        return {};
    }
    throw RuntimeError(paren, "Invalid usage of \"fileClose\". Usage: fileClose(<file path>).");
}

uint32_t Input::arity()
{
    return 1;
}

std::any Input::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    if (arguments[0].type() == typeid(std::string))
    {
        auto any_message = arguments[0];
        auto message = std::any_cast<std::string &>(any_message);
        std::string ret;

        std::cout << message;
        std::cin >> ret;

        return ret;
    }
    throw RuntimeError(paren, "Invalid usage of \"input\". Usage: input(<message>).");
}