# для float
```bash
rm -rf build && mkdir build && cd build && cmake .. && make && ./main && cd ..
```
### пример выхода: 
```terminal
arr_type = float, sum = -0.213894
```
# для double
```bash
rm -rf build && mkdir build && cd build && cmake -DUSE_DOUBLE=ON .. && make && ./main && cd ..
```
### пример выхода: 
```
arr_type = double, sum = -6.76916e-10
```
