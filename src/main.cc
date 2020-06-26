#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

// for the life of me I cannot get boost+macOS+gcc-9 to play nicely so using this instead
#include <cxxopts.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/utility.hpp>
#include <nlohmann/json.hpp>

using name_url = std::pair<std::string, std::string>;

struct PairHash {
    // TODO would it be better to just compbine the strings?
    std::size_t operator()(const name_url& k) const {
        return std::hash<std::string>()(k.first) ^
               (std::hash<std::string>()(k.second) << 1);
    }
};

// for convenience
using json = nlohmann::json;

void process_input(const std::filesystem::path& input_file_path,
                   const std::filesystem::path& output_directory) {
    std::cout << "Parsing json at " << input_file_path;
    json j;
    using post_contents = std::unordered_set<std::string>;
    std::unordered_map<name_url, post_contents, PairHash> post_map;
    std::ifstream i(input_file_path);
    i >> j;

    for (auto& post : j) {
        std::cout << "Processing " << post.at("title") << "\n";
        name_url key(post.at("title"), post.at("url"));
        std::string post_text = post.at("body");
        boost::algorithm::to_lower(post_text);
        boost::tokenizer<> tokens(post_text);

        post_map[key] = post_contents(tokens.begin(), tokens.end());
    };

    auto build_dir = std::filesystem::temp_directory_path() / "static_searcher";
    std::filesystem::create_directory(build_dir);
    std::cout << "Building inside of " << build_dir;
    {
        std::ofstream output(build_dir / "post_data.bin", std::ios::binary);
        cereal::BinaryOutputArchive archive(output);
        archive(post_map);
    }
}

int main(int argc, char** argv) {

    // clang-format off
    cxxopts::Options options("StaticSearcher", "WebAssembly Static Site Searcher");
    options.add_options()
        ("help", "Print usage")
        ("i,input", "input file path", cxxopts::value<std::string>())
        ("output", "output directory", cxxopts::value<std::string>())
    ;
    // clang-format on

    auto passed_options = options.parse(argc, argv);

    if (passed_options.count("help") or !passed_options.count("input")) {
        std::cout << options.help() << '\n';
        return 1;
    }

    process_input(passed_options["input"].as<std::string>(),
                  passed_options["output"].as<std::string>());
}
