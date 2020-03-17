# Game of Life
*Simple Game of Life for Qt5*

## Requirements
- Basic C++ build tools
- Qt5 Development tools

## Compiling on Ubuntu
### Installing the requirements
```
sudo apt install build-essential
sudo apt install qtcreator
sudo apt install qt5-default
```

### Compiling and running tests
```
cd tests
make test
```

### Compiling and running the project
```
cd build
qmake ../GameOfLife.pro
make
./GameOfLife
```

Alternatively you can just open the project file in Qt Creator and use that.
