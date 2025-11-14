#!/usr/bin/env python3
from pathlib import Path
import yaml
from datetime import datetime


class TizenSerialPortConfig:

    def __init__(self, port_name):
        self.config_file = (
            Path(__file__).resolve().parent.joinpath("serial_port_mapping.yml")
        )
        self.port_name = port_name
        self.config = {}
        self.load_config()

    def load_config(self):
        """加载配置文件"""
        try:
            """如果文件不存在，就新建一个文件"""
            if not self.config_file.exists():
                self.config_file.touch()
            with open(self.config_file, "r", encoding="utf-8") as f:
                self.config = yaml.safe_load(f) or {}
        except Exception as e:
            self.config = {"serial_ports": {}}

    def save_config(self):
        """保存配置文件"""
        try:
            with open(self.config_file, "w", encoding="utf-8") as f:
                yaml.dump(self.config, f, allow_unicode=True, default_flow_style=False)
            return True
        except Exception as e:
            return False

    def check_config(self) -> bool:
        """检查配置是否完整"""
        port_config = self.get_port_config()
        if not port_config:
            return False
        if "chip_id" not in port_config:
            return False
        if "flash_id" not in port_config:
            return False
        if "work_baudrate" not in port_config:
            return False
        if "debug" not in port_config:
            return False
        return True

    def get_port_config(self):
        """获取指定串口的配置"""
        port_config = self.config.get("serial_ports", {}).get(self.port_name, {})
        return port_config

    def update_port_config(
        self, ota_done=None, flash_id=None, chip_id=None, work_baudrate=None, debug=None, link_type=None
    ):
        """更新指定串口的配置"""
        if "serial_ports" not in self.config:
            self.config["serial_ports"] = {}

        if self.port_name not in self.config["serial_ports"]:
            self.config["serial_ports"][self.port_name] = {}

        # 更新时间
        self.config["serial_ports"][self.port_name][
            "last_updated"
        ] = datetime.utcnow().strftime("%Y-%m-%dT%H:%M:%SZ")

        # 更新flash_id（如果提供），确保以十六进制格式存储
        if flash_id is not None:
            try:
                # 尝试将输入转换为整数，然后转换为十六进制字符串
                flash_id_hex = hex(int(flash_id))
                self.config["serial_ports"][self.port_name]["flash_id"] = flash_id_hex
            except (ValueError, TypeError):
                # 如果转换失败，保留原始值
                self.config["serial_ports"][self.port_name]["flash_id"] = flash_id

        # 更新chip_id（如果提供），确保以十六进制格式存储
        if chip_id is not None:
            try:
                # 尝试将输入转换为整数，然后转换为十六进制字符串
                chip_id_hex = hex(int(chip_id))
                self.config["serial_ports"][self.port_name]["chip_id"] = chip_id_hex
            except (ValueError, TypeError):
                # 如果转换失败，保留原始值
                self.config["serial_ports"][self.port_name]["chip_id"] = chip_id

        # 更新work_baudrate（如果提供）
        if work_baudrate is not None:
            self.config["serial_ports"][self.port_name]["work_baudrate"] = work_baudrate
        # 更新debug（如果提供）
        if debug is not None:
            self.config["serial_ports"][self.port_name]["debug"] = debug

        # 更新ota_done（如果提供）
        if ota_done is not None:
            self.config["serial_ports"][self.port_name]["ota_done"] = ota_done
        # 更新link_type（如果提供）
        if link_type is not None:
            self.config["serial_ports"][self.port_name]["link_type"] = link_type
        return self.save_config()

    def delete_port_config(self):
        """删除指定串口的配置"""
        if (
            "serial_ports" in self.config
            and self.port_name in self.config["serial_ports"]
        ):
            del self.config["serial_ports"][self.port_name]
            return self.save_config()
        else:
            return False
    
    def is_bl1_link(self):
        """判断是否是bl1链接"""
        return self.link_type == "0"
    
    @property
    def link_type(self):
        """获取指定串口的link_type"""
        return self.get_port_config().get("link_type", None)
    
    @link_type.setter
    def link_type(self, value):
        """设置指定串口的link_type"""
        self.update_port_config(link_type=value)

    @property
    def chip_id(self):
        """获取指定串口的chip_id"""
        return int(self.get_port_config().get("chip_id", None), 16)

    @chip_id.setter
    def chip_id(self, value):
        """设置指定串口的chip_id"""
        self.update_port_config(chip_id=value)

    @property
    def flash_id(self):
        """获取指定串口的flash_id"""
        return int(self.get_port_config().get("flash_id", None), 16)

    @flash_id.setter
    def flash_id(self, value):
        """设置指定串口的flash_id"""
        self.update_port_config(flash_id=value)

    @property
    def work_baudrate(self):
        """获取指定串口的work_baudrate"""
        return self.get_port_config().get("work_baudrate", None)

    @work_baudrate.setter
    def work_baudrate(self, value):
        """设置指定串口的work_baudrate"""
        self.update_port_config(work_baudrate=value)

    @property
    def debug(self):
        """获取指定串口的debug level"""
        return self.get_port_config().get("debug", None)

    @debug.setter
    def debug(self, value):
        """设置指定串口的debug level"""
        self.update_port_config(debug=value)

    @property
    def ota_done(self):
        """ota是否完成"""
        return self.get_port_config().get("ota_done", False)

    @ota_done.setter
    def ota_done(self, value):
        """设置指定串口的ota_done"""
        self.update_port_config(ota_done=value)


# 示例用法
if __name__ == "__main__":
    config = TizenSerialPortConfig("/dev/ttyUSB0")
    print(config.check_config())
    config.flash_id = 1024
    config.chip_id = 0x2048  # 十进制值，会自动转换为十六进制

    port_config = config.get_port_config()
    print(port_config)
    print(config.chip_id)
    print(config.flash_id)
    print(config.work_baudrate)

    # config.chip_id = "dfafasfafasfa1"
    # config.flash_id = "testttt"
    # config.work_baudrate = 1000000
    # print(config.chip_id)
    # print(config.flash_id)
    # print(config.work_baudrate)
