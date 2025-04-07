- Cấu hình GPIO của I2C ở chế độ open drain

**open drain**

Là chế độ chân output chỉ có thể kéo xuống mức thấp (0).

Khi muốn "thả" mức cao (1), nó không tự kéo lên mà sẽ để chân ở trạng thái "hở" (không tác động gì).

Mức cao (1) được duy trì nhờ điện trở kéo lên (pull-up resistor) gắn bên ngoài.

**Tại sao?**

- Để nhiều thiết bị chia sẻ bus mà không bị xung đột

- **R pull - up** .Giá trị thường dùng: 4.7kΩ đến 10kΩ, tùy thuộc vào tốc độ I2C và số lượng thiết bị.

**Đường bus SDA được Master và Slave chia sẻ/ dùng chung với nhau** theo cơ chế AND dữ liệu, Master kiểm soát đường dây. Khi đưa đường SDA lên 1 -> Slave truyền dữ liệu

# Software

## 1. Cấu hình GPIO, define macro

```c
#define I2C_SCL		GPIO_Pin_6
#define I2C_SDA		GPIO_Pin_7

#define I2C_GPIO  GPIOB

#define WRITE_SDA_0 GPIO_ResetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SDA_1 GPIO_SetBits(I2C_GPIO, I2C_SDA)
#define WRITE_SCL_0 GPIO_ResetBits(I2C_GPIO, I2C_SCL)
#define WRITE_SCL_1 GPIO_SetBits(I2C_GPIO, I2C_SCL)
#define READ_SDA_VAL 	GPIO_ReadInputDataBit(I2C_GPIO, I2C_SDA)

typedef enum {
	NOT_OK, OK
} status;

typedef enum{
	NACK, ACK
} ACK_Bit;
```

## 2. Truyền nhận dữ liệu

Master Phát Địa Chỉ Slave: Sau khi Start Condition, Master gửi địa chỉ 7 bit của Slave lên bus kèm theo một bit đọc/ghi (R/W).

- Nếu bit R/W là 0, Master yêu cầu ghi (truyền dữ liệu tới Slave).

- Nếu bit R/W là 1, Master yêu cầu đọc (đọc dữ liệu từ 1 thanh ghi tron Slave).

Acknowledge (ACK): Mỗi khi một byte (8 bit) được truyền đi, thiết bị Slave sẽ phản hồi bằng cách kéo đường SDA xuống thấp trong 1 bit thời gian để cho biết nó đã nhận được byte. Đây được gọi là bit ACK.

- Giao tiếp

**Write data**: Master gửi từng byte đến Slave, và Slave sẽ phản hồi lại bằng một bit ACK sau mỗi byte.

**Read data**: Master đọc từng byte từ Slave, và sau mỗi byte, Master phát lại bit ACK để báo rằng dữ liệu đã được nhận. (có hoặc ko).

Kết thúc Giao tiếp: Khi quá trình truyền dữ liệu hoàn tất, Master phát Stop Condition để ngắt kết nối với Slave.

- **Start & Stop condition**
```c
void I2C_Start()
{
	WRITE_SDA_0; // SDA reset 0 before SCL
	delay_us(3);
	WRITE_SCL_0;
	delay_us(3);
}

void I2C_Stop()
{
	WRITE_SCL_1; // SCL set 1 before SDA
	delay_us(3);
	WRITE_SDA_1;
	delay_us(3);
}
```

- **Write data & Read data**

```c
status I2C_Master_Write(uint8_t u8Data) // Send Address/ Data to Slave, Slave receive -> send ACK
{
	status stRet;
	for(int i = 0; i < 8; i++)
	{
		if(u8Data & 0x80)
		{
			WRITE_SDA_1;
		}
		else
		{
			WRITE_SDA_0;
		}
		delay_us(3);
		
		// Clock
		WRITE_SCL_1;
		delay_us(5);
		WRITE_SCL_0;
		delay_us(5);
		
		// Shift 1 bit to the left
		u8Data <<= 1;
	}
	
	WRITE_SDA_1;        // Set SDA = 1 to read ACK from Slave (if Slave send ACK, SDA = 0)
	delay_us(3);
	WRITE_SCL_1;
	delay_us(3);
	
	if(READ_SDA_VAL)    // SDA = 0 -> ACK, SDA = 1 -> NACK
	{ 
		stRet = NOT_OK;
	}
	else
	{
		stRet = OK;
	}
	
	delay_us(2);        // Clock SCK = 5uS -> Read at the middle to avoid bouncing
	WRITE_SCL_0;
	delay_us(5);
	
	return stRet;
}
	
uint8_t I2C_Master_Read	(ACK_Bit _ACK)
{
	uint8_t i;
	uint8_t u8Ret = 0x00;
	WRITE_SDA_1;
	delay_us(3);
	for(i = 0; i < 8; ++i)
	{
		u8Ret <<= 1;
		delay_us(3);
		if(READ_SDA_VAL)  // read register of Slave on SDA bus
		{
			u8Ret |= 0x01;
		}
		delay_us(2);
		WRITE_SCL_0;
		delay_us(5);
	}
	if(_ACK)
	{
		WRITE_SDA_0;
	}
	else
	{
		WRITE_SDA_1;
	}
	delay_us(3);
	
	WRITE_SCL_1;
	delay_us(5);
	WRITE_SCL_0;
	delay_us(5);

	return u8Ret;
}

```

## Hardware

