# Thực hành SPI

- Vi điều khiển có tích hợp module SPI, điều khiển bởi các thanh ghi, phần cứng, IO khác nhau -> **Hardware SPI** -> bản chất SPI giống nhau trên mỗi chip lại được **cài đặt và sử dụng không giống nhau** ->  Khó khăn cho người sử dụng khi bắt đầu tìm hiểu vi điều khiển mới vì các chân MISO, MOSI, SS, SCK đặt khác nhau.

- Để khắc phục, **lập trình giả lập** hardware SPI (**Software SPI**), tạo ra một giao thức truyền thông giống SPI nhưng chỉ sử dụng các cổng GPIO của vi điều khiển -> có thể sử dụng trên bất kì vi điều khiển nào

