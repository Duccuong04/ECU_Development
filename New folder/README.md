CÂU HỎI PHỎNG VẤN CAN:
1. CAN là gì?
Trả lời: 1. CAN là tiêu chuẩn bus nối tiếp phát sóng đa chủ để kết nối bộ điều khiển điện tử (ECU).
2. Mạng khu vực điều khiển (CAN hoặc CAN-bus) là một tiêu chuẩn bus xe được thiết kế để cho phép các bộ vi điều khiển và các thiết bị giao tiếp với nhau trong xe mà không cần máy tính chủ.
3. CAN là giao thức dựa trên thông báo, được thiết kế dành riêng cho các ứng dụng ô tô nhưng hiện nay cũng được sử dụng trong các lĩnh vực khác như tự động hóa công nghiệp và thiết bị y tế.
4. Bus Mạng Khu vực Điều khiển (CAN) là một bus không đồng bộ nối tiếp được sử dụng trong các ứng dụng thiết bị đo đạc cho các ngành công nghiệp như ô tô
2. CAN frame có hoạt động không?
Trả lời: SOF –
Trường trọng tài chiếm ưu thế 1 – Mã định danh 11 bit, RTR 1 bit (hoặc) 11 bit, 1SRR, 1IDE, 18 bit,
Trường điều khiển 1RTR – Trường dữ liệu IDE, r0, 4 bit (DLC)
– (0-8) Byte
Trường CRC – 15 bit, Dấu phân cách (lặn 1 bit)
Trường ACK – 1 bit, Dấu phân cách (lặn 1 bit)
EOF – 7 bit lặn
IFS – 3 bit lặn
Các loại khung – Dữ liệu, từ xa, Khung lỗi và Khung quá tải
Các loại khung lỗi – Lỗi ACK, Lỗi Bit, Lỗi nội dung, Lỗi biểu mẫu, Lỗi CRC
Khung lỗi – 0-12 cờ xếp chồng, 8 cờ lặn (Dấu phân cách)
Khung quá tải – 0-12 cờ xếp chồng, 8 cờ lặn (Dấu phân cách)
3. Tại sao CAN lại có 120 ohms ở mỗi đầu?
Trả lời: Để giảm thiểu tham chiếu phản xạ, giảm nhiễu. Để đảm bảo rằng sự phản xạ không gây ra lỗi liên lạc, đường truyền phải được chấm dứt.
4. Tại sao CAN là giao thức hướng thông điệp?
Trả lời: Giao thức CAN là giao thức dựa trên thông báo, không phải giao thức dựa trên địa chỉ. Điều này có nghĩa là các tin nhắn không được truyền từ nút này sang nút khác dựa trên địa chỉ. Được nhúng trong chính thông báo CAN là mức độ ưu tiên và nội dung của dữ liệu được truyền đi. Tất cả các nút trong hệ thống đều nhận được mọi tin nhắn được truyền trên xe buýt (và sẽ xác nhận nếu tin nhắn được nhận đúng cách). Mỗi nút trong hệ thống có quyền quyết định xem tin nhắn nhận được sẽ bị loại bỏ ngay lập tức hay được giữ lại để xử lý. Một tin nhắn có thể được gửi đến một nút cụ thể để nhận hoặc nhiều nút dựa trên cách thiết kế mạng và hệ thống. Ví dụ: cảm biến túi khí ô tô chỉ có thể được kết nối qua CAN với nút bộ định tuyến của hệ thống an toàn. Nút bộ định tuyến này lấy thông tin hệ thống an toàn khác và định tuyến nó đến tất cả các nút khác trên mạng hệ thống an toàn. Sau đó, tất cả các nút khác trên mạng hệ thống an toàn có thể nhận được thông tin cảm biến túi khí mới nhất từ ​​bộ định tuyến cùng lúc, xác nhận xem tin nhắn đã được nhận đúng chưa và quyết định xem nên sử dụng thông tin này hay loại bỏ nó.
5. CÓ THỂ logic những gì nó tuân theo?
Trả lời: Có dây VÀ logic
6. Trọng tài CAN là gì?
Trả lời: Trọng tài CAN không là gì khác ngoài nút đang cố gắng giành quyền kiểm soát trên bus CAN.
7. CAN sẽ tuân theo Trọng tài như thế nào?
Trả lời: CSMA/CD + AMP (Trọng tài về mức độ ưu tiên của tin nhắn)
Hai nút bus đã nhận được yêu cầu truyền. Phương thức truy cập bus là CSMA/CD+AMP (Đa truy cập cảm nhận sóng mang với phát hiện va chạm và phân xử theo mức độ ưu tiên thông báo). Theo thuật toán này, cả hai nút mạng đều đợi cho đến khi xe buýt rảnh (Carrier Sense). Trong trường hợp đó, bus rảnh, cả hai nút đều truyền bit bắt đầu chiếm ưu thế của chúng (Đa truy cập). Mỗi nút bus sẽ đọc lại từng bit từ bus trong toàn bộ thông báo và so sánh giá trị được truyền với giá trị nhận được. Miễn là các bit giống hệt nhau từ cả hai máy phát thì không có gì xảy ra. Lần đầu tiên có sự khác biệt – trong ví dụ này là bit thứ 7 của thông báo – quá trình phân xử diễn ra: Nút A truyền mức ưu thế, nút B truyền mức lặn. Mức lặn sẽ bị ghi đè bởi mức vượt trội. Điều này được nút B phát hiện vì giá trị được truyền không bằng giá trị nhận được (Phát hiện va chạm). Tại thời điểm này, nút B đã mất trọng tài, dừng truyền bất kỳ bit nào nữa ngay lập tức và chuyển sang chế độ nhận, vì thông báo đã thắng trọng tài phải được xử lý bởi nút này (Trọng tài về mức độ ưu tiên của tin nhắn)
Ví dụ: xem xét ba thiết bị CAN, mỗi thiết bị đang cố gắng truyền tin nhắn:
• Thiết bị 1 – địa chỉ 433 (thập phân hoặc 00110110001 nhị phân)
• Thiết bị 2 – địa chỉ 154 (00010011010)
• Thiết bị 3 – địa chỉ 187 (00010111011)
Giả sử cả ba đều thấy bus không hoạt động và bắt đầu truyền cùng lúc, đây là cách phân xử diễn ra. Tất cả ba thiết bị sẽ điều khiển bus đến trạng thái vượt trội cho phần bắt đầu khung (SOF) và hai bit quan trọng nhất của mỗi mã nhận dạng thông báo. Mỗi thiết bị sẽ giám sát xe buýt và xác định thành công. Khi họ viết bit 8 của ID tin nhắn, thiết bị viết tin nhắn ID 433 sẽ thông báo rằng xe buýt đang ở trạng thái chiếm ưu thế khi nó đang cố gắng để nó ở trạng thái lặn, vì vậy nó sẽ cho rằng có xung đột và bỏ cuộc ngay bây giờ. Các thiết bị còn lại sẽ tiếp tục ghi bit cho đến bit thứ 5, khi đó thiết bị ghi tin nhắn ID 187 sẽ nhận thấy xung đột và hủy bỏ quá trình truyền. Điều này để lại thiết bị viết tin nhắn ID 154. Nó sẽ tiếp tục ghi các bit trên bus cho đến khi hoàn thành hoặc phát hiện ra lỗi. Lưu ý rằng phương pháp phân xử này sẽ luôn khiến ID thông báo có giá trị số thấp nhất được ưu tiên. Phương pháp phân xử và ưu tiên theo bit tương tự này cũng áp dụng cho phần mở rộng 18 bit ở định dạng mở rộng.
8. Tốc độ của CAN là bao nhiêu?
Trả lời: 40m @1Mbps và nếu chiều dài cáp tăng sẽ làm giảm tốc độ do RLC trên cáp.
9. Nếu master gửi 764 và Slave gửi 744 thì ai sẽ được phân xử?
Trả lời: Bắt đầu từ MSB, nibble đầu tiên cũng vậy, Master gửi 7, Slave cũng gửi 7 tin nhắn có nhiều bit trội hơn sẽ được phân xử, giá trị nhận dạng tin nhắn thấp nhất có mức độ ưu tiên cao hơn.
10. CAN tiêu chuẩn và CAN mở rộng khác nhau như thế nào?
Trả lời: Số lượng định danh có thể cung cấp cho khung tiêu chuẩn là 2power11.
Số lượng định danh nhiều hơn so với khung cơ sở, đối với khung mở rộng là 2power29.
Bit IDE - 1 cho khung mở rộng.
Bit IDE - 0 cho khung Chuẩn.
11. Bit nhồi là gì?
Trả lời: CAN sử dụng giao thức Non-Return-to-Zero, NRZ-5, với tính năng nhồi bit. Ý tưởng đằng sau việc nhồi bit là cung cấp một biên được đảm bảo trên tín hiệu để bộ thu có thể đồng bộ lại với bộ phát trước khi sự khác biệt nhỏ về xung nhịp giữa hai nút có thể gây ra sự cố. Với NRZ-5, máy phát truyền tối đa năm bit liên tiếp có cùng giá trị. Sau năm bit có cùng giá trị (không hoặc một), bộ phát sẽ chèn một bit thứ có trạng thái ngược lại.
12. Công dụng của việc nhồi bit là gì?
Trả lời: Các thông báo NRZ dài gây ra sự cố ở máy thu
• Đồng hồ trôi có nghĩa là nếu không có cạnh, máy thu sẽ mất dấu bit
• Các cạnh tuần hoàn cho phép máy thu đồng bộ lại với đồng hồ của người gửi
13. Chức năng của bộ thu phát CAN là gì?
Trả lời: Bộ thu phát cung cấp khả năng truyền vi sai cho bus và khả năng nhận vi sai cho bộ điều khiển CAN. Bộ thu phát cung cấp giao diện nâng cao giữa bộ điều khiển giao thức và bus vật lý trong nút Mạng Khu vực Điều khiển (CAN).
Thông thường, mỗi nút trong hệ thống CAN phải có thiết bị chuyển đổi tín hiệu số do bộ điều khiển CAN tạo ra thành tín hiệu phù hợp để truyền qua cáp bus (đầu ra vi sai). Nó cũng cung cấp một vùng đệm giữa bộ điều khiển CAN và các xung điện áp cao có thể được tạo ra trên bus CAN bởi các nguồn bên ngoài (EMI, ESD, quá độ điện, v.v.).
Bộ thu phát lon là thiết bị phát hiện các mức tín hiệu được sử dụng trên bus CAN đến các mức tín hiệu logic được bộ vi điều khiển nhận ra.
14. Chức năng của lớp Data link trong CAN?
Trả lời: LLC (Logical Link Control) – Chức năng kiểm soát quá tải, thông báo, lọc tin nhắn và quản lý Khôi phục.
MAC (Kiểm soát truy cập trung bình) - Đóng gói/hủy đóng gói, phát hiện và kiểm soát lỗi, nhồi và khử nhồi và tuần tự hóa/khử tuần tự hóa.
15. Đồng bộ hóa nghĩa là gì?
Trả lời: Đồng bộ hóa là việc chấm công đòi hỏi sự phối hợp của các sự kiện để vận hành hệ thống một cách đồng bộ.
16. Đồng bộ cứng và đồng bộ mềm có nghĩa là gì?
Trả lời: Đồng bộ hóa cứng được thực hiện ở mọi cạnh từ cạnh lặn đến cạnh trội trong khi Bus nhàn rỗi. Ngoài ra, cần phải có Đồng bộ hóa cứng cho mỗi bit SOF nhận được. Một bit SOF có thể được nhận cả trong khi Bus nhàn rỗi cũng như trong khi Tạm dừng truyền và khi kết thúc Không gian liên khung. Bất kỳ nút nào cũng vô hiệu hóa Đồng bộ hóa cứng nếu nó lấy mẫu một cạnh từ lặn đến chiếm ưu thế hoặc nếu nó bắt đầu gửi bit SOF chiếm ưu thế.
Hai loại đồng bộ hóa được hỗ trợ:
– Đồng bộ hóa cứng được thực hiện với cạnh xuống trên bus trong khi bus ở trạng thái rảnh, được hiểu là Bắt đầu khung (SOF). Nó khởi động lại Logic thời gian bit bên trong.
– Đồng bộ hóa mềm được sử dụng để kéo dài hoặc rút ngắn thời gian bit trong khi nhận được khung CAN.
17. Sự khác biệt giữa chức năng và địa chỉ vật lý là gì?
Trả lời: Địa chỉ chức năng là một sơ đồ đánh địa chỉ gắn nhãn các thông điệp dựa trên mã hoạt động hoặc nội dung của chúng. Địa chỉ vật lý là một sơ đồ đánh địa chỉ gắn nhãn các tin nhắn dựa trên vị trí địa chỉ vật lý của (các) nguồn và/hoặc đích của chúng.
18. Điều gì xảy ra nếu tôi phải gửi nhiều hơn 8 byte dữ liệu?
Trả lời: Tiêu chuẩn J1939 đã xác định phương thức giao tiếp hơn 8 byte dữ liệu bằng cách gửi dữ liệu theo gói như được chỉ định trong Giao thức vận chuyển (TP). Có hai loại TP, một loại để phát dữ liệu và loại kia để gửi dữ liệu đến một địa chỉ cụ thể.
DTC bao gồm 4 thành phần – SPN, FMI, OC và CM.
DTC là sự kết hợp của bốn trường độc lập: Số thông số nghi ngờ (SPN) của kênh hoặc tính năng có thể có lỗi; Mã định danh chế độ lỗi (FMI) của lỗi cụ thể; số lần xuất hiện (OC) của tổ hợp SPN/FMI; và phương thức chuyển đổi SPN (CM) cho chế độ nhận biết cách diễn giải SPN. Cùng với nhau, SPN, FMI, OC và CM tạo thành một con số mà công cụ chẩn đoán có thể sử dụng để hiểu lỗi đang được báo cáo.

