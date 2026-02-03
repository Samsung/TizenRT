import logging
from pathlib import Path
import threading
import click
from bkutils.action_service.tizen.serial_port_config import TizenSerialPortConfig
from bkutils.action_service.tizen.tizen_download import TizenDownload, TizenDownloadInfo
from bkutils.action_service.tizen.tizen_end_worker import TizenEndWorker
from bkutils.action_service.tizen.tizen_erase import TizenErase, TizenEraseInfo
from bkutils.action_service.tizen.tizen_pre_worker import PreWorker
from bkutils.bk_log.log_utils import BKLog
from bkutils.common.common import BIN_TYPE, LINKCHECK, parse_tizen_binfile_info


def init_log(debug=None, port=None):
    if debug is None:
        debug = TizenSerialPortConfig(port).debug or 3
    debug = str(debug)

    log_path = Path(__file__).parent.resolve().joinpath("tizen_log")
    logger_level_dict = {
        "0": logging.CRITICAL,
        "1": logging.ERROR,
        "2": logging.WARNING,
        "3": logging.INFO,
        "999": logging.DEBUG,
    }

    BKLog(
        logger_level=logger_level_dict[debug],
        log_folder_path=log_path,
        threading_id=threading.current_thread().ident,
        COM_PORT=port,
    )  # 实例化log


@click.group()
def cli():
    pass


@cli.command("pre_download")
@click.option(
    "-p",
    "--portnum",
    default="/dev/ttyUSB0",
    required=True,
    help="UART device, such as default '/dev/ttyUSB0'",
)
@click.option(
    "--link_type",
    type=click.Choice(["0", "1", "2"]),
    default="0",
    help="link boot rom type",
)
@click.option(
    "-b",
    "--baudrate",
    default=2000000,
    type=int,
    help="Burn UART baudrate, defaults to 2000000",
)
@click.option(
    "-d",
    "--debug",
    type=click.Choice(["0", "1", "2", "3", "999"]),
    default="3",
    help="Set debug level: 0=nothing, 1=error message, 2=warning message, 3=info in test process + file, 999=full debug message + file",
)
def pre_download_cli(**kwargs):
    debug = kwargs["debug"]
    port = kwargs["portnum"]
    baudrate = kwargs["baudrate"]
    link_type = kwargs["link_type"]
    real_pre_download(debug, port, baudrate, link_type)


@cli.command("download")
@click.option(
    "-p",
    "--portnum",
    default="/dev/ttyUSB0",
    required=True,
    help="UART device, such as default '/dev/ttyUSB0'",
)
@click.option(
    "-e",
    "--pre_erase",
    type=click.Choice(["0", "1"]),
    default="1",
    help="erase flash before write",
)
@click.option(
    "-i",
    "--infile",
    type=click.Path(),
    help="Input one for more existing binary files.",
)
@click.option("--partition", type=str, required=True)
def download_cli(**kwargs):
    port = kwargs["portnum"]
    pre_erase = kwargs["pre_erase"]
    infile = kwargs["infile"]
    partition = kwargs["partition"]
    real_download(port, pre_erase, infile, partition)


@cli.command("erase")
@click.option(
    "-p",
    "--portnum",
    default="/dev/ttyUSB0",
    required=True,
    help="UART device, such as default '/dev/ttyUSB0'",
)
@click.option(
    "-f",
    "--freedom",
    type=str,
    help='Input freedom address and length,Hex data,such as " [-f 0-10000] "',
)
@click.option("--partition", type=str, required=True)
def erase_cli(**kwargs):
    port = kwargs["portnum"]
    freedom = kwargs["freedom"]
    partition = kwargs["partition"]
    real_erase(port, freedom, partition)


@cli.command("post_download")
@click.option(
    "-p",
    "--portnum",
    default="/dev/ttyUSB0",
    required=True,
    help="UART device, such as default '/dev/ttyUSB0'",
)
@click.option(
    "-r", "--reboot", is_flag=True, default=False, help="Reboot after download process"
)
def post_download(**kwargs):
    reboot = kwargs["reboot"]
    port = kwargs["portnum"]
    real_post_download(port, reboot)


def real_pre_download(debug, port, baudrate, link_type):
    try:
        init_log(debug, port=port)
        BKLog.i(
            "pre download args: debug: %s, port: %s, baudrate: %s, link_type: %s",
            debug,
            port,
            baudrate,
            link_type,
        )
        TizenSerialPortConfig(port).delete_port_config()
        pre_worker = PreWorker(
            port=port,
            worker_baudrate=baudrate,
            link_check_type=LINKCHECK.BOOTROM if link_type == "0" else LINKCHECK.BL2,
        )

        result, chip_id, flash_id, worker_baudrate, real_link_type = pre_worker.pre_work()
        if result:
            config = TizenSerialPortConfig(port)
            config.chip_id = chip_id
            config.flash_id = flash_id
            config.work_baudrate = worker_baudrate
            config.debug = debug
            config.link_type = '0' if real_link_type == LINKCHECK.BOOTROM else '1'

    except Exception as e:
        BKLog.e(str(e))


def real_download(port, pre_erase, infile, partition):
    try:
        init_log(port=port)
        BKLog.i(
            "download args: port: %s, pre_erase: %s, infile: %s, partition: %s",
            port,
            pre_erase,
            infile,
            partition,
        )
        if partition.lower() in ["rf", "net"]:
            BKLog.i("partition %s skip download", partition)
            return
        bintype, binlist,path_list = parse_tizen_binfile_info(infile, 0)
        tizen_download_info = TizenDownloadInfo()
        tizen_download_info.vecbinInfo = binlist
        tizen_download_info.pre_erase = int(pre_erase)

        tizen_download = TizenDownload(port=port)

        if bintype == BIN_TYPE.TYPE_BL2_OTA:
            tizen_download.bootloader_ota(tizen_download_info)
        else:
            tizen_download.download(tizen_download_info)
    except Exception as e:
        BKLog.e(str(e))


def real_erase(port, freedom, partition):
    try:
        init_log(port=port)
        BKLog.i(
            "erase args: port: %s, freedom: %s, partition: %s", port, freedom, partition
        )
        if partition.lower() in ["rf", "net"]:
            BKLog.i("partition %s skip erase", partition)
            return
        if freedom is None:
            raise Exception("erase addr is None")

        stmp = freedom.split("-")
        start_addr = int(stmp[0], 16)
        length = int(stmp[1], 16)

        erase_info = TizenEraseInfo()
        erase_info.start_addr = start_addr
        erase_info.length = length

        tizen_erase = TizenErase(port=port)
        tizen_erase.erase(erase_info)
    except Exception as e:
        BKLog.e(str(e))


def real_post_download(port, reboot):
    try:
        init_log(port=port)
        BKLog.i("post download args: port: %s, reboot: %s", port, reboot)
        tizen_end_worker = TizenEndWorker(port=port)
        tizen_end_worker.end_worker(reboot_after_donwload=reboot)
    except Exception as e:
        BKLog.e(str(e))


def test():
    biz = 1
    port = "/dev/ttyUSB0"

    if biz == 1:
        real_pre_download(3, port, 2000000, 0)
    elif biz == 2:
        real_download(
            port,
            1,
            "/home/yibo.liu/project/bk_flash_tizen/build/output/bin/esp-at.bin@0x0-0x1000",
            "bl1",
        )
    elif biz == 3:
        real_erase(port, "0-1000", "bl1")
    elif biz == 4:
        real_post_download(port, True)


if __name__ == "__main__":
    cli()
    # test()
