import os
import string
import sys
import struct
import getopt
import zipfile

path = ""
dump_info = "dump_info.txt"
reg_file = "reg.cmm"
ram_dump = "ramdump_0x02020400--0x215FFFF.bin"
zip_file = "crash_dump.zip"
elf_file = "tinyara"

task_state =  [ "invalid",
				"pend",
				"ready to run",
				"running",
				"inactive",
				"wait_sem",
				"wait_sig",
				"wait_mq_emty",
				"wait_mq_full",
				"wait_pagefill" ]

fault_status = [" ",
				"Alignment fault",
				"Debug event fault",
				"Access Flag fault on Section",
				"Cache maintenance operation fault",
				"Translation fault on Section",
				"Access Flag fault on Page",
				"Translation fault on Page",
				"Precise External Abort",
				"Domain fault on Section",
				" ",
				"Domain fault on Page",
				"External abort on translation, first level",
				"Permission fault on Section",
				"External abort on translation, second level",
				"Permission fault on Page",
				" ",
				" ",
				"Imprecise External Abort",
				" ",
				" " ]

class BinaryReader:
	typeNames = {
		'int8'   :'b',
		'uint8'  :'B',
		'int16'  :'h',
		'uint16' :'H',
		'int32'  :'i',
		'uint32' :'I',
		'int64'  :'q',
		'uint64' :'Q',
		'float'  :'f',
		'double' :'d',
		'char'   :'s'}

	def __init__(self, fileName):
		self.file = open(fileName, 'rb')

	def read(self, typeName):
		typeFormat = BinaryReader.typeNames[typeName.lower()]
		typeSize = struct.calcsize(typeFormat)
		value = self.file.read(typeSize)
		if typeSize != len(value):
			print 'Not enough bytes in file to satisfy read request'
			sys.exit(2)
		return struct.unpack(typeFormat, value)[0]
	def readu(self, typeName):
		typeFormat = BinaryReader.typeNames[typeName.lower()]
		typeSize = struct.calcsize(typeFormat)
		value = self.file.read(typeSize)
		if typeSize != len(value):
			print 'Not enough bytes in file to satisfy read request'
			sys.exit(2)
		return value
	def rawread(self, n):
		value = self.file.read(n)
		if n != len(value):
			print 'Not enough bytes in file to satisfy read request'
			sys.exit(2)
		return value
	def m_pos(self, offset):
		self.cur_pos = self.file.tell()
		self.file.seek(self.cur_pos + offset)
	def r_pos(self):
		self.file.seek(self.cur_pos)
	def close(self):
		self.file.close()

def parse_header(b, flag, len):
	start_magic = 'ARTK'
	end_magic = 'DONE'
	magic = b.readu('uint32')
	if (magic != start_magic):
		print ("Dump data corrupted : Start magic is not match")

	flag = b.read('uint32')
	len = b.read('uint32')
	b.m_pos(len)
	magic =  b.readu('uint32')
	if (magic != end_magic):
		print ("Dump data corrupted : End magic is not match")

	b.r_pos()
	return flag, len


def parse_dumpinfo(b, flag, len):
	flag = b.read('uint32')
	dfar = b.read('uint32')
	dfsr = b.read('uint32')
	magic = b.readu('uint32')
	data = "\n================fault information================\n"
	data += "outfile : %s\n" % (path + dump_info)
	data += ("Dump Present Flag : 0x{0:08x}\n".format(flag))
	data += ("DFAR : 0x{0:08x}\n".format(dfar))
	data += ("DFSR : 0x{0:08x}  ".format(dfsr))
	data += fault_status[(dfsr&0x1f)]
	data += "\n"

	fp = open(path + dump_info, 'a')

	sys.stdout.write(data)
	fp.write(data)
	fp.close()

	return flag, len

