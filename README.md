# 10m1s Central — 多芯片 BLE+2.4G SDK / Multi-Chip BLE+2.4G SDK

> Telink `tl_ble_sdk V4.0.4.3` 多芯片统一 SDK，同时支持 B91/B92/TL721X/TL321X 四类芯片，含 BLE+2.4G 双协议栈与键盘 HID 应用。
> Telink `tl_ble_sdk V4.0.4.3` unified multi-chip SDK supporting B91/B92/TL721X/TL321X, with BLE+2.4G dual stack and keyboard HID application.

## 简介 / Overview

本仓库是 Telink `tl_ble_sdk V4.0.4.3` 的多芯片统一 SDK，工程名 `10m1s_central`。通过 `CHIP_TYPE` 宏支持 B91/B92/TL721X/TL321X 四类芯片，提供 2.4G 私有协议与 BLE 双协议栈，应用层聚焦键盘 HID 与 USB 标准 HID。含 secure boot 工具与固件校验。

This repo is the unified multi-chip SDK based on Telink `tl_ble_sdk V4.0.4.3`. Supports B91/B92/TL721X/TL321X via `CHIP_TYPE` macro, with 2.4G proprietary + BLE dual stack, keyboard HID + USB standard HID applications. Includes secure boot tools and firmware verification.

## 主要特性 / Key Features

- **多芯片统一 SDK** / Multi-chip unified SDK (B91/B92/TL721X/TL321X via `CHIP_TYPE`)
- **BLE + 2.4G 双协议栈** / BLE + 2.4G dual protocol stack
- **键盘 HID 应用** / Keyboard HID application (`application/keyboard/`)
- **USB 标准 HID** / USB standard HID (`application/usbstd/`)
- **Secure Boot** / Secure boot support (`secure_boot_tool/`)
- **固件校验工具** / Firmware verification (`tl_check_fw2.exe`)

## 版本信息 / Version

| 项 | 值 |
|---|---|
| SDK 版本 | tl_ble_sdk V4.0.4.3 |
| 支持芯片 | B91, B92, TL721X, TL321X |
| 工具链 | TC32 ELF GCC |
| 许可证 | Apache 2.0 |

## 目录结构 / Directory Structure

| 目录/文件 | 说明 / Description |
|---|---|
| `stack/ble/` | BLE 协议栈 / BLE stack |
| `stack/2p4g/` | 2.4G 私有协议 / 2.4G proprietary stack |
| `application/keyboard/` | 键盘 HID 应用 / Keyboard HID app |
| `application/usbstd/` | USB 标准 HID / USB standard HID |
| `drivers/` | 驱动 / Drivers |
| `algorithm/` | 算法库 / Algorithm library |
| `3rd-party/` | 第三方库 / Third-party libraries |
| `secure_boot_tool/` | 安全启动工具 / Secure boot tools |
| `config.h` | 芯片类型配置 / Chip type config (`CHIP_TYPE`) |

## 构建 / Build

```bash
# 在 config.h 中选择 CHIP_TYPE:
#   CHIP_TYPE_B91=1, CHIP_TYPE_B92=2, CHIP_TYPE_TL721X=5, CHIP_TYPE_TL321X=6
# Select CHIP_TYPE in config.h, then build with TelinkIoTStudio
```

## 许可证 / License

Apache License 2.0

## 作者 / Author

**JannLeo** — [GitHub](https://github.com/JannLeo)
