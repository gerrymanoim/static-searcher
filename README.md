# C++ WebAssembly Static Search

Status: Working POC

![](https://github.com/gerrymanoim/static-searcher/blob/master/demo.gif)

Heavily inspired by https://github.com/tinysearch/tinysearch.

## Requirements

- emsdk
- cereal
- nlohmann json
- boost

Tested on gcc-9 + linux.

## Current usage

Currently this runs in two stages. 

First compile and execute the main program, passing in a json file and an output directory. 

```
 ./main --input ../fixtures/index.json --output /home/g/tmp/static_test/
```

Then navigate to the output directory you specifed a use cmake to compile the program there. 

Finally, test your search in `<your-output-dir>/dist`.


## TODO

- [ ] Consider a more efficient data structure than `unordred_set`.
- [ ] stop words, better clean up of post text
- [ ] Come up with a scoring/ranking mechanism for results

- [ ] Can this work end to end?
- [ ] Can you make the file sizes smaller here?
