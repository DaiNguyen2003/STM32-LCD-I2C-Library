# STM32 LCD I2C Library

Thư viện giao tiếp LCD1602 qua I2C cho STM32 sử dụng module PCF8574.

---

## 📚 Giới thiệu về I2C

### I2C là gì?
**I2C (Inter-Integrated Circuit)** là giao thức truyền thông nối tiếp đồng bộ, sử dụng 2 dây:
- **SCL (Serial Clock)**: Xung clock đồng bộ
- **SDA (Serial Data)**: Đường truyền dữ liệu

### Đặc điểm của I2C:
| Thông số | Giá trị |
|----------|---------|
| Số dây | 2 (SCL + SDA) |
| Tốc độ | 100kHz (Standard), 400kHz (Fast) |
| Địa chỉ | 7-bit hoặc 10-bit |
| Topology | Multi-master, Multi-slave |

### Ưu điểm khi dùng I2C với LCD:
- Chỉ cần **2 chân GPIO** thay vì 6-8 chân
- Dễ dàng kết nối nhiều thiết bị trên cùng bus
- Tiết kiệm chân I/O cho STM32

---

## 🖥️ LCD1602 là gì?

**LCD1602** là màn hình LCD ký tự với:
- **16 cột x 2 hàng** = 32 ký tự
- Hỗ trợ hiển thị chữ, số, ký tự đặc biệt
- Có đèn nền (backlight)

### Module I2C PCF8574
Module này chuyển đổi giao tiếp song song của LCD thành I2C:
- Địa chỉ mặc định: `0x27` hoặc `0x3F`
- Có biến trở điều chỉnh độ tương phản
- Jumper điều khiển backlight

---

## 🔌 Sơ đồ kết nối

### Kết nối với STM32F103C8T6 (Blue Pill)

```
┌─────────────────┐                    ┌─────────────────┐
│  STM32F103C8T6  │                    │  LCD1602 + I2C  │
│   (Blue Pill)   │                    │   (PCF8574)     │
├─────────────────┤                    ├─────────────────┤
│                 │                    │                 │
│  3.3V     ──────┼────────────────────┼──── VCC         │
│                 │                    │                 │
│  GND      ──────┼────────────────────┼──── GND         │
│                 │                    │                 │
│  PB6 (SCL)──────┼────────────────────┼──── SCL         │
│                 │                    │                 │
│  PB7 (SDA)──────┼────────────────────┼──── SDA         │
│                 │                    │                 │
└─────────────────┘                    └─────────────────┘
```

### Bảng kết nối

| STM32 Pin | LCD I2C Module | Chức năng |
|-----------|----------------|-----------|
| 3.3V | VCC | Nguồn |
| GND | GND | Mass |
| PB6 | SCL | I2C Clock |
| PB7 | SDA | I2C Data |

> ⚠️ **Lưu ý**: Một số module cần 5V. Nếu LCD không hiển thị rõ, thử đổi nguồn VCC.

---

## ⚙️ Cấu hình STM32CubeIDE

### Bước 1: Bật I2C1
1. Mở file `.ioc` trong STM32CubeIDE
2. Vào **Connectivity → I2C1**
3. Chọn Mode: **I2C**

### Bước 2: Cấu hình thông số
```
I2C1 Configuration:
├── I2C Speed Mode: Standard Mode
├── I2C Clock Speed: 100000 Hz
├── Clock No Stretch Mode: Disabled
├── Primary Address Length: 7-bit
└── Dual Address Acknowledged: Disabled
```

### Bước 3: Kiểm tra chân
- **PB6** → I2C1_SCL
- **PB7** → I2C1_SDA

### Bước 4: Generate Code
Nhấn **Ctrl+S** để save và generate code.

---

## 📁 Cấu trúc thư viện

```
LCD/
├── Core/
│   ├── Inc/
│   │   └── lcd_i2c.h      # Header file
│   └── Src/
│       ├── lcd_i2c.c      # Implementation
│       └── main.c         # Demo code
```

