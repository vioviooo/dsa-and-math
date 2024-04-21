#include <not_implemented.h>

#include <client_logger.h>
#include <client_logger_builder.h>

#include <string>
#include <fstream>

std::unordered_map<std::string, std::pair<std::ostream *, size_t>> client_logger::_global_stream_mapping;

void client_logger::reduce_user_stream(const std::string& file_name) noexcept 
{
    if (file_name == "console") {
        return;
    }

    auto& stream_user_count = _global_stream_mapping[file_name].second;
    if (--stream_user_count == 0) {
        delete _global_stream_mapping[file_name].first;
        _global_stream_mapping.erase(file_name);
    }
}

client_logger::client_logger(std::string const &format, std::map<std::string, std::set<logger::severity>> const &config) : _format{format}
{
    for (auto const &[path, severities] : config) {
        if (path == "console") {
            _local_stream_mapping[path] = { &std::cout, severities };
            continue;
        }

        std::ofstream *stream = nullptr;
        try {
            stream = new std::ofstream{path};
            if (!stream->is_open()) {
                throw std::runtime_error{"File can not be opened."};
            }
        } catch (...) {
            for (auto it = config.begin(); it != std::prev(it); ++it) {
                reduce_user_stream(it->first);
            }
            throw;
        }

        _global_stream_mapping[path] = {stream, 0};
        _local_stream_mapping[path] = {stream, severities};
        _global_stream_mapping[path].second++;
    }
}

client_logger::~client_logger() noexcept 
{
    for (auto it = _local_stream_mapping.begin(); it != _local_stream_mapping.end(); it++) {
        reduce_user_stream(it->first);
    }
}

logger const *client_logger::log(
    const std::string &text,
    logger::severity severity) const noexcept 
{    
    for (auto elem : _local_stream_mapping) {
        if (elem.second.second.find(severity) != elem.second.second.end()) { // => present
            std::ostream &stream = *(elem.second.first);
            for (int i = 0; i < _format.size(); i++) {
                if (_format[i] == '%') {
                    if (_format[i + 1] == 'm') {
                        stream << text;
                    } else if (_format[i + 1] == 's') {
                        stream << logger::severity_to_string(severity);
                    } else if (_format[i + 1] == 't') { 
                        std::string time = logger::current_datetime_to_string();
                        time = time.substr(11, 18);
                        stream << time;
                    } else if (_format[i + 1] == 'd') {
                        std::string date = logger::current_datetime_to_string();
                        date = date.substr(0, 10);
                        stream << date;
                    } else {
                        stream << _format[i] << _format[i + 1];
                    }
                    i++;
                } else {
                    stream << _format[i];
                }
            }
            stream << '\n';
        }
    }
    return this;
}