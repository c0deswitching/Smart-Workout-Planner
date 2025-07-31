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
    // Enhanced JSON parser - handles nested arrays and objects
    std::string cleaned = jsonStr;
    
    // Remove whitespace and braces
    cleaned.erase(std::remove_if(cleaned.begin(), cleaned.end(), ::isspace), cleaned.end());
    if (cleaned.front() == '{') cleaned = cleaned.substr(1);
    if (cleaned.back() == '}') cleaned = cleaned.substr(0, cleaned.length() - 1);
    
    // Split by commas - but respect nested structures
    size_t pos = 0;
    int braceDepth = 0;
    int bracketDepth = 0;
    bool inQuotes = false;
    
    while (pos < cleaned.length()) {
        size_t nextComma = pos;
        
        // Find next comma that's not inside nested structures
        for (size_t i = pos; i < cleaned.length(); ++i) {
            char c = cleaned[i];
            
            if (c == '"' && (i == 0 || cleaned[i-1] != '\\')) {
                inQuotes = !inQuotes;
            } else if (!inQuotes) {
                if (c == '{') braceDepth++;
                else if (c == '}') braceDepth--;
                else if (c == '[') bracketDepth++;
                else if (c == ']') bracketDepth--;
                else if (c == ',' && braceDepth == 0 && bracketDepth == 0) {
                    nextComma = i;
                    break;
                }
            }
            
            // If we reached the end without finding a comma
            if (i == cleaned.length() - 1) {
                nextComma = cleaned.length();
                break;
            }
        }
        
        std::string pair = cleaned.substr(pos, nextComma - pos);
        size_t colonPos = pair.find(':');
        
        if (colonPos != std::string::npos) {
            std::string key = pair.substr(0, colonPos);
            std::string value = pair.substr(colonPos + 1);
            
            // Remove quotes from key
            if (key.front() == '"' && key.back() == '"') {
                key = key.substr(1, key.length() - 2);
            }
            
            // For values, keep the original structure (arrays, objects, etc.)
            // Only remove quotes if it's a simple string value
            if (value.front() == '"' && value.back() == '"' && 
                value.find('[') == std::string::npos && 
                value.find('{') == std::string::npos) {
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

std::vector<std::string> SimpleJson::getStringArray(const std::string& key, const std::vector<std::string>& defaultValue) const {
    auto it = data_.find(key);
    if (it != data_.end()) {
        return parseArray(it->second);
    }
    return defaultValue;
}

std::unordered_map<std::string, std::string> SimpleJson::getObject(const std::string& key, const std::unordered_map<std::string, std::string>& defaultValue) const {
    auto it = data_.find(key);
    if (it != data_.end()) {
        return parseObject(it->second);
    }
    return defaultValue;
}

void SimpleJson::setStringArray(const std::string& key, const std::vector<std::string>& value) {
    std::ostringstream stream;
    stream << "[";
    for (size_t i = 0; i < value.size(); ++i) {
        if (i > 0) stream << ",";
        stream << "\"" << escapeString(value[i]) << "\"";
    }
    stream << "]";
    data_[key] = stream.str();
}

std::vector<std::string> SimpleJson::parseArray(const std::string& arrayStr) const {
    std::vector<std::string> result;
    
    if (arrayStr.empty() || arrayStr.front() != '[' || arrayStr.back() != ']') {
        return result;
    }
    
    std::string content = arrayStr.substr(1, arrayStr.length() - 2);
    
    // Simple array parsing - split by commas not inside quotes
    size_t pos = 0;
    bool inQuotes = false;
    size_t start = 0;
    
    for (size_t i = 0; i < content.length(); ++i) {
        if (content[i] == '"' && (i == 0 || content[i-1] != '\\')) {
            inQuotes = !inQuotes;
        } else if (content[i] == ',' && !inQuotes) {
            std::string item = content.substr(start, i - start);
            // Remove whitespace and quotes
            item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
            if (item.front() == '"' && item.back() == '"') {
                item = item.substr(1, item.length() - 2);
            }
            result.push_back(unescapeString(item));
            start = i + 1;
        }
    }
    
    // Handle last item
    if (start < content.length()) {
        std::string item = content.substr(start);
        item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
        if (item.front() == '"' && item.back() == '"') {
            item = item.substr(1, item.length() - 2);
        }
        result.push_back(unescapeString(item));
    }
    
    return result;
}

std::unordered_map<std::string, std::string> SimpleJson::parseObject(const std::string& objectStr) const {
    std::unordered_map<std::string, std::string> result;
    
    if (objectStr.empty() || objectStr.front() != '{' || objectStr.back() != '}') {
        return result;
    }
    
    std::string content = objectStr.substr(1, objectStr.length() - 2);
    
    // Parse key-value pairs
    size_t pos = 0;
    while (pos < content.length()) {
        size_t nextComma = content.find(',', pos);
        if (nextComma == std::string::npos) nextComma = content.length();
        
        std::string pair = content.substr(pos, nextComma - pos);
        size_t colonPos = pair.find(':');
        
        if (colonPos != std::string::npos) {
            std::string key = pair.substr(0, colonPos);
            std::string value = pair.substr(colonPos + 1);
            
            // Remove whitespace and quotes
            key.erase(std::remove_if(key.begin(), key.end(), ::isspace), key.end());
            value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
            
            if (key.front() == '"' && key.back() == '"') {
                key = key.substr(1, key.length() - 2);
            }
            if (value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.length() - 2);
            }
            
            result[unescapeString(key)] = unescapeString(value);
        }
        
        pos = nextComma + 1;
    }
    
    return result;
}

}