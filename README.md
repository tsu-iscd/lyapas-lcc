### Quick and Dirty Install Guide

## Requirements
Project needs g++ 5 for building.

List of packages needs to be installed:
* libjansson-dev
* libdw-dev
* bison
* flex

Assuming that you are in the project directory, type the following.

## Instalation
```bash
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
