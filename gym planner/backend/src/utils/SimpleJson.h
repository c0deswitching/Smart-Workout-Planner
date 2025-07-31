#pragma once

#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>

namespace json {

class SimpleJson {
public:
    SimpleJson() = default;
    explicit SimpleJson(const std::string& jsonStr);
    
    // Parse JSON string
    static SimpleJson parse(const std::string& jsonStr);
    
    // Convert to JSON string
    std::string toString() const;
    
    // Getters
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    double getDouble(const std::string& key, double defaultValue = 0.0) const;
    int getInt(const std::string& key, int defaultValue = 0) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;
    
    // Array getters
    std::vector<std::string> getStringArray(const std::string& key, const std::vector<std::string>& defaultValue = {}) const;
    std::unordered_map<std::string, std::string> getObject(const std::string& key, const std::unordered_map<std::string, std::string>& defaultValue = {}) const;
    
    // Setters
    void setString(const std::string& key, const std::string& value);
    void setDouble(const std::string& key, double value);
    void setInt(const std::string& key, int value);
    void setBool(const std::string& key, bool value);
    void setObject(const std::string& key, const SimpleJson& value);
    void setStringArray(const std::string& key, const std::vector<std::string>& value);
    
    // Check if key exists
    bool has(const std::string& key) const;
    
private:
    std::unordered_map<std::string, std::string> data_;
    
    std::string escapeString(const std::string& str) const;
    std::string unescapeString(const std::string& str) const;
    void parseKeyValue(const std::string& jsonStr);
    std::vector<std::string> parseArray(const std::string& arrayStr) const;
    std::unordered_map<std::string, std::string> parseObject(const std::string& objectStr) const;
};

}