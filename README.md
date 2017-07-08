### Detailed Install Guide
[Курс молодого бойца](https://github.com/tsu-iscd/lyapas-lcc/wiki/Курс-молодого-бойца.)

### Quick and Dirty Install Guide

## Requirements
Project requires g++ 5 and cmake 2.8.8 for building.

List of packages needs to be installed:
* libjansson-dev
* libdw-dev
* bison
* flex

Assuming that you are in the project directory, type the following.

## Instalation
```bash
git submodule update --recursive --init
mkdir build
cd build
cmake ..
make
```
