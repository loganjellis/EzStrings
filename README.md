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

### Using EzStrings (in-directory, ez_strings.h inside /include, libez_strings.a files inside /lib, and any .dll files next any executables)

```
target_include_directories(app PRIVATE "path/to/include")
target_link_libraries(app PRIVATE "path/to/lib")
```

### Using EzStrings (subdirectory)

```
add_subdirectory(EzStrings)
target_link_libraries(app PRIVATE EzStrings::ez_strings)
```

## Example and Documentation

Please refer to `example.c` for an example of EzString code. The library's documentation can also be found in <a href="docs/html" targets="_blank">docs</a>.
