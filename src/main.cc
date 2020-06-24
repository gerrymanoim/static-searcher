#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>


#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/utility.hpp>
#include <nlohmann/json.hpp>

using name_url = std::pair<std::string, std::string>;

struct PairHash
{
    // TODO would it be better to just compbine the strings?
    std::size_t operator()(const name_url &k) const
    {
        return std::hash<std::string>()(k.first) ^ (std::hash<std::string>()(k.second) << 1);
    }
};

// for convenience
using json = nlohmann::json;

int main()
{
    json j;
    using post_contents = std::unordered_set<std::string>;
    std::unordered_map<name_url, post_contents, PairHash> post_map;

    std::cin >> j;
    for (auto &post : j)
    {
        std::cout << "Processing " << post.at("title") << "\n";
        name_url key(post.at("title"), post.at("url"));
        std::string post_text = post.at("body");
        boost::algorithm::to_lower(post_text);
        boost::tokenizer<> tokens(post_text);

        post_map[key] = post_contents(tokens.begin(), tokens.end());
    };

    {
        std::ofstream output("./out.bin", std::ios::binary);
        cereal::BinaryOutputArchive archive(output);
        archive(post_map);
    }
}
