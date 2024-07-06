#pragma once

#include <string>

struct FileSearchFilter
{
    std::string name, pattern;
};

void OpenFilePath(std::string& text, const FileSearchFilter& filter = FileSearchFilter());
void SaveFilePath(std::string& text, const FileSearchFilter& filter = FileSearchFilter());
