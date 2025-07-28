#include "SimpleJson.h"
#include <algorithm>
#include <iostream>

namespace json {

SimpleJson::SimpleJson(const std::string& jsonStr) {
    parseKeyValue(jsonStr);
}

SimpleJson SimpleJson::parse(const std::string& jsonStr) {
    return SimpleJson(jsonStr);
}

void SimpleJson::parseKeyValue(const std::string& jsonStr) {
    // Simple JSON parser - handles basic key-value pairs
    std::string cleaned = jsonStr;
    
    // Remove whitespace and braces
    cleaned.erase(std::remove_if(cleaned.begin(), cleaned.end(), ::isspace), cleaned.end());
    if (cleaned.front() == '{') cleaned = cleaned.substr(1);
    if (cleaned.back() == '}') cleaned = cleaned.substr(0, cleaned.length() - 1);
    
    // Split by commas
    size_t pos = 0;
    while (pos < cleaned.length()) {
        size_t nextComma = cleaned.find(',', pos);
        if (nextComma == std::string::npos) nextComma = cleaned.length();
        
        std::string pair = cleaned.substr(pos, nextComma - pos);
        size_t colonPos = pair.find(':');
        
        if (colonPos != std::string::npos) {
            std::string key = pair.substr(0, colonPos);
            std::string value = pair.substr(colonPos + 1);
            
            // Remove quotes from key
            if (key.front() == '"' && key.back() == '"') {
                key = key.substr(1, key.length() - 2);
            }
            
            // Remove quotes from value if it's a string
            if (value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.length() - 2);
            }
            
            data_[key] = value;
        }
        
        pos = nextComma + 1;
    }
}

std::string SimpleJson::toString() const {
    std::ostringstream stream;
    stream << "{";
    
    bool first = true;
    for (const auto& pair : data_) {
        if (!first) stream << ",";
        first = false;
        
        stream << "\"" << escapeString(pair.first) << "\":";
        
        // Try to determine if value is a number or boolean
        std::string value = pair.second;
        bool isNumber = !value.empty() && (std::isdigit(value[0]) || value[0] == '-' || value[0] == '.');
        bool isBoolean = (value == "true" || value == "false");
        
        if (isNumber || isBoolean) {
            stream << value;
        } else {
            stream << "\"" << escapeString(value) << "\"";
        }
    }
    
    stream << "}";
    return stream.str();
}

std::string SimpleJson::getString(const std::string& key, const std::string& defaultValue) const {
    auto it = data_.find(key);
    return (it != data_.end()) ? unescapeString(it->second) : defaultValue;
}

double SimpleJson::getDouble(const std::string& key, double defaultValue) const {
    auto it = data_.find(key);
    if (it != data_.end()) {
        try {
            return std::stod(it->second);
        } catch (...) {
            return defaultValue;
        }
    }
    return defaultValue;
}

int SimpleJson::getInt(const std::string& key, int defaultValue) const {
    auto it = data_.find(key);
    if (it != data_.end()) {
        try {
            return std::stoi(it->second);
        } catch (...) {
            return defaultValue;
        }
    }
    return defaultValue;
}

bool SimpleJson::getBool(const std::string& key, bool defaultValue) const {
    auto it = data_.find(key);
    if (it != data_.end()) {
        return it->second == "true";
    }
    return defaultValue;
}

void SimpleJson::setString(const std::string& key, const std::string& value) {
    data_[key] = escapeString(value);
}

void SimpleJson::setDouble(const std::string& key, double value) {
    data_[key] = std::to_string(value);
}

void SimpleJson::setInt(const std::string& key, int value) {
    data_[key] = std::to_string(value);
}

void SimpleJson::setBool(const std::string& key, bool value) {
    data_[key] = value ? "true" : "false";
}

void SimpleJson::setObject(const std::string& key, const SimpleJson& value) {
    data_[key] = value.toString();
}

bool SimpleJson::has(const std::string& key) const {
    return data_.find(key) != data_.end();
}

std::string SimpleJson::escapeString(const std::string& str) const {
    std::string escaped = str;
    // Basic escaping - replace quotes
    size_t pos = 0;
    while ((pos = escaped.find('"', pos)) != std::string::npos) {
        escaped.replace(pos, 1, "\\\"");
        pos += 2;
    }
    return escaped;
}

std::string SimpleJson::unescapeString(const std::string& str) const {
    std::string unescaped = str;
    // Basic unescaping - replace escaped quotes
    size_t pos = 0;
    while ((pos = unescaped.find("\\\"", pos)) != std::string::npos) {
        unescaped.replace(pos, 2, "\"");
        pos += 1;
    }
    return unescaped;
}

}