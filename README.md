- nhớ là để chạy dc thì hãy để terminal ở cmd (mở chế độ khác có thể nó sẽ lỗi)
  //Để có thể chạy dc thì bắt buộc phải tải cmake và nhớ hãy khai báo nó trong "edit enviroment variables"

//Cài đặt các thư viện cần thiết

- Mở MSYS2 MINGW64
  pacman -S mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-SDL2_image mingw-w64-ucrt-x86_64-SDL2_ttf mingw-w64-ucrt-x86_64-SDL2_mixer mingw-w64-ucrt-x86_64-SDL2_gfx

//Cách chạy
git clone https://github.com/ZHANG123DZ/Tower_Defense.git
rm -rf .git && git init && git add . && git commit -m "Initial commit" : lệnh này xóa bỏ mqh với repo cũ
Sau đó nhớ xóa cái thư mục build cũ đi để mà build lại, những lần sau chỉnh sửa code thì chỉ cần gõ
cmake --build .

//đẩy lên một repo mới
copy từ github cho nhanh (chỗ nhiều dòng nhất)

//Cách commit code lên github
git add .
git commit -m "nội dung muốn commit"
git push

//Cách build lại
mkdir build
cd build
cmake -G "MinGW Makefiles" .. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
cmake --build .

//Khi chỉnh sửa lại code
Các file mới có đuôi cpp nhớ thêm vào mục chứa các cpp để cmake load dc ko thì nó sẽ lỗi

//Quy tắc đặt tên ô
0 : Đường đi
1 : Chỗ đặt thành
2 : Tường (hoặc chướng ngại vật)

//Quy tắc tạo bản đồ
1 ô: 128px\*128px
12 ô ngang
6 ô dọc
Mỗi Tile là 4 ô nhỏ 32px kết hợp lại (m có thể phóng to sao cho đạt)
