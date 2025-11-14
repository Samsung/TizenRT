#!/usr/bin/env python3
from serial_port_config import TizenSerialPortConfig

def main():
    # 初始化配置对象
    config_file = '/home/yibo.liu/project/bk_flash_tizen/build/tools/armino/burn/config/serial_port_mapping.yml'
    config = TizenSerialPortConfig(config_file)

    # 打印当前配置
    print("=== 初始配置 ===")
    config.print_config()

    # 获取特定串口的配置
    port_name = '/dev/ttyUSB0'
    port_config = config.get_port_config(port_name)
    print(f"\n=== {port_name} 的配置 ===")
    print(f"last_updated: {port_config.get('last_updated', 'N/A')}")
    print(f"flash_id: {port_config.get('flash_id', 'N/A')}")
    print(f"chip_id: {port_config.get('chip_id', 'N/A')}")

    # 更新串口配置
    print("\n=== 更新串口配置 ===")
    config.update_port_config('/dev/ttyUSB0', flash_id='flash-987654', chip_id='chip-432109')

    # 重新加载配置
    config.load_config()

    # 打印更新后的配置
    print("\n=== 更新后的配置 ===")
    config.print_config()

    # 添加新的串口配置
    print("\n=== 添加新串口配置 ===")
    config.update_port_config('/dev/ttyUSB2', flash_id='flash-112233', chip_id='chip-445566')

    # 重新加载配置
    config.load_config()

    # 打印添加后的配置
    print("\n=== 添加后的配置 ===")
    config.print_config()

    # 删除串口配置
    print("\n=== 删除串口配置 ===")
    config.delete_port_config('/dev/ttyUSB1')

    # 重新加载配置
    config.load_config()

    # 打印删除后的配置
    print("\n=== 删除后的配置 ===")
    config.print_config()

if __name__ == '__main__':
    main()