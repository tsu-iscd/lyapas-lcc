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

```bash
$ sudo apt-get install python-pip libgraphviz-dev build-essential
$ sudo pip install virtualenv
$ virtualenv ../lcc-env
$ . ../lcc-env/bin/activate
(lcc-env)$ pip install -r requirements.txt
```

### Running Tests

```bash
(lcc-env)$ make test-ex1
(lcc-env)$ make test-ex2
(lcc-env)$ make test-ex3
(lcc-env)$ make test-ex4
```
