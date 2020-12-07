# HTML Parser and Extractor

## Compilation

**Requires `libcurl4-openssl-dev`**
```
sudo apt install libcurl4-openssl-dev
```

```
cmake .
make
```

`./bin/run` -- to test fetching onet.pl

`./bin/lexer_html` -- to test html lexer

`./bin/lexer_conf` -- to test configuration string lexer

## Testing
```
cmake -DCMAKE_BUILD_TYPE=Debug .
make
make coverage_html
make coverage_conf
make coverage
```
Then open `coverage/index.html`