#!/usr/bin/env python
#-*- coding: utf-8 -*-

''' Download script for ARTIK 05X '''

import argparse

from sys import stdout, stderr
from os.path import dirname, realpath, exists
import __main__

__author__ = "Jihun Ahn"
__copyright__ = "Copyright 2018 Samsung Electronics All Rights Reserved."
__version__ = "0.0.0"
__email__ = "jhun.ahn@samsung.com"


def singleton(cls):
    instances = {}

    def wrapper(*args, **kwargs):
        if cls not in instances:
            instances[cls] = cls(*args, **kwargs)
        return instances[cls]
    return wrapper


def system_binary_info(class_):
    def f(*args, **kwargs):

        try:
            ret = class_(*args, **kwargs)
            return ret
        except Exception as e:
            raise RuntimeError(e)

        finally:
            if not kwargs.get('hidden', False):
                print('create %s binary: %s' % (args[0], getattr(ret, 'binpath')))

    return f

def system_binary(class_):

    ret = type(getattr(class_, '__name__'), (), dict(class_.__dict__))

    def __wrapper(cls):

        orig_vars = cls.__dict__.copy()
        slots = orig_vars.get('__slots__')

        if slots is not None:
            slots = [slots] if isinstance(slots, str) else slots

            for slots_var in slots:
                orig_vars.pop(slots_var)

        orig_vars.pop('__dict__', None)
        orig_vars.pop('__weakref__', None)

        return metaclass_(cls.__name__, cls.__bases__, orig_vars)

    class metaclass_(type):
        def __new__(self, *args, **kwargs):

            dct = args[2]

            __inner_init = dct['__init__'] if '__init__' in dct else \
                lambda self, *args, **kwargs: None

            def __wrapper_init(self, *args, **kwargs):
                target_ = args[0]
                kwargs['binpath'] = path(
                    (system().boardpath(kwargs.get('board')), 'bin'))

                _erase = kwargs.get('erase')
                _verify = kwargs.get('verify')

                command_ = 'flash_erase_part' if _erase else 'flash_write'
                opt_ = 'verify' if _verify and not _erase else ''

                if not _erase:
                    __inner_init(self, *args, **kwargs)
                    if not exists(self.binpath):
                        raise RuntimeError("Could not found %s binary: %s" %
                                           (target_, self.binpath))

                else:
                    binpath_ = ''

                for k, v in vars().items():
                    if k.endswith('_'):
                        setattr(self, k[:-1], v)

            dct['__init__'] = __wrapper_init

            __inner_repr = dct['__repr__'] if '__repr__' in dct else \
                lambda self, *args, **kwargs: args[0]

            def __wrapper_repr(self, *args, **kwargs):
                str_ = ' '.join((self.command, self.target, self.binpath, self.opt))
                return __inner_repr(self, str_, **kwargs)

            dct['__repr__'] = __wrapper_repr

            return type.__new__(self, *args, **kwargs)

        def __call__(self, target, **kwargs):
            return super(metaclass_, self).__call__(target, **kwargs)

    return __wrapper(ret)

def path(*args):
    from os import sep
    return sep.join(*(s for s in args))


def execute(*command):
    from subprocess import Popen, STDOUT

    proc = Popen(args=command, stdout=stdout, stderr=STDOUT)
    _, errmsg = proc.communicate()
    retcode = proc.poll()

    if retcode:
        raise RuntimeError(errmsg)


@singleton
class system(object):
    def __init__(self):
        import platform
        from os.path import abspath

        basepath = dirname(abspath(__file__))
        configpath = path((basepath, 'scripts'))
        buildpath = realpath(path((basepath, '..', '..')))
        outputpath = path((buildpath, 'output', 'bin'))
        toolpath = path((basepath, 'tools'))

        _arch = '64' if platform.machine() == 'x86_64' else '32'
        if platform.system() == "Windows":
            _tools = 'win' + _arch
        elif platform.system() == "Linux":
            _tools = 'linux' + _arch
        elif platform.system() == "Darwin":
            _tools = 'macos'
        else:
            raise NotImplementedError(
                "Not supported at %s" % platform.system())

        execpath = path((buildpath, 'tools', 'openocd', _tools))

        for key, value in sorted(vars().items()):
            if key.endswith('path'):
                setattr(self, key, value)

        setattr(self, 'codesigner', path((toolpath, 'codesigner', _tools,
                                          'artik05x_AppCodesigner')))

    def boardpath(self, boardname='artik05x'):
        return realpath(path((self.basepath, '..', boardname)))

    def __repr__(self):
        return '\n'.join(('%s: %s' % (key, value)
                          for key, value in vars(self).items() if not key.startswith('_')))


@singleton
class _parser(argparse.ArgumentParser):

    def __init__(self, function):
        argparse.ArgumentParser.__init__(self)
        self.prog = "artik05x_download"
        self.description = "Flashing tool for Artik05x"
        self.formatter_class = argparse.RawTextHelpFormatter

        self.add_argument("--board", type=str, required=True,
                          metavar='<boardtype>',
                          help="select target board-name")
        self.add_argument("--secure", action='store_true',
                          help="choose secure mode, and set the codesinger path")
        self.add_argument("--verify", action='store_true',
                          help="verify downloaded image if you need")
        self.add_argument("--erase", action='store_true',
                          help='erase the FLASH memory area corresponding to "<partition>".')
        self.add_argument("--make", action='store_true',
                          help='Does not write to FLASH memory, '
                          'only create image file for <partition>".')

        self.function = function

    def __call__(self, *args, **kwargs):
        self.add_argument("partition", type=str, default=['ALL'],
                          nargs='*',
                          help='write each firmware image into FLASH\n'
                          '[ALL | %s]' % ' | '.join(n for n in sorted(args)))
        self._args = self.parse_args()
        return {k: v for k, v in vars(self._args).items()}

