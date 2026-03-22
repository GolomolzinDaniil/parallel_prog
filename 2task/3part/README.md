# 3 задание
## 1 вариант
### Сборка
```bash
cd 3part
cmake -B build -S . -DSND_VERSION=OFF
cmake --build build
```
## Запуск
```bash
OMP_NUM_THREADS=N ./build/main <n>
```
## 2 вариант
### Сборка
```bash
cd 3part
cmake -B build -S . -DSND_VERSION=ON
cmake --build build
```
## Запуск
```bash
OMP_NUM_THREADS=N ./build/main <n>
```