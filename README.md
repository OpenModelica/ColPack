# ColPack

[![build](https://github.com/OpenModelica/ColPack/actions/workflows/build.yml/badge.svg)](https://github.com/OpenModelica/ColPack/actions/workflows/build.yml)

> [!NOTE]
> This is a fork of the original [ColPack](https://github.com/CSCsw/ColPack),
> which has not changed since 2019.
> It is maintained mainly for use in
> [OpenModelica](https://github.com/OpenModelica/OpenModelica), but it is still
> a general-purpose library.
> Changes should keep ColPack usable by any project and not tie it to
> OpenModelica.
> See [ChangeLog](ChangeLog) for the changes since the original ColPack 1.0.10.

ColPack's Doxygen documentation is available here:
<https://cscapes.cs.purdue.edu/coloringpage/software.htm>

ColPack's project home page:
<https://cscapes.cs.purdue.edu/coloringpage/>

## Table of Contents

1. [ColPack](#about-colpack)
2. [Installation Guilds](#build-and-compile-colpack-instructions)
   1. [Prebuilt Packages](#prebuilt-packages)
   2. [Compile ColPack Without Install](#try-colpack-by-compile-and-run-without-installation)
   3. [Ubuntu Install](#ubuntu-build-and-install-colpack-instructions)
   4. [Windows Install](#windows-build-and-install-colpack-instructions)
   5. [MacOS Install](#mac-os-build-and-install-colpack-instructions)
   6. [Utilize the Installed Library](#after-the-build-use-colpack-as-installed-library)
   7. [Code Formatting](#code-formatting)
   8. [Making a Release](#making-a-release)
3. [Usages](#usage)
4. [HowToCite](#the-best-source-for-citing-this-work)

## About ColPack

ColPack is a package comprising of implementations of algorithms for the
specialized vertex coloring problems discussed in the previous section as well
as algorithms for a variety of related supporting tasks in derivative
computation.

### Vertex Graph Coloring

Vertex graph coloring problem is nothing but a way of labelling graph vertices
under the constraints that no two adjacent vertices has the same lable (color).
Here it is an example from wikipedia.

[![3-coloring of the Petersen graph](https://upload.wikimedia.org/wikipedia/commons/9/90/Petersen_graph_3-coloring.svg)](https://commons.wikimedia.org/wiki/File:Petersen_graph_3-coloring.svg)

### ColPack Coloring capabilities

the table below gives a quick summary of all the coloring problems (on general
and bipartite graphs) supported by ColPack.

| General Graph | Bipartite Graph one-sided | Bipartite Graph Bicoloring |
| --- | --- | --- |
| Distance 1 coloring | Partial distance-2 coloring | Star bicoloring |
| Distance 2 coloring | Partial distance-2 coloring | |
| Star coloring | | |
| Acyclic coloring | | |
| Restricted star coloring | | |
| Triangular coloring | | |

All of the coloring problems listed in the above table are NP-hard. Their
corresponding algorithms in ColPack are *greedy* heuristics in the sense that
the algorithms progressively extend a partial coloring by processing one vertex
at a time, in some order, in each step assigning a vertex the smallest allowable
color. Listed beneath each coloring problem in the table is the complexity of
the corresponding algorithm in ColPack. In the cases where ColPack has multiple
algorithms for a problem (these are designated by the superscript †), the
complexity expression corresponds to that of the fastest algorithm. In the
complexity expressions,

*the complexity of the corresponding algorithm can be found here
[ColPack's project](https://cscapes.cs.purdue.edu/coloringpage/software.htm)*

### Ordering techniques

The order in which vertices are processed in a greedy coloring algorithm
determines the number of colors used by the algorithm. ColPack has
implementations of various effective ordering techniques for each of the
supported coloring problems. These are summarized below.

- General Graph Coloring
  - Natural
  - Largest First
  - Smallest Last
  - Incidence Degree
  - Dynamic Largest First
  - Distance-2 Largest First
  - Distance-2 Smallest Last
  - Distance-2 Incidence Degree
  - Distance-2 Dynamic Largest First
- Bipartite Graph one-sided coloring
  - Column Natural
  - Column Largest First
  - Column Smallest Last
  - Column Incidence Degree
  - Row Natural
  - Row Largest First
  - Row Smallest Last
  - Row Incidence Degree
- Bipartite Graph Bicoloring
  - Natural
  - Largest First
  - Smallest Last
  - Incidence Degree
  - Dynamic Largest First
  - Selective Largest First
  - Selective Smallest Last
  - Selective Incidence Degree

### Recovery routines

Besides coloring and ordering capabilities, ColPack also has routines for
recovering the numerical values of the entries of a derivative matrix from a
compressed representation. In particular the following reconstruction routines
are currently available:

- Recovery routines for direct (via star coloring ) and substitution-based (via
  acyclic coloring) Hessian computation
- Recovery routines for unidirectional, direct Jacobian computation (via
  column-wise or row-wise distance-2 coloring)
- Recovery routines for bidirectional, direct Jacobian computation via star
  bicoloring

### Graph construction routines

Finally, as a supporting functionality, ColPack has routines for constructing
bipartite graphs (for Jacobians) and adjacency graphs (for Hessians) from files
specifying matrix sparsity structures in various formats, including Matrix
Market, Harwell-Boeing and MeTis.

### ColPack : organization

ColPack is written in an object-oriented fashion in C++ heavily using the
Standard Template Library (STL). It is designed to be simple, modular,
extensible and efficient. Figure 1 below gives an overview of the structure of
the major classes of ColPack.

![ColPack Organization](https://cscapes.cs.purdue.edu/coloringpage/software_files/ColPack_structure_2.png)

## Build and Compile ColPack Instructions

There are two ways to use ColPack, *Try without Installiation* and *Build and
Install*. The former is fast and easy to use, but is vulnerable for various OS
enviroments settings, thus it requires the user know how to modify the
**makefile** if met some compiling issue. The later one is more robust and it
will also collect the ColPack into a shared library which makes ColPack easy to
cooperate with other applications. But it requires to pre-install **CMake**
software.

### Prebuilt Packages

Each [release](https://github.com/OpenModelica/ColPack/releases) provides
prebuilt packages, so ColPack can be used without building it:

| Package | Platform |
| --- | --- |
| `colpack-<version>-linux-x86_64.tar.gz` | Linux x86_64, glibc 2.35+, GCC |
| `colpack-<version>-linux-aarch64.tar.gz` | Linux AArch64, glibc 2.35+, GCC |
| `colpack-<version>-macos-arm64.tar.gz` | macOS 15+ on Apple silicon |
| `colpack-<version>-windows-msvc-x64.zip` | Windows x64, MSVC |
| `colpack-<version>-windows-ucrt64-x64.zip` | Windows x64, MSYS2 UCRT64 |

Each package contains the headers, the static and shared libraries built in
Release mode and the CMake package. Extract it and
[use it as installed library](#after-the-build-use-colpack-as-installed-library).
ColPack needs an OpenMP runtime: libgomp on Linux and MSYS2, `libomp` from
Homebrew on macOS and the LLVM OpenMP runtime (`libomp140.x86_64.dll`) of
Visual Studio for MSVC. The MSVC libraries link the release C runtime (`/MD`),
so use them in Release or RelWithDebInfo builds only.
`SHA256SUMS` lists the checksums of all packages and of the source archive.

### Try ColPack by Compile and Run without Installation

You can just try ColPack by download, compile and run it. This is the fastest
and simplest way to use ColPack. Do the following instructions in terminals.

```sh
cd
git clone https://github.com/CSCsw/ColPack.git   #Download ColPack
cd ColPack                   # go to ColPack Root Directory
cd Examples/ColPackAll       # go to ColPack Example folder
make                         # compile the code
```

After all source codes been compiled, we will generate a executable file
`ColPack` under current folder.
The above instruction are tested under Ubuntu system. You may need to modify
the Makefile to fit the different OS environments and compilers.(delete
`-fopenmp` for mac os. Replace `-fopenmp` to `-Qopenmp` )for intel compiler.)

### Ubuntu Build and Install ColPack Instructions

Install ColPack makes ColPack easy to use and it can also decreases the size of
the execuable file. ColPack is built with **CMake** (version 3.12 or newer) and
a compiler supporting OpenMP. To build, test and install ColPack, follow the
instructions below:

```sh
cd
git clone https://github.com/OpenModelica/ColPack.git  #Download ColPack
cd ColPack             # ColPack Root Directory
fullpath=$(pwd)/install  # modify fullpath to your destination folder if need
cmake -S . -B build -DCMAKE_INSTALL_PREFIX:PATH=${fullpath}
cmake --build build -j 4   # Where "4" is the number of cores on your machine
ctest --test-dir build     # run the Basic examples as tests
cmake --install build      # install the libraries, headers and CMake package
```

Use `cmake -LH build` or `ccmake build` to see a list of
options, such as `COLPACK_ENABLE_EXAMPLES` and `COLPACK_ENABLE_OPENMP`, which
you can set when configuring:

```sh
cmake -S . -B build -DCOLPACK_ENABLE_EXAMPLES=ON
```

If not using`-DCMAKE_INSTALL_PREFIX:PATH`, the library files will be installed
under `/usr/local/` by default which may requires privilege.

### Windows Build and Install ColPack Instructions

ColPack can be built with Visual Studio 2019 16.9 or newer and CMake 3.30 or
newer. OpenMP is enabled with `/openmp:llvm`, because Visual Studio's default
OpenMP 2.0 is not sufficient. ColPack can also be built with MSYS2 and GCC.

On Windows, the examples link to the static library instead of the shared
library.

Unlike on UNIX, the static library is named ColPack_static (ColPack_static.lib)
to avoid a name conflict with the shared library's ColPack.lib.

Finally, some of the examples have file names so long that their object file
paths exceed the Windows path length limit. ColPack therefore defaults to
`CMAKE_INTERMEDIATE_DIR_STRATEGY=SHORT` on Windows, which requires CMake 4.2 or
newer to build all examples.

### MAC OS Build and Install ColPack Instructions

To install ColPack on Mac, you first need to install *Apple Xcode* and *CMake*.
ColPack requires OpenMP. Mac's default compiler clang supports OpenMP but does
not ship the runtime library, so install *libomp* with
[Homebrew](https://brew.sh) and point CMake to it. Otherwise follow the
[Ubuntu instructions](#ubuntu-build-and-install-colpack-instructions).

```sh
brew install libomp
cmake -S . -B build -DOpenMP_ROOT=$(brew --prefix libomp)
cmake --build build -j 4
ctest --test-dir build
```

Another recommend altinative way is to install an Ubuntu system on your MAC
with *VirtualBox* (or any other virtual machine software), then install ColPack
on your virtual machines.

### After the Build, Use ColPack as Installed Library

ColPack installs a CMake package that provides the targets
`ColPack::ColPack_static` and `ColPack::ColPack_shared`, which carry the include
directories and OpenMP flags. Set `CMAKE_PREFIX_PATH` to the install prefix and
use:

```cmake
find_package(ColPack REQUIRED)
target_link_libraries(myapp PRIVATE ColPack::ColPack_static)
```

Include the headers with `#include "ColPackHeaders.h"`.

### Use ColPack as a Subdirectory

ColPack can also be added to another CMake project, for example as a git
submodule. The targets `ColPack_static` and `ColPack_shared` then carry the
include directories of the source tree:

```cmake
set(COLPACK_BUILD_SHARED OFF CACHE BOOL "" FORCE)
add_subdirectory(ColPack)
target_link_libraries(myapp PRIVATE ColPack_static)
```

When ColPack is a subdirectory, `COLPACK_ENABLE_TESTS` defaults to `OFF`, so the
ColPack executable and the Basic examples are not built and no tests are added
to the parent project. `COLPACK_BUILD_SHARED` (default `ON`) controls whether
the shared library is built and installed.

### Code Formatting

The C and C++ sources are formatted with clang-format 23.1.1 using the style in
`.clang-format`. CI checks the formatting of every pull request. To format all
sources:

```sh
pipx run clang-format==23.1.1 -i $(git ls-files '*.c' '*.cpp' '*.h' '*.hpp')
```

### Making a Release

1. Set the version in `project()` and `COLPACK_VERSION` in `CMakeLists.txt`.
2. Add an entry for the version at the top of `ChangeLog`.
3. After the changes are merged, tag the commit on `master` and push the tag:

   ```sh
   git tag -a v1.1.0 -m "ColPack 1.1.0"
   git push origin v1.1.0
   ```

The [release workflow](.github/workflows/release.yml) then checks that the tag
matches the version, builds and tests the packages and publishes them as a
GitHub release.

## USAGE

After building (or compile), you can run the following commands from where the
executable file `ColPack` generated (the build directory if using CMake, or
current directory if directly compile):

```sh
./ColPack -f <graph_file_name> -o <ordering> -m <methods> [-v] ...
```

### DISPLAY HELP

```sh
./ColPack
```

### OPTIONs

```text
<gfile_name>:  Input file name
<ordering>  :  LARGEST_FIRST
               SMALLEST_LAST,
               DYNAMIC_LARGEST_FIRST,
               INCIDENCE_DEGREE,
               NATURAL,
               RANDOM,
               ...
<methods>   :  DISTANCE_ONE
               ACYCLIC
               ACYCLIC_FOR_INDIRECT_RECOVERY
               STAR
               RESTRICTED_STAR
               DISTANCE_TWO
               --------------------
               IMPLICIT_COVERING__STAR_BICOLORING
               EXPLICIT_COVERING__STAR_BICOLORING
               EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING
               IMPLICIT_COVERING__GREEDY_STAR_BICOLORING
               --------------------
               COLUMN_PARTIAL_DISTANCE_TWO
               ROW_PARTIAL_DISTANCE_TWO
               --------------------
               D1_OMP_GMMP
               D1_OMP_GM3P
               D1_OMP_GMMP_LOLF
               D1_OMP_GM3P_LOLF
               D1_OMP_...
               ...
               --------------------
               D2_OMP_GMMP
               D2_OMP_GM3P
               D2_OMP_GMMP_LOLF
               D2_OMP_GM3P_LOLF
               --------------------
               PD2_OMP_GMMP
               PD2_OMP_GM3P
               PD2_OMP_GMMP_LOLF
               PD2_OMP_GM3P_LOLF
               ...

-v          :  # verbose for debug infomation
-fmt        :  MM/SQRT  # only used by Partial Distance Two Parallel graph
               coloring. SQRT will read sqrt of grahp.
-low        :  # only used by Partial Distance Two Parallel graph coloring.
               The lower bound of coloring information will be displayed.
```

### EXAMPLES

```sh
./ColPack -f ./Graphs/bcsstk01.mtx -o LARGEST_FIRST -m DISTANCE_ONE -v
./ColPack -f ./Graphs/bcsstk01.mtx -o SMALLEST_LAST -m ACYCLIC -v
./ColPack -f ./Graphs/bcsstk01.mtx -o DYNAMIC_LARGEST_FIRST \
  -m DISTANCE_ONE_OMP -v
./ColPack -f ./Graphs/bcsstk01.mtx -o RANDOM -m D1_OMP_GMMP D2_OMP_GMMP \
  -nT 1 2 4 -v
./ColPack -f ./Graphs/bcsstk01.mtx -o RANDOM -m PD2_OMP_GMMP PD2_OMP_GMMP_LOLF \
  -nT 1 2 4 -v
```

### EXAMPLE OUTPUT

```text
ReadMatrixMarketAdjacencyGraph
Found file Graphs/bcsstk01.mtx
Graph of Market Market type: [matrix coordinate real symmetric]
    Graph structure and VALUES will be read

#DISTANCE_ONE Result:
6  : (NATURAL)
6  : (LARGEST_FIRST)
6  : (DYNAMIC_LARGEST_FIRST)
6  : (SMALLEST_LAST)
6  : (INCIDENCE_DEGREE)
6  : (RANDOM)

#ACYCLIC Result:
8  : (NATURAL)
8  : (LARGEST_FIRST)
8  : (DYNAMIC_LARGEST_FIRST)
8  : (SMALLEST_LAST)
8  : (INCIDENCE_DEGREE)
8  : (RANDOM)

#ACYCLIC_FOR_INDIRECT_RECOVERY Result:
8  : (NATURAL)
8  : (LARGEST_FIRST)
8  : (DYNAMIC_LARGEST_FIRST)
8  : (SMALLEST_LAST)
8  : (INCIDENCE_DEGREE)
8  : (RANDOM)

#STAR Result:
12  : (NATURAL)
12  : (LARGEST_FIRST)
12  : (DYNAMIC_LARGEST_FIRST)
12  : (SMALLEST_LAST)
12  : (INCIDENCE_DEGREE)
12  : (RANDOM)

#RESTRICTED_STAR Result:
15  : (NATURAL)
15  : (LARGEST_FIRST)
15  : (DYNAMIC_LARGEST_FIRST)
15  : (SMALLEST_LAST)
15  : (INCIDENCE_DEGREE)
15  : (RANDOM)

#DISTANCE_TWO Result:
15  : (NATURAL)
15  : (LARGEST_FIRST)
15  : (DYNAMIC_LARGEST_FIRST)
15  : (SMALLEST_LAST)
15  : (INCIDENCE_DEGREE)
15  : (RANDOM)
```

## The best source for citing this work

Assefaw H. Gebremedhin, Duc Nguyen, Mostofa Ali Patwary, and Alex Pothen,
*[ColPack: Graph coloring software for derivative computation and
beyond](https://doi.org/10.1145/2513109.2513110)*,
ACM Transactions on Mathematical Software, 40 (1), 30 pp., 2013.
