# Drakar och Demoner – Character Creator

A terminal-based character creator for the Swedish tabletop RPG *Drakar och Demoner*, built with [FTXUI](https://github.com/ArthurSonzogni/FTXUI).

## Requirements

| Tool | Minimum version |
|------|----------------|
| CMake | 3.16 |
| C++ compiler | C++17 support |
| Git | any recent version |
| Internet access | required on first build (FetchContent downloads dependencies) |

**Linux / macOS:** GCC 9+ or Clang 10+
**Windows:** Visual Studio 2019 (MSVC 19.20+), or MinGW-w64 / LLVM/Clang via `winlibs`

---

## Building on Linux / macOS

```bash
# 1. Clone the repository
git clone <repo-url>
cd Drakar

# 2. Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# 3. Build
cmake --build build --parallel

# 4. Run
./build/bin/dod_creator
```

To run the tests:

```bash
ctest --test-dir build --output-on-failure
```

To clean the build:

```bash
# Remove only compiled artifacts (keeps CMake configuration)
cmake --build build --target clean

# Full clean — remove and re-configure from scratch
rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Release
```

---

## Building on Windows

### Option A – Visual Studio 2019 / 2022 (recommended)

Install **Visual Studio 2019** or **2022** with the **"Desktop development with C++"** workload.
Install **CMake** from <https://cmake.org/download/> and tick *"Add CMake to the system PATH"* during setup.

Open a **Developer Command Prompt for VS 202x** (search for it in the Start menu — this sets
up the compiler environment automatically) and run:

```cmd
rem 1. Clone the repository
git clone <repo-url>
cd Drakar

rem 2. Configure using the Visual Studio generator (no Ninja required)
cmake -B build -G "Visual Studio 18 2026" -A x64

rem 3. Build
cmake --build build --config Release --parallel

rem 4. Run
.\build\bin\dod_creator.exe
```

Adjust the generator name to match your installed version:

| Visual Studio | Generator string |
|---|---|
| VS 2019 | `"Visual Studio 16 2019"` |
| VS 2022 | `"Visual Studio 17 2022"` |
| VS 2026 | `"Visual Studio 18 2026"` |

> **Why not Ninja?**
> The error *"CMake was unable to find a build program corresponding to Ninja"*
> means Ninja is not on your PATH. The Visual Studio generator above works out of
> the box without installing anything extra.  If you do have Ninja installed you can
> use `-G "Ninja" -DCMAKE_BUILD_TYPE=Release` instead, but you **must** run the
> command from a **Developer Command Prompt** (or call `vcvarsall.bat` first) so
> that MSVC is on the PATH.

> **UTF-8 console on Windows**
> The application displays Swedish characters (å, ä, ö) and emoji. Run
> `chcp 65001` in your terminal **before** launching the executable, or use
> **Windows Terminal** which defaults to UTF-8 automatically.

To clean the build (Option A):

```cmd
rem Remove compiled artifacts only
cmake --build build --target clean

rem Full clean — delete and re-configure from scratch
rmdir /s /q build
cmake -B build -G "Visual Studio 17 2022" -A x64
### Option B – MinGW-w64 via MSYS2

Install [MSYS2](https://www.msys2.org/), open an **MSYS2 MinGW 64-bit** shell, then:

```bash
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-ninja git

git clone <repo-url>
cd Drakar

cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel

./build/bin/dod_creator.exe
```

> Ninja and GCC are already available inside the MSYS2 MinGW shell, so the
> `CMAKE_MAKE_PROGRAM` / `CMAKE_CXX_COMPILER` errors do not occur there.

To clean the build (Option B):

```bash
# Remove compiled artifacts only
cmake --build build --target clean

# Full clean — delete and re-configure from scratch
rm -rf build
cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release
```

---

## Project structure

```
Drakar/
├── CMakeLists.txt          # Root build file
├── src/
│   ├── core/               # Game logic library (dod_core)
│   │   ├── include/core/   # Public headers
│   │   └── src/            # Implementation
│   └── app/                # Executable (dod_creator)
│       ├── main.cpp
│       └── tui/            # FTXUI screens and theme
├── tests/                  # GoogleTest unit tests
└── Rules/                  # Reference rule texts
```

## Dependencies (fetched automatically by CMake)

| Library | Version | Purpose |
|---------|---------|---------|
| [FTXUI](https://github.com/ArthurSonzogni/FTXUI) | v6.1.9 | Terminal UI |
| [nlohmann/json](https://github.com/nlohmann/json) | v3.11.3 | JSON save/load |
| [GoogleTest](https://github.com/google/googletest) | v1.14.0 | Unit tests |
