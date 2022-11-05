#include "IO.hpp"

uint32_t FileOpen::arity()
{
    return 2;
}
std::any FileOpen::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto file_path = arguments[0], mode = arguments[1];
    std::string file_path_str = std::any_cast<std::string>(file_path);
    std::string mode_str = std::any_cast<std::string>(mode);
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
std::string FileOpen::SurpherCallableToString()
{
    return {};
}

uint32_t Write::arity()
{
    return 2;
}

std::any Write::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    if (arguments[0].type() == typeid(std::shared_ptr<std::fstream>) && arguments[1].type() == typeid(std::string))
    {
        auto any_file_ptr = arguments[0], any_data = arguments[1];
        auto file_ptr = std::any_cast<std::shared_ptr<std::fstream>>(any_file_ptr);
        auto data = std::any_cast<std::string>(any_data);

        *file_ptr << data;
    }
    else
    {
        throw RuntimeError(paren, "Invalid usage of \"fileWrite\". Usage: fileWrite(file, data).");
    }

    return {};
}

std::string Write::SurpherCallableToString()
{
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

std::string ReadAll::SurpherCallableToString()
{
    return {};
}

uint32_t ReadSome::arity()
{
    return 2;
}

std::any ReadSome::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    if (arguments[0].type() == typeid(std::shared_ptr<std::fstream>) && arguments[1].type() == typeid(double))
    {
        auto any_file_ptr = arguments[0];
        auto file_ptr = std::any_cast<std::shared_ptr<std::fstream>>(any_file_ptr);

        auto any_size = arguments[1];
        auto size = std::any_cast<double>(any_size);
        auto size_int = static_cast<int64_t>(size);

        char buffer[size_int];
        file_ptr->read(buffer, size_int);

        return std::string{buffer};
    }
    throw RuntimeError(paren, "Invalid usage of \"fileReadSome\". Usage: fileReadSome(<file path>, size).");
}

std::string ReadSome::SurpherCallableToString()
{
    return {};
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
    throw RuntimeError(paren, "Invalid usage of \"fileClose\". Usage: fileClose<file path>).");
}

std::string FileClose::SurpherCallableToString()
{
    return {};
}