Hình 1 – Định dạng tin nhắn
19. KWP2000 là gì?
Trả lời: KWP 2000(ISO14230) là tiêu chuẩn truyền thông chẩn đoán. Chỉ định các cấu hình hệ thống có thể sử dụng dòng K & L. Như 9141-2 nhưng bị giới hạn ở các đặc tính vật lý. Chỉ định các cấu hình hệ thống có thể sử dụng dòng K & L.
 5 Baud thức dậy là 9141- 2
 Phương pháp khởi tạo nhanh mới

20.OBDII là gì?
Trả lời: Chẩn đoán trên xe trong bối cảnh ô tô là một thuật ngữ chung đề cập đến khả năng tự chẩn đoán và báo cáo của xe

21. Tại sao lại là Tiêu chuẩn Chẩn đoán?
Trả lời: Khi các hệ thống trở nên phức tạp hơn, mối liên hệ giữa nguyên nhân và triệu chứng trở nên ít rõ ràng hơn. Điều này có nghĩa là các hệ thống điện tử phải có khả năng tự chẩn đoán ở một mức độ nào đó và liên lạc với thế giới bên ngoài. Ban đầu, nhiều hệ thống sử dụng các giao thức riêng, điều đó có nghĩa là các gara phải có một số lượng lớn công cụ – thậm chí để chẩn đoán một chiếc xe.

