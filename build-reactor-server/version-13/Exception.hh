/**
 * @file Exception.hh
 * @author yichenyan (2513626958@qq.com)
 * @brief
 * @version 1.0
 * @date 2024-01-01
 * @github https://github.com/yichenyiyan
 * @copyright Copyright (严亦辰) 2024
 *
 */

#pragma once


#include <iostream>
#include <stdexcept>
#include <string>

namespace yichen
{
    enum class ExceptionType : const int {
        INVALID = 0,
        INVALID_SOCKET = 1
    };

    class Exception : public std::runtime_error {
    public:
        explicit Exception(const std::string &message) : std::runtime_error(message), type_(ExceptionType::INVALID) {
            std::string exception_message = "Message :: " + message + "\n";
            std::cerr << exception_message;
        }

        Exception(ExceptionType type, const std::string &message) : std::runtime_error(message), type_(type) {
            std::string exception_message =
                "Exception Type :: " + ExceptionTypeToString(type_) + "\nMessage :: " + message + "\n";
            std::cerr << exception_message;
        }

        static std::string ExceptionTypeToString(ExceptionType type) {
            switch (type) {
            case ExceptionType::INVALID:
                return "Invalid";
            case ExceptionType::INVALID_SOCKET:
                return "Invalid socket";
            default:
                return "Unknoen";
            }
        }

    private:
        ExceptionType type_;
    };
}