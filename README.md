# CookCookhNya frontend

## Building (development)

1. Install Conan package manager

2. To update dependencies (or install them first time)
```bash
conan install . --build missing --setting build_type=Debug
```

3. To configure
```bash
cmake --preset conan-debug
```

4. To build. The binary is `build/Debug/main`.
```bash
cmake --build --preset conan-debug
```

5. If you need `compile_commands.json` for LSP or other,
it is located in `build/Release/compile_commands.json` and is regenerated when CMake is configured.
Clangd's default search path of it is `.` and `./build`, so I propose create a symlink to the file:
```bash
ln -s Debug/compile_commands.json build/compile_commands.json
```


## Building (production)

1. Install Conan package manager

2. To update dependencies (or install them first time)
```bash
conan install . --build missing --setting build_type=Release
```

3. To configure
```bash
cmake --preset conan-release
```

4. To build. The binary is `build/Release/main`.
```bash
cmake --build --preset conan-release
```

Use `Dockerfile`.
