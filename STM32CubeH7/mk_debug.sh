mkdir -p Debug
cd Debug
cmake -DCMAKE_TOOLCHAIN_FILE="../arm-cross.cmake" -DCMAKE_BUILD_TYPE=Debug ..
make
cd ..

