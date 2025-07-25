# The 21 School project - `decimal`

![decimal](misc/s21_decimal.png)

## Makefile Commands

| Command | Description | Tips |
|---------|-------------|------|
| `make all` | Build library with style checks and tests | Runs style-check and test targets. Note: You need to run `make style-format` separately before committing |
| `make test` | Run all tests with valgrind | Tests are run with memory checking enabled. Check test.log for detailed valgrind output |
| `make %.test` | Run specific test suite | Name your test file as `test_<function>.c` and suite as `<function>` (without s21_ prefix). Example: `make strlen.test` for `test_strlen.c` |
| `make gcov_report` | Generate code coverage report | Opens report in browser automatically (uses `open` on macOS, `xdg-open` on Linux). Report is generated in `coverage/web/` |
| `make release` | Build release version | Enables optimizations (-O2) and disables debug info. Outputs to `decimal.a` |
| `make gdb` | Build debug version and launch GDB | Enables debug symbols (-g) and automatically launches GDB. Use `tui enable` for better interface and `b main` to set breakpoint at main |
| `make style-format` | Format code using clang-format | Uses Google style guide. Run this before committing your changes |
| `make style-check` | Check code style and run cppcheck | Checks for style violations and potential bugs. Run this before committing |
| `make clean` | Remove build artifacts | Cleans build directory, library, tests, and coverage reports. Use this when switching build modes |
| `make rebuild` | Clean and rebuild everything | Useful when switching between debug/release modes or after major changes |

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