class image(object):
    def __new__(self, target, **kwargs):
        _inst = getattr(__main__, target) \
            if hasattr(__main__, target) else _binary


        return _inst(target, **kwargs)


@system_binary
class _binary(object):
    def __init__(self, target, **kwargs):
        self.binpath = path((kwargs['binpath'], target)) + '.bin'


@system_binary
class bl1(object):
    def __init__(self, target, **kwargs):
        self.binpath = path((kwargs['binpath'], target)) + '.bin'

    def __repr__(self, *args, **kwargs):
        return 'flash_protect off; %s; flash_protect on' % args[0]

@system_binary
class os(object):
    def __init__(self, target, **kwargs):
        _system = system()
        binpath = path((_system.outputpath, 'tinyara_head.bin'))

        if not exists(binpath):
            raise RuntimeError("Could not found %s binary: %s" % (target, binpath))

        if kwargs.get('secure'):
            if not exists(binpath + '-signed'):
                execute(*(_system.codesigner,
                          path((dirname(_system.codesigner), '..', 'rsa_private.key')),
                          binpath))
            binpath += '-signed'

        self.binpath = binpath


@system_binary_info
@system_binary
class factory(object):
    def __init__(self, target, **kwargs):
        osimage = image('os', **kwargs)

        binpath = path((kwargs.get('binpath'), target)) if not kwargs.get('hidden') \
            else path((kwargs.get('binpath'), '.' + target))
        binpath += '.bin'

        execute(*('gzip', '-k', osimage.binpath))
        execute(*('mv', '%s.gz' % osimage.binpath, binpath))

        self.binpath = binpath


@system_binary_info
@system_binary
class ota(object):
    def __init__(self, target, **kwargs):
        from os import remove
        import binascii
        import struct

        factory = path((kwargs.get('binpath'), 'factory')) + '.bin'

        if not exists(factory):
            kwargs['hidden'] = True
            factory = image('factory', **kwargs).binpath

        binpath = path((kwargs.get('binpath'), target)) + '.bin'

        if exists(binpath):
            remove(binpath)

        with open(factory, 'rb') as f:
            _bytes = bytearray(f.read())

        size = len(_bytes)
        cnt = (((size + 3) / 4) * 4) - size

        for _ in range(0, cnt):
            _bytes.append(0)

        size = len(_bytes)
        crc = binascii.crc32(_bytes) & 0xFFFFFFFF

        with open(binpath, 'wb') as f:
            f.write(bytearray(4096))
            f.write(_bytes)
            f.write("NRTA")
            f.seek(0)
            f.write("TIZE")
            f.write(struct.pack('I', size))
            f.write(struct.pack('I', crc))

        remove(factory)

        self.binpath = binpath


class partition(object):
    def __init__(self):
        with open(path((system().configpath, 'partition_map.cfg')), 'r') as f:
            text = '\n'.join(s.strip() for s in f.read().split('\n'))

        import re
        match = re.search(r'.*set\ .*\ {(.*)}$',
                          text, re.MULTILINE | re.DOTALL)

        if match is None:
            raise RuntimeError("Invalid format of the partition map.")

        items = [s for s in match.groups()[0].split('\n')]
        pattern = r'^([a-zA-Z0-9]*).{.\"(.*)\".\s*(0x[a-fA-F0-9]*).(0x[a-fA-F0-9]*).([0-1]).}$'
        for i in items:
            match = re.search(pattern, i)
            if match is not None:
                p = match.groups()
                setattr(self, p[0], dict(
                    desc=p[1], addr=p[2], size=p[3], ro=int(p[4])))

    @property
    def names(self):
        for key, value in vars(self).items():
            if key is __name__:
                continue

            if not key.startswith('_') and value.get('ro') is 0:
                yield key

    def __repr__(self, *args, **kwargs):
        import json
        return json.dumps({k: getattr(self, k) for k in self.names},
                          indent=4, sort_keys=True)

    @property
    def optional(self):
        return ['factory', 'ota', 'user']


@_parser
def parser(*args):
    del args


@singleton
class download(object):

    def __init__(self):
        self.opt = 'init;reset halt'

    def append(self, image):
        self.opt += ';' + str(image)

    def start(self):
        self.opt += ''.join((';', 'reset', ';', 'exit'))

        execute(*(path((system().execpath, 'openocd')), '-f', 'artik05x.cfg',
                  '-s', system().configpath, '-c', self.opt))


if __name__ == '__main__':
    from signal import signal, SIGINT, SIGTERM

    def sighandler(sig, frame):
        from os import getpgrp, killpg, getpid
        killpg(getpgrp(), SIGINT)

    signal(SIGINT, sighandler)

    try:
        part = partition()

        params = parser(*part.names)
        makeOnly = params.pop('make')
        _partition = params.pop('partition')

        if 'ALL' in _partition:
            if makeOnly:
                raise GeneratorExit("To create binary image, you have to set the <'partition'>")

            _partition = [p for p in part.names if p not in part.optional] \
                if not params.get('erase') else ['all']

        _addoption = download().append if not makeOnly else lambda x: None
        _start = download().start if not makeOnly else lambda: None

        for p in sorted(_partition):
            _addoption(image(p, **params))
        _start()
    except Exception as e:
        stderr.write(str(e) + '\n')
        exit(1)
