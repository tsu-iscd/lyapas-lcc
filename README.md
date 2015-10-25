### Quick and Dirty Install Guide

Assuming that you are in the project directory, type the following.

```bash
$ sudo apt-get install python-pip libgraphviz-dev
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
