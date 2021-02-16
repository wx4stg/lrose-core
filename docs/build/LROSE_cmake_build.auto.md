# Build LROSE automatically using cmake - LINUX and MAC

1. [overview](#overview)
2. [prepare](#prepare)
3. [build](#build)
4. [verify](#verify)

<a name="overview"/>

## 1. Overview

The default install prefix is ```~/lrose```.

The default directories for installation are:

```
  ~/lrose/include
  ~/lrose/lib
  ~/lrose/bin
```

This can be changed using the ```--prefix``` argument on the scripts.

<a name="prepare"/>

## 2. Prepare

### Create a working directory for cloning:

```
  mkdir -p ~/git
  cd ~/git
```

### 2.1 Prepare LINUX

Most good, up-to date versions or LINUX should work.

First, you will need to install the required packages.

Recommended distributions are:

  * Debian
  * Ubuntu (based on Debian)
  * RedHat
  * CentOS (based on RedHat)
  * Fedora (based on RedHat)

For the required packages on each OS, see:

* [LROSE package dependencies](../build/lrose_package_dependencies.md)

You can install the packages automatically using a script in the bootstrap repository.

```
  cd ~/git
  git clone https://github.com/ncar/lrose-bootstrap
  cd ~/git/lrose-bootstrap/scripts
  sudo ./install_linux_packages.py --debug
```

### 2.2 Prepare MAX OSX

In preparation for building on a mac, you need to install XCode and Homebrew.

For details, see section 1 of:

* [Install XCode and Homebrew](../download/install_using_homebrew.mac_osx.md)

Then, use Homebrew to install the required packages:

```
  brew install hdf5 netcdf fftw flex jpeg libpng libzip qt szip pkg-config cmake rsync libx11 libxext
```

<a name="build-using-bootstrap"/>

## 3. Build using scripts in the bootstrap repository (recommended)

### Clone the bootstrap for LROSE

```
  cd ~/git
  git clone https://github.com/ncar/lrose-bootstrap
```

The distribution will be in the lrose-bootstrap subdirectory:

```
  cd ~/git/lrose-bootstrap
```

### Run the ```checkout_and_build_cmake.py``` script:

To see the usage:

```
  cd ~/git/lrose-bootstrap/scripts
  ./checkout_and_build_cmake.py --help
```

```
  Usage: checkout_and_build_cmake.py [options]
  Options:
    -h, --help            show this help message and exit
    --clean               Cleanup tmp build dir
    --debug               Set debugging on
    --verbose             Set verbose debugging on
    --package=PACKAGE     Package name. Options are: lrose-core (default),
                          lrose-radx, lrose-cidd, samurai
    --releaseDate=RELEASEDATE
                          Date from which to compute tag for git clone. Applies
                          if --tag is not used.
    --tag=TAG             Tag to check out lrose. Overrides --releaseDate
    --prefix=PREFIX       Install directory, default: ~/lrose
    --buildDir=BUILDDIR   Temporary build dir, default: /tmp/lrose-build
    --logDir=LOGDIR       Logging dir, default: /tmp/lrose-build/logs
    --static              use static linking, default is dynamic
    --installAllRuntimeLibs
                          Install dynamic runtime libraries for all binaries, in
                          a directory relative to the bin dir. System libraries
                          are included.
    --installLroseRuntimeLibs
                          Install dynamic runtime lrose libraries for all
                          binaries, in a directory relative to the bin dir.
                          System libraries are not included.
    --noScripts           Do not install runtime scripts as well as binaries
    --buildNetcdf         Build netcdf and hdf5 from source
    --fractl              Checkout and build fractl after core build is complete
    --vortrac             Checkout and build vortrac after core build is
                          complete
    --samurai             Checkout and build samurai after core build is
                          complete
    --cmake3              Use cmake3 instead of cmake for samurai
    --no_core_apps        Do not build the lrose core apps
    --withJasper          Set if jasper library is installed. This provides
                          support for jpeg compression in grib files.
    --verboseMake         Verbose output for make, default is summary
```

`package` defaults to `lrose-core`.

Available packages are:

```
  lrose-core lrose-cidd lrose-radx samurai
```

We recommend building the full core, unless you have specific limitations.

For older systems, such as centos7, you will need to use ```--cmake3```.

To build and install the full lrose package into ```~/lrose```:

```
  ./checkout_and_build_cmake.py
```

To specify the sub-package, e.g. lrose-radx:

```
  ./checkout_and_build_cmake.py --package lrose-radx
```

To set the install directory:

```
  ./checkout_and_build_cmake.py --prefix /my/install/dir
```

To cleanup between builds:

```
  ./checkout_and_build_cmake.py --clean
```

<a name="build-using-script"/>

## 4. Build using scripts in lrose-core (intermediate)

### Clone lrose-core

```
  cd ~/git
  git clone https://github.com/ncar/lrose-core
```

### Run the ```build_lrose_cmake.py``` script:

To see the usage:

```
  cd ~/git/lrose-core/build/scripts
  ./build_lrose_cmake.py --help
```

```
  Usage: build_lrose_cmake.py [options]
  Options:
    -h, --help         show this help message and exit
    --debug            Set debugging on
    --verbose          Set verbose debugging on
    --package=PACKAGE  Package name. Options are: lrose-core (default), lrose-
                       radx, lrose-cidd, samurai
    --prefix=PREFIX    Prefix name for install location
    --scripts          Install scripts as well as binaries
    --static           use static linking, default is dynamic
    --cmake3           Use cmake3 instead of cmake for samurai
```

`package` defaults to `lrose-core`.

Available packages are:

```
  lrose-core lrose-cidd lrose-radx samurai
```

We recommend building the full core.

For older systems, such as centos7, you will need to use ```--cmake3```.

To build and install the full lrose package into ```~/lrose```:

```
  ./build_lrose_cmake.py
```

To specify the sub-package, e.g. lrose-radx:

```
  ./build_lrose_cmake.py --package lrose-radx
```

To set the install directory:

```
  ./build_lrose_cmake.py --prefix /my/install/dir
```

<a name="build-manually"/>

## 5. Build manually (most user intensive)

### Clone lrose-core

```
  cd ~/git
  git clone https://github.com/ncar/lrose-core
```

### Install the required package makefiles

This step is not needed if you are building the full core.

```
  cd ~/git/lrose-core/build/scripts
  ./installPackageMakefiles.py --help
```

The usage is:

```
  Usage: installPackageMakefiles.py [options]
  Options:
    -h, --help         show this help message and exit
    --debug            Set debugging on
    --package=PACKAGE  Package name.
                       lrose-core (default), lrose-radx, lrose-cidd, samurai
```

Select the package you want with ```--package```.

### Generate the CMakeLists.txt files for cmake

```
  cd ~/git/lrose-core/build/cmake
  ./createCMakeLists.py --help
```

The usage is:

```
  Usage: createCMakeLists.py [options]
  Options:
    -h, --help            show this help message and exit
    --debug               Set debugging on
    --verbose             Set verbose debugging on
    --silent              Set debugging off
    --coreDir=COREDIR     Path of lrose-core top level directory, default is:
                          /data/mdtest/git/lrose-core/build/cmake/../..
    --prefix=PREFIX
                          Path of lrose install dir, default is ~/lrose
    --dependDirs=DEPENDDIRS
                          Comma-delimited list of dirs to be searched as
                          dependencies. Each dir in the list will have include/
                          and lib/ subdirs.
    --static              Create static lib objects. Default is shared
    --renewTemplates      Copy Makefile to __makefile.template for all
                          directories used
    --verboseMake         Verbose output for make, default is summary
    --withJasper          Set if jasper library is installed. This provides
                          support for jpeg compression in grib files.
```

To install in ```~/lrose```:

```
  cd ~/git/lrose-core/build/cmake
  ./createCMakeLists.py
```

To set the install directory:

```
  ./createCMakeLists.py --prefix /my/install/dir
```

### Make build directory and run cmake

```
  cd ~/git/lrose-core/codebase
  mkdir build
  cd build
  cmake ..
```

### Perform the build

Note: we have to build ```tdrp_gen``` before the apps, since it is a dependency.

```
  cd ~/git/lrose-core/codebase/build/libs
  make -j 8 install
  cd ~/git/lrose-core/codebase/build/apps/tdrp/src/tdrp_gen
  make -j 8 install
  cd ~/git/lrose-core/codebase/build/apps
  make -j 8 install
```

<a name="verify"/>

## 6. Verify

Try the commands:
```
  ~/lrose/bin/RadxPrint -h
  ~/lrose/bin/RadxConvert -h
  ~/lrose/bin/Radx2Grid -h
  ~/lrose/bin/HawkEye
```
