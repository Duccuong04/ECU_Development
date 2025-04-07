# UART

## Software

**Baudrate: số đơn vị tín hiệu truyền được trong 1 giây**, ở đây 1 đơn vị tín hiệu có 1 bit nên tốc độ baud = tốc độ bit

Giả sử baudrate = 9600 -> gửi 9600 bit trong 1.10^6 us 

Suy ra **khoảng thời gian truyền của 1 bit** là 0 đến 104us

### Hàm truyền

- Tạo Start bằng cách set TX = 0 để thoát idle status -> bắt đầu truyền

- Tạo thời gian để truyền 1 bit (vd 104us), dịch 1 bit để truyền bit tiếp theo

- Truyền xong 8 bit, tạo stop (delay tương ứng với số bit stop)

### Hàm nhận

- Chờ tín hiệu start

- Delay 1.5 period -> đọc dữ liệu ở đoạn giữa để tránh bouncing

- Đọc data trên RX

- Dịch 1 bit

- Delay 1 period time và đợi stop bit