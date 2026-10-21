# NDS03 测距功能接入说明

已在原 `PTM_Wall_Filling_Bracket_FPC_SOFT_BOARD_TEST_TOOLS_v1.1.0` 工程中接入 NDS03 SDK 2.0.3，支持治具测试期间周期测距，并通过现有打印串口输出结果。

## 接线与运行

| 功能 | STM32 引脚 / 设置 |
| --- | --- |
| NDS03 SDA | PB7（用户已确认） |
| NDS03 SCL | PB6（用户已确认） |
| NDS03 XSHUT | PB5（用户已确认，程序控制复位/启动） |
| NDS03 INT / GPIO1 | PB4（依据原工程注释；本功能使用轮询，不依赖中断） |
| 打印串口 | USART1，TX=PA9，RX=PA10 |
| 串口参数 | 2000000 baud，8 数据位，1 停止位，无校验、无流控 |
| I²C 地址 | 7 位地址 0x5C；总线写地址 0xB8，读地址 0xB9 |

传感器和串口适配器需与 MCU 共地；传感器供电、电平和 SDA/SCL 外部上拉应符合所用模组的硬件规格。PB6/PB7 测距期间由 NDS03 驱动独占，不能同时运行原 SHT4x 驱动或 PB7 系统 LED 闪烁函数。当前主流程没有调用这些函数。

1. 在 Keil 打开 `PROJECT/PTM_Wall_Filling_Bracket_FPC_SOFT_BOARD_TEST_TOOLS.uvproj`，执行 Rebuild 后烧录。
2. 串口助手连接现有 USART1 打印接口，设置上述参数。
3. 按下/下压原有治具开关（PB1 低有效），原流程开启待测板电源后，NDS03 自动初始化并开始测距。未开始治具测试时不读取传感器。
4. 默认每 200 ms 触发一次单次测距，约 5 次/秒；目标就绪后打印。实际频率受测量及主循环耗时影响。
5. 松开治具开关结束测试，原流程断电，NDS03 状态清零并释放 SDA/SCL 内部上拉，下次测试重新初始化。

串口格式示例（示意值，非实板测量结果）：

```text
[NDS03] ready, addr=0x5C, firmware=0x........
[NDS03] distance=321 mm, confidence=90, count=1234
[NDS03] no valid target, raw_depth=65300, confidence=0
[NDS03] poll failed, error=-3; retry in 1000 ms
[NDS03] stopped
```

`distance` 是毫米距离；`confidence` 是 SDK 原始置信度数值，不解释为百分比；`count` 是 SDK 返回的计数值。距离为 SDK 无效值 65300，或置信度不大于 10 时，打印无有效目标，不把它作为正常距离。通信失败不打印旧测量值。

常见错误：`-3`=I²C 通信失败，`-2`=超时，`-4`=启动失败，`-11`=模组型号不是 NDS03。失败后每隔 1000 ms 自动重新初始化；可检查供电、接线、上拉及 XSHUT 状态。

## 代码位置与工作方式

| 文件 | 作用 |
| --- | --- |
| `APP/NDS03/inc/`、`src/` | 原厂 SDK 文件，逐字节保留 |
| `APP/NDS03/LICENSE` | 原厂 BSD 3-Clause 许可证 |
| `APP/NDS03/nds03_platform.h` | SDK 原有平台接口声明 |
| `APP/NDS03/nds03_platform.c` | STM32 软件 I²C、延时、系统毫秒时钟、XSHUT 控制 |
| `APP/NDS03/nds03_board.h` | 引脚、采样间隔、轮询间隔、超时及置信度门限 |
| `APP/NDS03/nds03_app.c`、`.h` | 初始化、分步测距、串口打印和错误重试 |
| `USER/main.c` | 按键处理后调用 `nds03_app_process()` |
| `APP/GPIO/gpio.c` | PB4 从输出改为无上下拉输入，避免驱动传感器 INT 输出 |
| `PROJECT/*.uvproj` | 加入 NDS03 分组、6 个 C 文件和头文件路径 |

启动顺序：平台初始化 → `NDS03_WaitDeviceBootUp` → `NDS03_IsNDS03` → `NDS03_InitDevice` → 配置 1 个目标及 100000 µs 帧间隔 → 关闭 GPIO1 中断功能。

测量使用 SDK 的 `NDS03_StartSingleMeasurement` 触发，主循环每 5 ms 检查一次 `NDS03_REG_DAT_VAL`；就绪后调用 `NDS03_ReadRangingData` 和 `NDS03_ClearDataValidFlag`，再打印。此顺序对应 SDK 单次测距流程，分开等待以保留主循环的按键处理。单次测量等待上限为 500 ms。初始化、I²C 事务本身仍同步执行，不能把该接口放入中断。

软件 I²C 默认标称 100 kHz，实际频率会因 GPIO 调用和中断降低；支持 10–100 kHz 配置、SCL 有界等待、失败后 STOP 清理、连续读重复 START、末字节 NACK。没有改动原 `APP/IIC` 驱动。平台初始化执行 9 个时钟脉冲及 STOP 释放残留事务。

调整采样频率及门限可修改 `nds03_board.h`。如果修改模组帧间隔，也需相应调整测量超时和采样间隔。没有执行会写入模组 Flash 的标定操作；测距精度仍取决于模组原有标定和安装条件。

## 验证结果与限制

- 使用本机 STM32CubeIDE 附带的 ARM GCC 10.3 工具链，以 Cortex-M4 / Thumb / 硬件浮点 / C99 配置，成功编译新增的 6 个 C 文件及改动的 `main.c`、`gpio.c`。两个新实现文件编译无警告；原 SDK 和原工程保留了一些已有警告。
- 使用真实 SDK 源码配合模拟传感器寄存器，验证了测量/清除就绪位、200 ms 调度、无效距离、低置信度、禁用/重新启动、失败重试、500 ms 超时、32 位时钟回绕和失败后不输出旧数据。
- GPIO 模拟测试验证了 0x5C 的地址转换、连续读写、重复 START、ACK/末字节 NACK、地址 NACK 后 STOP、SCL/SDA 异常退出及参数检查。
- 已检查 Keil XML、源文件路径和 SDK 文件一致性。
- 当前环境未找到可用 Keil 编译器，没有完成完整 Keil 编译链接；上述 ARM 编译是各文件编译检查。没有执行烧录或实板测距，也没有生成新的 HEX。工程原有 OUTPUT/发布 中的历史 HEX 不包含本次功能，应在 Keil 重新生成。

源码包包含源码工程、验证日志、模拟测试代码和修改前的 3 个原文件备份，不包含历史固件或编译缓存。原始项目目录也已直接更新。备份用于按需恢复，恢复前应确认没有后续编辑。
