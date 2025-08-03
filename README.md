# <span><img style="height: 1em;" src="../misc/heart.gif"></span> The 21 School project - `decimal`

![decimal](misc/s21_decimal.png)

## Description

<!-- TODO(anybody): create small, informative and funny description -->

## Dependencies

You better to have `clang-format`, `cppcheck`, `clang-tidy`, `valgrind`, `lcov` and `pkg-config`.

For fast install on ubuntu-based system do:
```bash
sudo apt-get -y install clang-tidy cppcheck clang-tidy valgrind lcov pkg-config
```

## Make targets

| Target          | Description |
|-----------------|-------------|
| `all`           | Build library with all necessary checks and tests |
| `clean`         | Remove all build artifacts |
| `test`          | Run all tests under valgrind |
| `s21_decimal.a` | Build the static library |
| `gcov_report`   | Generate code coverage report |
| `help` | Show available targets |


## Project Structure

```
├── src/           # Source files
├── include/       # Header files
├── tests/         # Test files
├── build/         # Build artifacts
│   ├── obj/      # Object files
│   └── test/     # Test object files
└── coverage/      # Coverage reports
    └── web/      # HTML coverage reports
```