---

## 🔧 Các hàm chính

### 1. Khởi tạo LCD
```c
void LCD_Init(I2C_HandleTypeDef *hi2c);
```
Khởi tạo LCD với I2C handle. Gọi sau khi `MX_I2C1_Init()`.

**Ví dụ:**
```c
LCD_Init(&hi2c1);
```

---

### 2. Xóa màn hình
```c
void LCD_Clear(void);
```
Xóa toàn bộ nội dung và đưa con trỏ về vị trí (0,0).

**Ví dụ:**
```c
LCD_Clear();
```

---

### 3. Đặt vị trí con trỏ
```c
void LCD_SetCursor(uint8_t row, uint8_t col);
```
| Tham số | Giá trị | Mô tả |
|---------|---------|-------|
| row | 0 hoặc 1 | Hàng (0 = trên, 1 = dưới) |
| col | 0-15 | Cột (0-15) |

**Ví dụ:**
```c
LCD_SetCursor(0, 0);  // Góc trên trái
LCD_SetCursor(1, 5);  // Hàng 2, cột 6
```

---

### 4. Gửi chuỗi ký tự
```c
void LCD_SendString(char *str);
```
Hiển thị chuỗi ký tự tại vị trí con trỏ hiện tại.

**Ví dụ:**
```c
LCD_SendString("Hello World!");
```

---

### 5. Gửi một ký tự
```c
void LCD_SendChar(char ch);
```
Hiển thị một ký tự đơn.

**Ví dụ:**
```c
LCD_SendChar('A');
```

---

### 6. Điều khiển đèn nền
```c
void LCD_Backlight(uint8_t state);
```
| Tham số | Giá trị |
|---------|---------|
| state = 1 | Bật đèn |
| state = 0 | Tắt đèn |

**Ví dụ:**
```c
LCD_Backlight(1);  // Bật
LCD_Backlight(0);  // Tắt
```

---

## 📝 Ví dụ sử dụng

### Ví dụ 1: Hiển thị text cố định
```c
#include "lcd_i2c.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    
    // Khởi tạo LCD
    LCD_Init(&hi2c1);
    
    // Hiển thị text
    LCD_SetCursor(0, 0);
    LCD_SendString("Hello World!");
    
    LCD_SetCursor(1, 0);
    LCD_SendString("STM32 + LCD");
    
    while (1) { }
}
```

### Ví dụ 2: Hiển thị số tăng dần
```c
#include "lcd_i2c.h"
#include <stdio.h>

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    
    LCD_Init(&hi2c1);
    
    LCD_SetCursor(0, 0);
    LCD_SendString("Counter:");
    
    uint32_t count = 0;
    char buffer[16];
    
    while (1)
    {
        sprintf(buffer, "%lu    ", count);
        LCD_SetCursor(1, 0);
        LCD_SendString(buffer);
        
        count++;
        HAL_Delay(500);
    }
}
```

---

## ❓ Xử lý sự cố

| Vấn đề | Nguyên nhân | Giải pháp |
|--------|-------------|-----------|
| LCD không sáng | Nguồn không đủ | Kiểm tra kết nối VCC/GND |
| LCD sáng nhưng không có chữ | Độ tương phản thấp | Xoay biến trở trên module |
| Chữ hiển thị sai | Sai địa chỉ I2C | Thử đổi `0x27` ↔ `0x3F` |
| I2C không hoạt động | Sai chân kết nối | Kiểm tra PB6/PB7 |

### Thay đổi địa chỉ I2C
Mở file `lcd_i2c.h` và sửa:
```c
#define LCD_I2C_ADDR    (0x27 << 1)  // Đổi 0x27 thành 0x3F nếu cần
```

---

## 📄 License

MIT License - Tự do sử dụng và chỉnh sửa.

---

## 👤 Tác giả

**DaiNguyen2003**
- GitHub: [@DaiNguyen2003](https://github.com/DaiNguyen2003)
