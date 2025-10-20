✅ 1. Cài đặt CMake

Yêu cầu: >= 3.15 (như dòng cmake_minimum_required)

✅ Cài đặt:

Windows: Dùng CMake Installer

Linux: sudo apt install cmake

macOS: brew install cmake

✅ 2. Trình biên dịch C++ hỗ trợ C++17

Yêu cầu trình biên dịch hỗ trợ C++17:

Windows: GCC (MinGW-w64 hoặc MSYS2), hoặc MSVC

Linux: g++

macOS: clang++

✅ 3. Cài đặt SDL2 và các thư viện phụ thuộc

Dự án sử dụng các thư viện:

SDL2

SDL2main

SDL2_image

SDL2_ttf

SDL2_mixer

🔧 Cách cài đặt trên từng hệ điều hành:
💻 Windows (dùng MSYS2 - theo cấu trúc của bạn)

Bước 1: Cài MSYS2

Tải tại: https://www.msys2.org

Bước 2: Cài gói cần thiết

pacman -Syu # cập nhật
pacman -S mingw-w64-ucrt-x86_64-gcc cmake make
pacman -S mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-SDL2_image \
 mingw-w64-ucrt-x86_64-SDL2_ttf mingw-w64-ucrt-x86_64-SDL2_mixer

Bước 3: Mở terminal MSYS2 UCRT64 để build

mkdir build && cd build
cmake ..
make

Bước 4: Chạy chương trình

File TowerDefense.exe sẽ được tạo ra và chạy được nếu các DLL đã được copy vào thư mục chứa file .exe (như dòng add_custom_command của bạn).

🐧 Linux (Ubuntu, Debian...)
sudo apt install build-essential cmake libsdl2-dev libsdl2-image-dev \
 libsdl2-ttf-dev libsdl2-mixer-dev

Sau đó:

mkdir build && cd build
cmake ..
make
./TowerDefense

🍏 macOS (dùng Homebrew)
brew install cmake sdl2 sdl2_image sdl2_ttf sdl2_mixer

Build:

mkdir build && cd build
cmake ..
make
./TowerDefense

✅ 4. Các file DLL phải có để chạy trên Windows

Ngay cả khi build xong .exe, máy người dùng Windows cần có các file sau nằm cùng thư mục .exe:

SDL2.dll

SDL2_image.dll

SDL2_ttf.dll

SDL2_mixer.dll

👉 Bạn đã copy tự động bằng lệnh add_custom_command sau khi build – rất tốt. Tuy nhiên, người dùng cũng phải có các file .dll này sẵn trong root dự án, hoặc bạn cần cung cấp kèm trong repo.

✅ 5. Tổng kết: Máy người khác cần có gì để chạy được?
Thành phần Ghi chú
✅ CMake >= 3.15 Biên dịch project
✅ Trình biên dịch C++17 MSYS2 hoặc GCC/Clang/Visual Studio
✅ Các thư viện SDL2 (dev & runtime) SDL2, SDL2_image, SDL2_ttf, SDL2_mixer
✅ Các file .dll nếu chạy trên Windows Nằm cùng thư mục .exe
✅ Có thể dùng MSYS2 để build dễ dàng Đã phù hợp với CMake script của bạn
