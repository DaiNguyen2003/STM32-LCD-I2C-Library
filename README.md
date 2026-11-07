# STM32 LCD I²C · Hiển thị bộ đếm trên LCD1602

## Tổng quan

Thư viện C điều khiển **LCD1602 tương thích HD44780** qua backpack **PCF8574**, với demo đếm số trên STM32F103C8T6. Dữ liệu đi qua I²C1 ở 100 kHz, sau đó backpack điều khiển LCD ở chế độ 4 bit.

## Phần cứng

![Sơ đồ đấu nối I2C1, LCD1602 và nguồn theo module](docs/images/lcd-i2c-wiring.svg)

| Blue Pill | Backpack | Vai trò |
| --- | --- | --- |
| PB6 | SCL | Xung I²C1, open-drain |
| PB7 | SDA | Dữ liệu I²C1, open-drain |
| GND | GND | Mass chung |
| Nguồn theo module | VCC | Theo yêu cầu của LCD và backpack |

Nếu dùng module 5 V, dùng bộ dịch mức I²C hai chiều với pull-up phù hợp ở phía 3,3 V và phía 5 V. Một backpack có thể kéo SDA/SCL lên chính VCC. Đối chiếu điện áp và pin map trước khi cấp nguồn. [Datasheet PCF8574](https://www.ti.com/lit/ds/symlink/pcf8574.pdf)

## Build và nạp firmware

Cần **Python 3.9+**, **GNU Arm Embedded** (`arm-none-eabi-gcc`, `objcopy`, `size`) và gói **STM32CubeF1 v1.8.7**. Script dùng HAL và startup từ gói ST, cùng linker script 64 KiB flash / 20 KiB SRAM của STM32F103C8.

```sh
python build.py
```

Đặt GNU Arm Embedded vào `PATH`, hoặc chỉ định thư mục `bin` của toolchain. Nếu gói HAL nằm ngoài thư mục mặc định `~/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.7`, truyền đường dẫn tới thư mục chứa `Drivers`.

```sh
python build.py --sdk "<STM32CubeF1 root>" --toolchain "<GNU Arm bin>"
```

Build tạo `build/firmware.elf`, `build/firmware.hex` và `build/firmware.bin`. Nạp `.hex` bằng STM32CubeProgrammer qua ST-LINK/SWD, rồi reset board. Có thể dùng `.elf` để nạp symbol và debug. Firmware mẫu dùng HSE 8 MHz và SYSCLK 72 MHz.

Tài liệu công cụ: [STM32CubeF1](https://github.com/STMicroelectronics/STM32CubeF1) · [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html).

## Chạy thử

Sau khi nạp firmware, hàng đầu hiển thị `Counter:`; hàng thứ hai bắt đầu từ 0 và tăng sau mỗi khoảng nửa giây. Nếu đèn nền sáng nhưng chữ không hiện, chỉnh biến trở tương phản và kiểm tra địa chỉ 7 bit của backpack.

## Cấu trúc dự án

| Đường dẫn | Vai trò |
| --- | --- |
| [`Core/Inc/`](Core/Inc/) | Header ứng dụng và thư viện. |
| [`Core/Src/`](Core/Src/) | Demo bộ đếm và thư viện LCD1602 qua PCF8574. |
| [`firmware/`](firmware/) | Khởi tạo phần cứng, SysTick và linker script. |
| [`docs/`](docs/) | Hướng dẫn kỹ thuật và sơ đồ SVG trong `docs/images/`. |
| [`build.py`](build.py) | Build firmware từ mã nguồn. |

## Tài liệu

- [Hướng dẫn kỹ thuật](docs/CODE_GUIDE.md).

**Giấy phép**  MIT, theo khai báo của dự án.
