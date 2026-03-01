## Сборка
```bash
rm -rf build/ && cmake -B build -S . && cmake --build build
```
N - кол-во используемых потоков [1,2,4,7,8,16,20,40]

## 1 задание
```bash
OMP_NUM_THREADS=N ./build/1main
```

## 2 задание
```bash
OMP_NUM_THREADS=N ./build/2main
```

## 3 задание (2 варианта в разных файлах)
```bash
OMP_NUM_THREADS=N ./build/3_1main
OMP_NUM_THREADS=N ./build/3_1main
```
