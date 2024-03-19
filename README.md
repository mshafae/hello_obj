
# OBJ Viewer

This is [tinyobjloader's](https://github.com/tinyobjloader/tinyobjloader) [viewer](https://github.com/tinyobjloader/tinyobjloader/tree/release/examples/viewer) example. It has been reorganized to help a student learn how to use tinyobjloader.

It includes a simple Makefile that can be used to build the viewer on MacOS (with Macports) and Ubuntu 22 with clang++.

On Ubuntu 22, the following command will install the needed packages.

```sh
sudo apt install -y --no-install-recommends build-essential clang clang-format clang-tidy libglm-dev libglew-dev libglfw3-dev libtinyobjloader-dev libstb-dev
```

Models can be downloaded from Morgan McGuire's [Computer Graphics Archive](https://casual-effects.com/data). Some suggested models are:

* [Dragon](https://casual-effects.com/g3d/data10/research/model/dragon/dragon.zip)
* [Bunny](https://casual-effects.com/g3d/data10/research/model/bunny/bunny.zip)

