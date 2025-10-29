#from distutils.core import setup
from setuptools import setup, find_packages
from bkutils.common.version import bk_ver

setup(name="bk_flash",
    version=bk_ver,
    description="Beken flash download tool",
    author="beken",
    author_email="",
    maintainer="beken",
    maintainer_email="zhu.su@bekencorp.com",
    url="",
    license="MIT",
    requires=["pyserial", "click", "pyyaml"],
    platforms=["Windows", "GNU/Linux", "Darwin"],
    packages=['bkutils', 'bkutils/action_service','bkutils/bk_log','bkutils/chip_controller','bkutils/chip_controller/chip_set','bkutils/common','bkutils/flash_adapter','bkutils/protocol','bkutils/protocol/app','bkutils/protocol/bootrom','bkutils/protocol/bootrom/v2_0','bkutils/protocol/bootrom/v2_0/v1_4','config'],
    data_files=[('config','config/flash_info.yml')],
    scripts=['bk_loader.py,tizen_entrance.py'])