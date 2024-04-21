#include <not_implemented.h>

#include "../include/client_logger_builder.h"

#include "../include/client_logger.h"

#include "logger_builder.h"

#include <filesystem>

#include <fstream>

#include <sstream>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

client_logger_builder::client_logger_builder() {
    _format = "%d - %t - [%s] - %m"; // default format
}

client_logger_builder::client_logger_builder(std::string const& format_string) {
    _format = format_string;
}

logger_builder* client_logger_builder::add_file_stream(std::string const &stream_file_path, logger::severity severity) {
    std::__fs::filesystem::path path(stream_file_path);
    std::string file_name = path.filename().string();
    
    _config[file_name].insert(severity);

    return this;
}

logger_builder* client_logger_builder::add_console_stream(logger::severity severity) {
    _config["console"].insert(severity);
    
    return this;
}

logger_builder* client_logger_builder::transform_with_configuration(std::string const &configuration_file_path, std::string const &configuration_path) {
    std::ifstream fp(configuration_file_path);
    if (!fp.is_open()) {
        throw std::runtime_error{"File can not be opened."};
    }
    
    std::vector<std::string> path_component;
    std::istringstream ss(configuration_path);
    std::string component, line;

    while (std::getline(ss, component, '/')) {
        if (!component.empty()) {
            path_component.push_back(component);
        }
    }

    std::getline(fp, line);
    
    std::string currentPath;
    int cnt = 0;
    bool found = false, end_check = true;
    while (!found) {
        if (!std::getline(fp, line)) {
            break;
        }
        if (line.find(path_component[cnt]) != std::string::npos) {
            cnt++;
            if (cnt == path_component.size()) {
                found = true;
            }
        } else {
            size_t open_bracket = std::count(line.begin(), line.end(), '{');
            size_t close_bracket = std::count(line.begin(), line.end(), '}');
            while (open_bracket != close_bracket) {
                if (!std::getline(fp, line)) {
                    break;
                }
                open_bracket += std::count(line.begin(), line.end(), '{');
                close_bracket += std::count(line.begin(), line.end(), '}');
            }
        }
    }

    if (!found) {
        throw std::runtime_error{"Configuration path not found."};
    }

    std::vector <std::string> my_level;
    std::vector <std::string> my_output;
    
    while (std::getline(fp, line)) {
        if (line.find("level") != std::string::npos) {
            size_t pos = line.find(':');
            std::string tmp = line.substr(pos + 3, line.rfind('"'));
            std::string level = tmp.substr(0, tmp.find('"'));
            my_level.push_back(level);
        } else if (line.find("format") != std::string::npos) {
            size_t pos = line.find(':');
            std::string tmp = line.substr(pos + 3, line.rfind('"'));
            std::string format = tmp.substr(0, tmp.find('"'));
            _format = format;
        } else if (line.find("file_path") != std::string::npos) {
            size_t pos = line.find(':');
            std::string tmp = line.substr(pos + 3, line.rfind('"'));
            std::string output_path = tmp.substr(0, tmp.find('"'));
            my_output.push_back(output_path);
        } else {
            break; // presumably the end of the needed configuration
        }
    }

    for (int i = 0; i < my_output.size(); i++) {
        if (my_output[i] == "stdout") {
            for (int j = 0; j < my_level.size(); j++) {
                add_console_stream(string_to_severity(my_level[j]));
            }
        } else {
            for (int j = 0; j < my_level.size(); j++) {
                add_file_stream(my_output[i], string_to_severity(my_level[j]));
            }
        }
    }

    std::cout << my_level.size() << ' ' << my_output.size() << '\n';

    return this;
}

logger_builder* client_logger_builder::clear() {
    _format = "%d - %t - [%s] - %m"; // default format
    _config.clear();

    return this;
}

logger* client_logger_builder::build() const {
    return new client_logger(_format, _config);
}