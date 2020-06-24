#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include <emscripten/bind.h>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/utility.hpp>

namespace static_search
{
    using name_url = std::pair<std::string, std::string>;
    struct PairHash
    {
        // TODO would it be better to just compbine the strings?
        std::size_t operator()(const name_url &k) const
        {
            return std::hash<std::string>()(k.first) ^ (std::hash<std::string>()(k.second) << 1);
        }
    };

    using post_contents = std::unordered_set<std::string>;

    std::unordered_map<name_url, post_contents, PairHash> post_map;

    //todo write an adapter for vecotr<pair> or unordered_set<pair>
    std::map<std::string, std::string> search(std::string query, int n_results)
    {
        std::map<std::string, std::string> out;

        boost::algorithm::to_lower(query);
        boost::tokenizer<> tokens(query);
        for (auto &token : tokens)
        {
            for (auto &[k, v] : post_map)
            {
                auto search = v.find(token);
                if (search != v.end())
                {
                    out.emplace(k);
                }
            }
        }
        return out;
    }

} // namespace static_search

int main()
{
    std::ifstream input("./post_data.bin", std::ios::binary);
    cereal::BinaryInputArchive iarchive(input);
    iarchive(static_search::post_map);
}

using namespace emscripten;

EMSCRIPTEN_BINDINGS(module)
{

    //must register bindings for more complicated return types
    register_map<std::string, std::string>("map<string, string>");
    //note: we must register these in order to access the keys of our maps
    register_vector<std::string>("vector<string>");

    function("search", &static_search::search);
}
