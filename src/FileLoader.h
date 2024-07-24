#pragma once

#include <string>
#include <string_view>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include "Parameters.h"

class FileLoader 
{
public:
    static Parameters      loadFromTXT(const std::string& filePath);

private:
    static TargetFunction  getTargetFunction(const std::string_view str);
    static SelectionMethod getSelectionMethod(const std::string_view str);
    static Points          getPoints(const std::string_view str);
        
};
