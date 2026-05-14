cmake -B build
cmake --build build
Запуск программы
./build/task input.txt 
Запуск тестов
cd build
ctest --output-on-failure
