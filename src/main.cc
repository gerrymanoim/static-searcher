#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
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

constexpr char const *web_runner =
#include "main.web.cc"
;

constexpr char const *web_page =
#include "index.html"
;

constexpr char const *web_builder =
#include "CMakeLists.web.txt"
;

// for convenience
using json = nlohmann::json;

void process_input(const std::filesystem::path& input_file_path,
                   const std::filesystem::path& output_directory) {
    std::cout << "Parsing json at " << input_file_path << "\n";
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

    std::filesystem::create_directory(output_directory);
    std::cout << "Outputting to " << output_directory << "\n";
    {
        std::ofstream output(output_directory / "post_data.bin", std::ios::binary);
        cereal::BinaryOutputArchive archive(output);
        archive(post_map);
    }
    {
        std::ofstream output(output_directory/ "main.cc");
        output << web_runner;
    }
    {
        std::ofstream output(output_directory/ "CMakeLists.txt");
        output << web_builder;
    }

    auto dist_directory = output_directory / "dist";
    std::filesystem::create_directory(dist_directory);
    {
        std::ofstream output(dist_directory / "index.html");
        output << web_page;
    }
}

namespace po = boost::program_options;

int main(int argc, const char* argv[]) {

    try {
        // clang-format off
        po::options_description desc{"StaticSearcher - WebAssembly Static Site Searcher"};
        desc.add_options()
            ("help", "Print usage")
            ("input", po::value<std::string>(), "input file path")
            ("output", po::value<std::string>(), "output directory")
        ;
        // clang-format on
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help") or !vm.count("input")) {
            std::cout << desc << "\n";
            return 1;
        }

        process_input(vm["input"].as<std::string>(), vm["output"].as<std::string>());

        return 0;
    }

    catch (const po::error& ex) {
        std::cerr << ex.what() << '\n';
    }
}
