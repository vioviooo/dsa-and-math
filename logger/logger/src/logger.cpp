#include "../include/logger.h"
#include <iomanip>
#include <sstream>

logger const *logger::trace(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::trace);
}

logger const *logger::debug(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::debug);
}

logger const *logger::information(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::information);
}

logger const *logger::warning(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::warning);
}

logger const *logger::error(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::error);
}

logger const *logger::critical(
    std::string const &message) const noexcept
{
    return log(message, logger::severity::critical);
}

std::string logger::severity_to_string(
    logger::severity severity)
{
    switch (severity)
    {
        case logger::severity::trace:
            return "TRACE";
        case logger::severity::debug:
            return "DEBUG";
        case logger::severity::information:
            return "INFORMATION";
        case logger::severity::warning:
            return "WARNING";
        case logger::severity::error:
            return "ERROR";
        case logger::severity::critical:
            return "CRITICAL";
    }

    throw std::out_of_range("Invalid severity value");
}

logger::severity logger::string_to_severity(
        std::string severity_string) 
{
    if (severity_string == "trace")
    {
        return logger::severity::trace;
    }
    if (severity_string == "debug")
    {
        return logger::severity::debug;
    }
    if (severity_string == "information")
    {
        return logger::severity::information;
    }
    if (severity_string == "warning")
    {
        return logger::severity::warning;
    }
    if (severity_string == "error")
    {
        return logger::severity::error;
    }
    if (severity_string == "critical")
    {
        return logger::severity::critical;
    }
    throw std::out_of_range("Invalid severity string value");
}


std::string logger::current_datetime_to_string() noexcept
{
    auto time = std::time(nullptr);

    std::ostringstream result_stream;
    result_stream << std::put_time(std::localtime(&time), "%d.%m.%Y %H:%M:%S");

    return result_stream.str();
}