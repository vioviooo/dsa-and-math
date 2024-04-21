#ifndef MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H
#define MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H

#include <logger.h>
#include "client_logger_builder.h"
#include <fstream>
#include <string>

class client_logger final:
    public logger
{

    friend class client_logger_builder;

private:
    static std::unordered_map<std::string, std::pair<std::ostream *, size_t>> _global_stream_mapping;

private:
    std::string _format;
    std::map<std::string, std::pair<std::ostream *, std::set<logger::severity>>> _local_stream_mapping;

private: 
    client_logger(std::string const &format, std::map<std::string, std::set<logger::severity>> const &config);

public:

    client_logger(
        client_logger const &other) = delete;

    client_logger &operator=(
        client_logger const &other) = delete;

    client_logger(
        client_logger &&other) = delete;

    client_logger &operator=(
        client_logger &&other) = delete;

    ~client_logger() noexcept final;

public:

    [[nodiscard]] logger const *log(
        const std::string &message,
        logger::severity severity) const noexcept override;

private:
    void reduce_user_stream(std::string const &file_path) noexcept;

};

#endif //MATH_PRACTICE_AND_OPERATING_SYSTEMS_CLIENT_LOGGER_H