23. Xác minh và xác nhận nghĩa là gì??
Trả lời: Xác minh và Xác thực (V&V) là quá trình kiểm tra xem hệ thống phần mềm có đáp ứng các thông số kỹ thuật hay không và nó có đáp ứng được mục đích đã định hay không. Nó thường là một phần của quá trình kiểm thử phần mềm của một dự án.
Theo Mô hình trưởng thành khả năng (CMMI-SW v1.1),
 Xác minh: Quá trình đánh giá phần mềm để xác định xem các sản phẩm của một giai đoạn phát triển nhất định có đáp ứng các điều kiện đặt ra khi bắt đầu giai đoạn đó hay không.
 Xác nhận: Quá trình đánh giá phần mềm trong hoặc ở cuối quá trình phát triển để xác định xem nó có đáp ứng các yêu cầu cụ thể hay không.
 Việc xác minh cho thấy sự phù hợp với đặc điểm kỹ thuật; xác nhận cho thấy chương trình đáp ứng nhu cầu của khách hàng

25. Bạn có thể có hai bộ phát sử dụng cùng một trường tiêu đề chính xác không?
Trả lời: Không – điều đó sẽ tạo ra xung đột bus
• Trừ khi bạn có phần mềm trung gian đảm bảo chỉ một nút có thể truyền tại một thời điểm
– Ví dụ: sử dụng thông báo có mức độ ưu tiên thấp làm mã thông báo để mô phỏng việc chuyển mã thông báo
26. Mức điện áp lớp vật lý CAN

