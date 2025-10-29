from __future__ import annotations

import shutil
import sys
from pathlib import Path
import os


def copy_files(postfix: str, src_dir: Path, dst_dir: Path):
    # logging.debug(f'copy *{postfix} from {src_dir}')
    # logging.debug(f'to {dst_dir}')
    for f in src_dir.iterdir():
        if f.suffix == postfix:
            shutil.copy(f, dst_dir / f.name)


def install_configs(cfg_dir: Path, install_dir: Path):
    # logging.debug(f'install configs from: {cfg_dir}')
    # logging.debug(f'to: {install_dir}')
    if not cfg_dir.exists():
        return

    copy_files(".bin", cfg_dir, install_dir)
    copy_files(".json", cfg_dir, install_dir)
    copy_files(".pem", cfg_dir, install_dir)
    copy_files(".csv", cfg_dir, install_dir)

    if (cfg_dir / "key").exists():
        copy_files(".pem", cfg_dir / "key", install_dir)
    if (cfg_dir / "csv").exists():
        copy_files(".csv", cfg_dir / "csv", install_dir)
    if (cfg_dir / "regs").exists():
        copy_files(".csv", cfg_dir / "regs", install_dir)


def pre_handle(middleware_dir: Path, workdir: Path):
    from partitions_adapter import adapt_partitions

    install_configs(middleware_dir, workdir)
    config_file = Path(__file__).parents[4] / "os/.config"
    output_csv = workdir / "partitions.csv"
    adapt_partitions(config_file, output_csv) # TODO use preset partitions csv now.
    tfm_prehandle_script = Path(__file__).parent.parent / "tfm_prehandle/main.py"
    os.chdir(workdir)
    ret = os.system(f"python3 -B {tfm_prehandle_script} gen all")
    if ret != 0:
        raise RuntimeError("gen config failed")
    shutil.copy(workdir / "partitions_partition.h", workdir / "partitions_gen.h")


if __name__ == "__main__":
    middleware_dir = Path(sys.argv[1]).absolute()
    workdir = Path(sys.argv[2]).absolute()
    assert middleware_dir.exists()
    assert workdir.exists()
    pre_handle(middleware_dir, workdir)
