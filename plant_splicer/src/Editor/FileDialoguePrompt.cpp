#define PFD_IMPLEMENTATION

#include <PortableFileDialogues.h>
#include "FileDialoguePrompt.h"

void OpenFilePath(std::string& text, const FileSearchFilter& filter)
{
    std::vector<std::string> fileSelected = pfd::open_file::open_file("Open Tree", "", { filter.name, filter.pattern }).result();
    if (!fileSelected.empty())
        text = fileSelected[0];
}

void SaveFilePath(std::string& text, const FileSearchFilter& filter)
{
    std::string result = pfd::save_file::save_file("Save Tree", "", { filter.name, filter.pattern }).result();
    if (!result.empty())
        text = result;
}
