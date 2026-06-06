# EzStrings

**EzStrings** is a library aimed to make strings easier to use and edit in C. EzStrings
handles all the memory management, you handle the rest. It's that ez.

> [!NOTE]
> EzStrings is not thread-safe as of right now.

## Clone the repository

```
git clone https://github.com/loganjellis/EzStrings.git
cd EzStrings
```

## Building (and optionally installing)

```
cmake -S . -B build
cmake --build build
(only include this line if you want to install EzStrings) cmake --install build --prefix ./install
```

> [!NOTE]
> Note that ./install is a placeholder install location for the library. Omitting the install location results in the library being installed in the operating system's default path.

### Using EzStrings (installed package)

```
list(APPEND CMAKE_PREFIX_PATH "/path/to/ez_strings/install")
find_package(EzStrings REQUIRED)
target_link_libraries(app PRIVATE EzStrings::ez_strings)
```

### Using EzStrings (subdirectory)

```
add_subdirectory(EzStrings)
target_link_libraries(app PRIVATE EzStrings::ez_strings)
```

## Example and Documentation

Please refer to `example.c` for an example of EzString code. The library's documentation can also be found in <a href="docs/html" targets="_blank">docs</a>.
