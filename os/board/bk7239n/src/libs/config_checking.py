#!/usr/bin/env python3

import argparse
import sys
import re
import os

class ConfigChecking:

	def __init__(self, lib_sdkconfig=None, armino_sdkconfig=None):
		self.lib_sdkconfig = lib_sdkconfig
		self.armino_sdkconfig = armino_sdkconfig
		print(f'properties lib sdkconfig: {self.lib_sdkconfig}')
		print(f'armino sdkconfig: {self.armino_sdkconfig}')

		self.lib_configs = dict()
		self.armino_configs = dict()

	def check_config(self):

		self.lib_configs = self.parse_sdkconfig(self.lib_sdkconfig)
		self.armino_configs = self.parse_sdkconfig(self.armino_sdkconfig)
		self.find_mismatch()

	def parse_sdkconfig(self, sdkconfig_path):
		d = dict()
		f = open(sdkconfig_path, 'r')
		for l in f:
			l = ' '.join(l.split())
			line_words = l.split(' ')
			if len(line_words) < 1:
				continue

			if line_words[0] == '#':
				if (len(line_words) != 5):
					continue
				if (line_words[2] == 'is') and (line_words[3] == 'not') and (line_words[4] == 'set'):
					if line_words[1][0:7] != 'CONFIG_':
						continue
					k = line_words[1]
					v = 'is not set'
					d[k] = v
				#print(f'add key={k} value={v}')
			else:
				line_key_value = l.split('=')
				if len(line_key_value) != 2:
					continue
				k = line_key_value[0]
				v = line_key_value[1]
				if k[0:7] != "CONFIG_":
					continue
				d[k] = v
				#print(f'add key={k} value={v}')

		f.close()
		return (d)

	def find_mismatch(self):
		mismatch_cnt = 0
		for lib_key,lib_value in self.lib_configs.items():
			for armino_key, armino_value in self.armino_configs.items():
				if (lib_key == armino_key) and (lib_value != armino_value):
					print(f"\033[31m--mismatched config={lib_key} properties={lib_value} armino={armino_value}\033[0m")
					mismatch_cnt = mismatch_cnt + 1
		if (mismatch_cnt != 0):
			exit(1)

def main():
	parser = argparse.ArgumentParser(description='sdkconfig_check - consistence checking between properties and armino')

	parser.add_argument(
		'--lib_sdkconfig', help='absolute path of properties lib sdkconfig',
		default=None,
		dest='lib_sdkconfig')

	parser.add_argument(
		'--armino_sdkconfig', help='absolute path of properties armino sdkconfig',
		default=None,
		dest='armino_sdkconfig')

	args = parser.parse_args()

	if args.lib_sdkconfig:
		if os.path.exists(args.lib_sdkconfig) == False:
			print(f'Properties lib config file {args.lib_sdkconfig} not exists')
			exit(1)
	else:
		print(f'Missing properties lib sdkconfig')
		exit(1)

	if args.armino_sdkconfig:
		if os.path.exists(args.armino_sdkconfig) == False:
			print(f'Armino config file {args.armino_sdkconfig} not exists')
			exit(1)
	else:
		print(f'please provide armino sdkconfig')
		exit(1)

	lib_sdkconfig = os.path.realpath(args.lib_sdkconfig)
	armino_sdkconfig = os.path.realpath(args.armino_sdkconfig)

	cc = ConfigChecking(lib_sdkconfig, armino_sdkconfig)
	cc.check_config()

if __name__ == "__main__":
	main()
