# C++ WebAssembly Static Search

Heavily inspired by https://github.com/tinysearch/tinysearch.

## Requirements

- emsdk
- gcc-9
- cereal
- nlohmann json
- boost

## Current usage

Not ideal. Just for my notes:

```
cd web
em++ --std=gnu++17 -I/usr/local/Cellar/cereal/1.3.0/include -I/usr/local/Cellar/nlohmann-json/3.7.3/include -I/usr/local/Cellar/boost/1.72.0_3/include --bind --preload-file post_data.bin -o static_search.js main.cc
python -m http.server 8999
```


## TODO

- [ ] End to end program run

- [ ] Better dependency inclusion
- [ ] Makefile
- [ ] Consider a more efficient data structure than `unordred_set`.
- [ ] stop words, better clean up of post text
- [ ] Come up with a scoring/ranking mechanism for results
- [ ] Dedupe code via include file for main structures
- [ ] use abseil containers
