# CookCookhNya frontend

## Building (development)

1. Install Conan package manager.

2. To build. The binary is `build/Debug/main`.
```bash
make build-debug
```

3. If you need `compile_commands.json` for LSP or similar,
it is located in `build/Debug/compile_commands.json` and is regenerated when CMake is configured.
Clangd's default search path of it is `.` and `./build`, so I propose create a symlink to the file:
```bash
ln -s Debug/compile_commands.json build/compile_commands.json
```

4. To run GDB to debug the application use a shortcut:
```bash
make gdb
```


## Building (production)

It's better to use `Dockerfile`. If you need to build locally, follow these steps:

1. Install Conan package manager

2. To build. The binary is `build/Release/main`.
```bash
make build-release
```

## Run

```bash
make start-debug
# or
make start-release
```
