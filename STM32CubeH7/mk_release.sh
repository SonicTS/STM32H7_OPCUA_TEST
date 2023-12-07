mkdir -p Release
cd Release
cmake -DCMAKE_TOOLCHAIN_FILE="../arm-cross.cmake" -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
make
cd ..

