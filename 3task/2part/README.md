# Parallel Task Server (Task 3)

Реализация многопоточного сервера задач на C++20 с поддержкой функций произвольной арности.

## Сборка и запуск
```bash
rm -rf build && cmake -DUSE_DOUBLE=ON -B build -S . && cmake --build build && ./build/main