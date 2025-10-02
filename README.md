mkdir build
cd build
cmake -G "MinGW Makefiles" .. -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
cmake --build .

//Quy tắc đặt tên ô
0 : Đường đi
1 : Chỗ đặt thành
2 : Tường (hoặc chướng ngại vật)

//Quy tắc tạo bản đồ
1 ô: 128px\*128px
12 ô ngang
6 ô dọc
Mỗi Tile là 4 ô nhỏ 32px kết hợp lại (m có thể phóng to sao cho đạt)
