from pathlib import Path
import re
import sys


def read_config(config_file: Path) -> dict[str, str]:
    config: dict[str, str] = {}
    with config_file.open("r") as f:
        for line in f:
            line = line.strip()
            if line.startswith("#"):
                continue
            match = re.match(r"^([^=]+)=\"?([^\"]*)\"?$", line)
            if match:
                key = match.group(1)
                value = match.group(2)
                config[key] = value
    return config


def main():
    old_config_file = Path(sys.argv[1])
    new_config_file = Path(sys.argv[2])
    try:
        old_config = read_config(old_config_file)
        new_config = read_config(new_config_file)
        assert old_config == new_config
        print("config is same")
    except AssertionError:
        print("config is not from menuconfig, please check defconfig.")
        sys.exit(1)


if __name__ == "__main__":
    main()
