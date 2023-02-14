#include <algorithm>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <unordered_map>
#include <vector>
#include <source_location>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/spdlog.h>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <boost/lexical_cast.hpp>

#define ICINFO(expr) SPDLOG_INFO("{}: {}", #expr, expr)

using boost::lexical_cast;
namespace fs = std::filesystem;

int main()
{
    spdlog::set_level(spdlog::level::debug);
    fs::path fpath = std::source("sample.gcode");

    constexpr std::string_view delim{" "};

    std::ifstream ifs(fpath);
    std::unordered_map<char, double> positions;
    std::unordered_map<char, int> cmds;
    std::string line;
    while (!ifs.eof())
    {
        cmds.clear();
        std::getline(ifs, line);
        if (!line.empty())
        {
            SPDLOG_DEBUG("line: {}", line);
        }
        else
        {
            continue;
        }

        auto res = std::ranges::find(line, ';');
        std::string_view content{line.begin(), res};
        std::string_view comment{res, line.end()};
        SPDLOG_DEBUG("content: [{}]", content);
        SPDLOG_DEBUG("comment: [{}]", comment);
        if (content.empty())
        {
            continue;
        }

        for (auto sub : content | std::views::split(delim))
        {
            std::string_view token{sub.begin(), sub.end()};
            auto flag = sub.front();
            sub.advance(1);
            std::string_view sv{sub.begin(), sub.end()};
            switch (flag)
            {
            case 'N':
            case 'G':
            case 'M':
            case 'F':
            case 'S':
            case 'T': {
                ICINFO(std::ranges::starts_with("G01", token));
                try
                {
                    cmds[flag] = lexical_cast<int>(sv);
                }
                catch (boost::bad_lexical_cast &e)
                {
                    SPDLOG_WARN("Token: {}. {}", sv, e.what());
                }
            }
            break;
            case 'X':
            case 'Y':
            case 'Z':
            case 'I':
            case 'J':
            case 'K':
            case 'U':
            case 'V':
            case 'W': {
                try
                {
                    positions[flag] = lexical_cast<double>(sv);
                }
                catch (boost::bad_lexical_cast &e)
                {
                    SPDLOG_WARN("Token: {}. {}", sv, e.what());
                }
            }
            break;
            default: {
                SPDLOG_WARN("Cannot parese token {}", token);
            }
            break;
            }
        }
        SPDLOG_DEBUG("cmds: {}", cmds);
        SPDLOG_DEBUG("positions: {}", positions);
    }
    return 0;
}