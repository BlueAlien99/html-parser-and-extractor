# HTML Parser and Extractor

## Compilation

**Requires:**
* `libcurl4-openssl-dev`
* `libicu-dev`
* `Boost.Test` and `Boost.Algorithm`

```
sudo apt install libcurl4-openssl-dev
sudo apt install libicu-dev
sudo apt install libboost-all-dev
```

```
mkdir bin
cmake -DCMAKE_BUILD_TYPE=Release .
make
```

`./bin/run` -- demo (fetch any webpage and extract any content)

`./bin/test` -- run tests

## Coverage
```
cmake -DCMAKE_BUILD_TYPE=Debug .
make
make coverage
```
Then open `coverage/index.html`
