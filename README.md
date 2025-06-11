# CookCookhNya frontend

## Building (development)

1. Install Conan package manager

2. To update dependencies (or install them first time)
```bash
conan install . --build=missing
```

3. To configure
```bash
cmake --preset conan-release
```

4. To build. The binary is `build/Release/main`.
```bash
cmake --build --preset conan-release
```

5. If you need `compile_commands.json` for LSP or other,
it is located in `build/Release/compile_commands.json` and is regenerated when CMake is configured.

## Building (production)

Use `Dockerfile`.
