@echo off

echo Building C++ and Rust...
set "PATH=%PATH%;C:\Program Files\LLVM\bin"
cmake -S . -B build -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build build --config RelWithDebInfo -j 8
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)

echo Building Rust lib...
cd testmod
cargo build --release
if %errorlevel% neq 0 (
    echo Rust build failed!
    cd ..
    pause
    exit /b 1
)
cd ..

echo Building testmod...
cmake -S testmod -B testmod/build -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build testmod/build --config RelWithDebInfo -j 8
if %errorlevel% neq 0 (
    echo Testmod build failed!
    pause
    exit /b 1
)

echo Build successful!
pause