27. Định thời bit CAN:
Theo đặc tả CAN, thời gian bit được chia thành bốn đoạn. Phân đoạn đồng bộ hóa, Phân đoạn thời gian truyền, Phân đoạn đệm pha 1 và Phân đoạn đệm pha 2. Mỗi phân đoạn bao gồm một số lượng tử thời gian cụ thể, có thể lập trình được (xem Bảng 1). Độ dài của lượng tử thời gian (tq), là đơn vị thời gian cơ bản của thời gian bit, được xác định bởi đồng hồ hệ thống fsys của bộ điều khiển CAN và Bộ chia tỷ lệ trước tốc độ Baud (BRP): tq = BRP / fsys. Đồng hồ hệ thống điển hình là: fsys = fosc hoặc fsys = fosc/2.
Phân đoạn đồng bộ hóa Sync_Seg là một phần thời gian bit mà các cạnh của mức bus CAN dự kiến ​​sẽ xảy ra; khoảng cách giữa một cạnh xảy ra bên ngoài Sync_Seg và Sync_Seg được gọi là lỗi pha của cạnh đó. Phân đoạn thời gian lan truyền Prop_Seg nhằm mục đích bù đắp cho thời gian trễ vật lý trong mạng CAN. Các phân đoạn đệm pha Phase_Seg1 và Phase_Seg2 bao quanh Điểm mẫu. Độ rộng bước nhảy đồng bộ hóa (Re-) (SJW) xác định khoảng cách quá trình đồng bộ hóa lại có thể di chuyển Điểm mẫu trong giới hạn được xác định bởi Phân đoạn bộ đệm pha để bù cho các lỗi pha biên.