def parse_bininfo(b, len):
	data = "\n================binary information================\n"
	data += "outfile : %s\n" % (path + dump_info)
	commit_hash_len = 41
	build_time_len = 20
	file_name_len = 32

	commit_hash = b.readu('uint8');
	for i in range(0, commit_hash_len - 1):
		commit_hash += b.readu('uint8');

	build_time = b.readu('uint8');
	for i in range(0, build_time_len - 1):
		build_time += b.readu('uint8');

	file_name = b.readu('uint8');
	for i in range(0,file_name_len - 1):
		file_name += b.readu('uint8');
	line = b.read('uint32');
	magic = b.readu('uint32')

	fp = open(path + dump_info, 'a')
	data += ("commit : %s\n" % (commit_hash))
	data += ("build_time : %s\n" % (build_time))
	data += ("file_name : %s\n" % (file_name))
	data += ("line : %d\n" % (line))

	sys.stdout.write(data)
	fp.write(data)
	fp.close()

def parse_register(b, len):
	print "\n================register dump================"
	print "outfile : %s" % (path + reg_file)
	fp = open(path + reg_file, 'w')

	fp.write('\nB::\n\n')

	for i in range (0, 15):
		reg = b.read('uint32')
		data = ("R%d 0x%08x\n" % (i, reg))
		sys.stdout.write(data)
		fp.write("REGISTER.SET "+ data)

	reg = b.read('uint32')
	data = ("PC 0x%08x\n" % (reg))
	sys.stdout.write(data)
	fp.write("REGISTER.SET "+ data)

	if (len == 68):
		reg = b.read('uint32')
		data = ("CPSR 0x%08x\n" % (reg))
		sys.stdout.write(data)
		fp.write("REGISTER.SET " + data)
	else:
		reg = b.read('uint32')
		data = ("SPSR 0x%08x\n" % (reg))
		sys.stdout.write(data)
		fp.write("REGISTER.SET " + data)

		reg = b.read('uint32')
		data = ("CPSR 0x%08x\n" % (reg))
		sys.stdout.write(data)
		fp.write("REGISTER.SET " + data)

		# USR mode
		for i in range(13, 15):
			reg = b.read('uint32')
			data = ("R%d_USR 0x%08x\n" % (i, reg))
			sys.stdout.write(data)
			fp.write("REGISTER.SET " + data)

		# FIQ mode
		for i in range(8, 15):
			reg = b.read('uint32')
			data = ("R%d_FIQ 0x%08x\n" % (i, reg))
			sys.stdout.write(data)
			fp.write("REGISTER.SET " + data)

		reg = b.read('uint32')
		data = ("SPSR_FIQ 0x%08x\n" % (reg))
		sys.stdout.write(data)
		fp.write("REGISTER.SET " + data)

		# IRQ mode
		for i in range(13,15):
			reg = b.read('uint32')
			data = ("R%d_IRQ 0x%08x\n" % (i, reg))
			sys.stdout.write(data)
			fp.write("REGISTER.SET " + data)

		reg = b.read('uint32')
		data = ("SPSR_IRQ 0x%08x\n" % (reg))
		sys.stdout.write(data)
		fp.write("REGISTER.SET " + data)

		#ABT mode
		for i in range(13,15):
			reg = b.read('uint32')
			data = ("R%d_ABT 0x%08x\n" % (i, reg))
			sys.stdout.write(data)
			fp.write("REGISTER.SET " + data)

		reg = b.read('uint32')
		data = ("SPSR_ABT 0x%08x\n" % (reg))
		sys.stdout.write(data)
		fp.write("REGISTER.SET " + data)


		#UND mode
		for i in range(13, 15):
			reg = b.read('uint32')
			data = ("R%d_UND 0x%08x\n" % (i, reg))
			sys.stdout.write(data)
			fp.write("REGISTER.SET " + data)


		reg = b.read('uint32')
		data =  ("SPSR_UND 0x%08x\n" % (reg))
		sys.stdout.write(data)
		fp.write("REGISTER.SET " + data)

	magic = b.read('uint32')
	fp.write('\nENDDO\n')
	fp.close()

