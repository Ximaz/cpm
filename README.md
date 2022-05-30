# CPM (C Package Manager)
This project has only one purpose : help me to improve my C coding skill. You can clone it and run the makefile commands to build the project, but for now, it does nothing exceptional.

# Roadmap

- [x] Be able to read a CPM configuration file.
    - [x] Be able to read a file.
    - [x] Be able to parse a file's content.
        - [x] Allowing values starting and ending by ``"``.
- [x] Be able to write a default CPM configuration.
    - [x] Be able to write a file.
    - [ ] Be able to parse config to text.
- [ ] Be able to search on a free market place some dependencies.
    - [ ] Setting up ``cURL`` localy using the tool itself.
    - [ ] Searching through ``GitHub``.

# Example CPM configuration

```ini
[INFO]
name="Project Name"
description="Project Desc"
version="2.0.1"
author="Ximaz"

[DEPENDENCIES]
curl="7.86.1"
```
