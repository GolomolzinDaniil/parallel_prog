# Сборка
N - кол-во используемых потоков [1,2,4,7,8,16,20,40]
## 1 задание
```bash
cd 1part
cmake -B build -S .
cmake --build build
OMP_NUM_THREADS=N ./build/main <n> <m>
```
## 2 задание
```bash
cd 2part
cmake -B build -S .
cmake --build build
OMP_NUM_THREADS=N ./build/main <nstep>
```
## 3 задание
### 1 вариант
```bash
cd 3part
cmake -B build -S . -DSND_VERSION=OFF
cmake --build build
OMP_NUM_THREADS=N ./build/main <n>
```
### 2 вариант
```bash
cd 3part
cmake -B build -S . -DSND_VERSION=ON
cmake --build build
OMP_NUM_THREADS=N ./build/main <n>
```