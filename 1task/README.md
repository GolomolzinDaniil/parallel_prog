# для float
```bash
rm -rf build && cmake -B build -S . && cmake --build build && ./build/main
```
### пример выхода: 
```
arr_type = float, sum = -0.213894
```
# для double
```bash
rm -rf build && cmake -DUSE_DOUBLE=ON -B build -S . && cmake --build build && ./build/main
```
### пример выхода: 
```
arr_type = double, sum = -6.76916e-10
```
