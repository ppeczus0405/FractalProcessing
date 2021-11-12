#include <filesystem>
#include <string>

namespace Config
{
    constexpr char OS_SEPARATOR = static_cast<char>(std::filesystem::path::preferred_separator);
    const std::string SAVE_PATH = string("/home/peki/Programowanie/C++/FractalProcessing") + OS_SEPARATOR + "generated_images";
}