Có hai loại đồng bộ hóa: Đồng bộ hóa cứng và Đồng bộ hóa lại. Đồng bộ hóa cứng được thực hiện một lần khi bắt đầu khung; bên trong một khung chỉ xảy ra quá trình đồng bộ lại.
• Đồng bộ hóa cứng Sau khi đồng bộ hóa cứng, thời gian bit được khởi động lại khi kết thúc Sync_Seg, bất kể lỗi pha biên. Do đó, đồng bộ hóa cứng buộc biên khiến cho đồng bộ hóa cứng nằm trong phân đoạn đồng bộ hóa của thời gian bit được khởi động lại.
• Tái đồng bộ hóa bit Tái đồng bộ hóa dẫn đến việc rút ngắn hoặc kéo dài thời gian bit sao cho vị trí của điểm mẫu bị dịch chuyển so với cạnh.

26. Công thức tính Baudrate?
Tốc độ baud được tính như sau:
tốc độ baud (bit trên giây) = 18,432 x 10^6 / BRP / (1 + TSEG1 + TSEG2)

27. Điều gì xảy ra khi hai nút CAN gửi cùng một mã định danh cùng một lúc?
Hai nút trên mạng không được phép gửi tin nhắn có cùng id. Nếu hai nút cố gắng gửi một tin nhắn có cùng id vào cùng thời điểm thì việc phân xử sẽ không hoạt động. Thay vào đó, một trong các nút truyền sẽ phát hiện ra rằng thông điệp của anh ta bị biến dạng bên ngoài trường trọng tài. Sau đó, các nút sẽ sử dụng cách xử lý lỗi của CAN, trong trường hợp này cuối cùng sẽ dẫn đến việc một trong các nút truyền bị tắt (chế độ tắt bus).

29. Sự khác biệt giữa Tốc độ bit và Tốc độ Baud là gì?
Sự khác biệt giữa tốc độ Bit và Baud rất phức tạp và đan xen. Cả hai đều phụ thuộc và liên quan đến nhau. Nhưng cách giải thích đơn giản nhất là Tốc độ bit là số lượng bit dữ liệu được truyền đi mỗi giây. Tốc độ baud là số lần mỗi giây tín hiệu trong kênh liên lạc thay đổi. Tốc độ bit đo số bit dữ liệu (tức là 0 và 1) được truyền trong một giây trong kênh liên lạc. Con số 2400 bit mỗi giây có nghĩa là 2400 số 0 hoặc số 1 có thể được truyền trong một giây, do đó có tên viết tắt là “bps”. Các ký tự riêng lẻ (ví dụ như chữ cái hoặc số) còn được gọi là byte bao gồm nhiều bit. Tốc độ truyền là số lần tín hiệu trong kênh liên lạc thay đổi trạng thái hoặc thay đổi. Ví dụ: tốc độ 2400 baud có nghĩa là kênh có thể thay đổi trạng thái lên tới 2400 lần mỗi giây. Thuật ngữ “trạng thái thay đổi” có nghĩa là nó có thể thay đổi từ 0 thành 1 hoặc từ 1 đến 0 cho đến X (trong trường hợp này là 2400) lần mỗi giây. Nó cũng đề cập đến trạng thái thực tế của kết nối, chẳng hạn như điện áp, tần số hoặc mức pha). Sự khác biệt chính giữa hai trạng thái này là một thay đổi trạng thái có thể truyền một bit, hoặc nhiều hơn hoặc ít hơn một bit, điều đó tùy thuộc vào về kỹ thuật điều chế được sử dụng. Vì vậy, tốc độ bit (bps) và tốc độ baud (baud mỗi giây) có kết nối như sau: bps = baud mỗi giây x số bit trên mỗi baudKỹ thuật điều chế xác định số lượng bit trên mỗi baud. Dưới đây là hai ví dụ: Khi sử dụng FSK (Khóa dịch tần số, một kỹ thuật truyền), mỗi baud sẽ truyền một bit. Chỉ cần một thay đổi về trạng thái để gửi một bit. Do đó, tốc độ bps của modem bằng tốc độ baud. Khi sử dụng tốc độ baud 2400, một kỹ thuật điều chế gọi là điều chế pha truyền bốn bit trên mỗi baud được sử dụng. Vì vậy: 2400 baud x 4 bit mỗi baud = 9600 bpsNhững modem như vậy có khả năng hoạt động ở tốc độ 9600 bps.
