# Babel

SIP based VOIP

### How to install

```
mkdir build
cd build
conan install ..
cmake ..
cmake -build
```
or
```
mkdir build && cd build && conan install .. && cmake .. && cmake -build
```

After that, you can copy default-server.cfg file in build directory, rename it to server.cfg and edit default variables.

### Authors

* Alexandre Fourcat
* Thomas Nicollet
* Killian Perrier
* Quentin Liardeaux