def parse_stackdump(b, len):
	data =  "\n================stack dump================\n"
	data += ('outfile : %s\n' % (path + dump_info))
	fp = open(path + dump_info, 'a')
	sys.stdout.write(data)
	fp.write(data)
	rlen = 0;

	while (len > rlen):

		data = b.read('uint32')
		out = "0x%08x " % (data)
		rlen +=4

		if (rlen % 32 == 0):
			out += "\n"

		sys.stdout.write(out)
		fp.write(out)

	sys.stdout.write("\n")
	fp.write("\n")
	fp.close();
	magic = b.readu('uint32')


def parse_tcb(b, len):
	strlist = []
	printable = set(string.printable)

	tcb_info_size = 52
	task_name_len = 31

	fp = open(path + dump_info, 'a')
	data =  "\n================task information================\n"
	data += ('outfile : %s\n' % (path + dump_info))
	data += ("pid".center(5) +
			"name".center(20) +
			"sch_pri".center(10) +
			"bas_pri".center(10) +
			"state".center(15) +
			"errno".center(10) +
			"stk_size".center(10) +
			"stk_used".center(10)) + "\n"

	sys.stdout.write(data)
	fp.write(data)
	for i in range(0, len/tcb_info_size):
		pid = b.read('uint16')

		block  = b.rawread(32)
		name = ""
		for ch in block:
			if ch in printable:
				name += ch

		sched_priority = b.read('uint32')
		base_priority = b.read('uint8')
		state = b.read('uint8')
		errno = b.read('int32')
		stack_size = b.read('uint32')
		stack_used = b.read('uint32')

		data = ('{:^5}'.format(pid) +
				'{:^20}'.format(name) +
				'{:^10}'.format(sched_priority) +
				'{:^10}'.format(base_priority)+
				'{:^15}'.format(task_state[state])+
				'{:^10}'.format(errno) +
				'{:^10}'.format(stack_size) +
				'{:^10}'.format(stack_used)) + "\n"
		sys.stdout.write(data)
		fp.write(data)

	magic = b.readu('uint32')
	fp.close();

def parse_ram_dump(b, len):
	fp = open(path + ram_dump, 'a')
	print "\n================ram dump data================"
	print "outfile : %s" % (path + ram_dump)
	dump = b.rawread(len)
	fp.write(dump)
	fp.close();

def init_dump(file):
	binR = BinaryReader(file)
	fp = open(path + dump_info, 'w')
	fp.close()
	# Remove Dummy
	binR.rawread(16)
	return binR

def mk_zip():
	print "\n================make archive================"
	print "outfile : %s" % (path + zip_file)
	zip_fp = zipfile.ZipFile((path + zip_file), 'w')
	file_list =[]
	file_list.append(elf_file)
	file_list.append(dump_info)
	file_list.append(reg_file)
	file_list.append(ram_dump)
	print "include files : " , file_list
	os.chdir(path)

	for file in file_list:
		zip_fp.write(file, compress_type=zipfile.ZIP_DEFLATED)
	zip_fp.close()

def main(argv):
	global path
	dump_file = ''
	try:
		opts, args = getopt.getopt(argv,"hi:")
	except getopt.GetoptError:
		print 'crash_dump_parser.py -i <dump file>'
		sys.exit(2)

	for opt, arg in opts:
		if opt == '-h':
			print 'crash_dump_parser.py -i <dump file>'
			sys.exit()
		elif opt in ("-i"):
			path = os.path.dirname(arg) + "/"
			dump_file = os.path.basename(arg)

	if (dump_file == ' '):
		print 'crash_dump_parser.py -i <dump file>'
		sys.exit()

	binR = init_dump(path + dump_file)

	len = 0
	flag = 0
	dump_flag = 0x01

	while dump_flag:
		flag, len = parse_header(binR, flag, len)
		if (flag == 0x01):
			dump_flag, len = parse_dumpinfo(binR, dump_flag, len)
		elif (flag == 0x02):
			parse_bininfo(binR, len)
		elif (flag == 0x04):
			parse_register(binR, len)
		elif (flag == 0x08):
			parse_stackdump(binR, len)
		elif (flag == 0x10):
			parse_tcb(binR, len)
		elif (flag == 0x20):
			parse_ram_dump(binR, len)
		else :
			break

		dump_flag &= (~flag)
	binR.close()
	mk_zip()

main(sys.argv[1:])
