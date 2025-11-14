#!/usr/bin/env python

import sys
import subprocess
import os
import re
import shutil

def obj_list_gen():
	if os.path.exists('obj_list.map'):
		os.remove('obj_list.map')
	file=open('obj_list.map', 'w')

	parse_file=open('image/ram_size.txt', 'r')
	all_lines = parse_file.readlines()

	for line in all_lines:
		item = filter(None, line.strip().split('\t'))
		num = len(item)
		#print item
		#print num

		line_strip = (line.strip())
		item_split_component=line_strip.split('component')
	
		found=line.find('component')
		if found > 0:
			print >> file, 'component' + item_split_component[1]
		else:
			if num == 1:
				print >> file, item[0]

	parse_file.close()
	file.close()
	return


def parse_text_map(str):
	if os.path.exists('parse_text.map'):
		os.remove('parse_text.map')
	file=open('parse_text.map', 'w')

	if os.path.exists('temp_text.map'):
		os.remove('temp_text.map')
	temp_file=open('temp_text.map', 'w')
	
	text_file=open('image/text_image2_ns.map', 'r')
	
	text_all_lines = text_file.readlines()
		
	count=0
	temp_count=1;
	
	for text_line in text_all_lines:
		item = filter(None, text_line.strip().split(' '))
		num = len(item)

		line_strip = (text_line.strip())
		item_split_bank=line_strip.split(' ')
		
		found_bss=text_line.find('.bss.')
		found_bdram=text_line.find('.bdsram.')		
		found_fdram=text_line.find('.bfsram.')			
		
		if found_bss>0:
			if num==1:
				temp_count=count+1
		else:
			if (count!=temp_count) and (found_bdram<0) and (found_fdram<0):
				temp_file.write(text_line)

		count=count+1
	
	temp_file.close()
	
	parse_file=open('temp_text.map', 'r')
	parse_file = 'temp_text.map'
	
	if (str=='flash'):
		command = "grep" + " 0x0e0 " +  parse_file + " | grep -e '\.o$' -e '\.o)$'";
	else:
		command = "grep -i" + " -e 0x100 -e 0x101 " +  parse_file + " | grep -e '\.o)$' -e '\.o$'";
	
	proc = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
	
	for line in proc.stdout:
		items = filter(None, line.strip().split(' '))
		
		num = len(items) 
		if num == 3 :
			if (str=='flash'):
				if (items[0].find("0x0e")==0):
					print >> file, items[0] + " " + items[1] + " " + items[2] 
			else:
				if (items[0].find("0x100")==0 or items[0].find("0x101")==0):
					print >> file, items[0] + " " + items[1] + " " + items[2]
		if num == 4 : 
			if (str=='flash'):
				if (items[1].find("0x0e")==0 and items[0].find(".debug")!=0):
					print >> file, items[1] + " " + items[2] + " " + items[3]
			else:
				if ((items[1].find("0x100")==0 or items[1].find("0x101")==0) and items[0].find(".debug")!=0):
					print >> file, items[1] + " " + items[2] + " " + items[3]

	file.close()
	return

def parse_text_map_2(str):
	if os.path.exists('parse_text_2.map'):
		os.remove('parse_text_2.map')
	file=open('parse_text_2.map', 'w')

	parse_file = 'parse_text.map'
	first = 1
	size = 0
	
	if (str=='flash'):
		command = "grep" + " 0x0e0 " +  parse_file + " | grep -e '\.o$' -e '\.o)$'";
	else:
		command = "grep -i" + " -e 0x100 -e 0x101 " +  parse_file + " | grep -e '\.o$'  -e '\.o)$'";
	
	proc = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
	
	for line in proc.stdout:
		items = filter(None, line.strip().split(' '))
		
		if first == 1 :
			line_last = line
			size = int(items[1], 16)
			first = 0
			continue
		
		items_last = filter(None, line_last.strip().split(' '))
		
		if  items[2] == items_last[2] :
			size = size + int(items[1], 16)
		else :
			split_line=os.path.split(items_last[2])
			print >> file, hex(size) + " " + split_line[1]
			
			size = int(items[1], 16)
			line_last = line
	file.close()
	return

def merge_size_and_objs():
	if os.path.exists('parse_text_obj_size_1.map'):
		os.remove('parse_text_obj_size_1.map')
	file_result=open('parse_text_obj_size_1.map', 'w')

	parse_file = 'obj_list.txt'
	command = "grep -e '\.o)$' -e '\.o$'" +  parse_file;
	last_item='none'
	last_obj_path='none'
	obj_size=0
	file_size=0
	image_size=0
	
	proc = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
	file=open('parse_text_2.map', 'r')
	all_lines = file.readlines()

	for line in proc.stdout:
		items = filter(None, line.strip().split(' '))
		items_split=os.path.split(items[0])
		file_size=0

		line_strip = (line.strip())
		items_split_component=line_strip.split('component')
		items_split_test=line_strip.split('/', line_strip.count('/'))

		obj_path=''
		for num in xrange(17):
			obj_path += items_split_test[num]+"/"

		if (last_obj_path != obj_path):
			if (last_obj_path != 'none'):
				print >> file_result, "Total size hex: " + hex(obj_size) + " dec: %d"%obj_size
			print >> file_result, "============================================================================================"
			obj_size=0
		last_obj_path=obj_path

		for line in all_lines:
			item = filter(None, line.strip().split(' '))
			if items_split[1] == item[1]:
				file_size=file_size+int(item[0],16)
		
		print >> file_result, hex(file_size) + "	" + items_split_component[1]
		obj_size = obj_size + file_size
		image_size=image_size+file_size

	print >> file_result, "Total size hex: " + hex(obj_size) + " dec: %d"%obj_size 
	print >> file_result, "============================================================================================"
	print >> file_result, "Image size hex: " + hex(image_size) + " dec: %d"%image_size 
	file_result.close();
	file.close()
	return
	
def merge_same_objlist():
	if os.path.exists('merge_temp.map'):
		os.remove('merge_temp.map')
	merge_temp_file=open('merge_temp.map', 'w')

	file=open('parse_text_2.map', 'r')
	all_lines = file.readlines()
	
	total_size =  0
	module_size = 0
	
	merge_lable=[]
	
	for line in all_lines:
		item = filter(None, line.strip().split(' '))
		num = len(item)

		line_strip = (line.strip())
		item_split_bank=line_strip.split(' ')
		
		if item_split_bank[1] not in merge_lable:
			merge_lable.append(item_split_bank[1])
			
		
	for i in range(len(merge_lable)):
		module_size = 0
		for line in all_lines:
			item = filter(None, line.strip().split(' '))
			num = len(item)

			line_strip = (line.strip())
			item_split_bank=line_strip.split(' ')
			found=line.find(merge_lable[i])
			
			if found>=0 and len(merge_lable[i]) == len(item_split_bank[1]):
				module_size = module_size + int(item_split_bank[0],16)
		print >> merge_temp_file, str(hex(module_size)) + " " + merge_lable[i]
	
	merge_temp_file.close()
	file.close()
	return
	
def merge_size_and_objs_2(strx):
	if (strx=='flash'):
		des_file='code_size_flash.map'
	else:
		des_file='code_size_ram.map'

	if os.path.exists(des_file):
		os.remove(des_file)
	file_result=open(des_file, 'w')
	
	merge_same_objlist()
	file=open('merge_temp.map', 'r')
	all_lines = file.readlines()
	
	# file=open('parse_text_2.map', 'r')
	# all_lines = file.readlines()

	total_size =  0
	module_size = 0
	
	lable=[]
	
	for line in all_lines:
		item = filter(None, line.strip().split(' '))
		num = len(item)
		#print item
		#print num

		line_strip = (line.strip())
		item_split_bank=line_strip.split(' ')
		
		found_a=item_split_bank[1].find('.a')
		
		if found_a>0:
			item_split_bank_a=item_split_bank[1].split('.a')
			if item_split_bank_a[0] not in lable:
				lable.append(item_split_bank_a[0])
			
	for i in range(len(lable)):		
		print >> file_result, "================================================================="
		module_size = 0
		for line in all_lines:
			item = filter(None, line.strip().split(' '))
			num = len(item)

			line_strip = (line.strip())
			item_split_bank=line_strip.split(' ')

			found=line.find(lable[i])

			if (found>0):
				module_size = module_size + int(item_split_bank[0],16)
				total_size += int(item_split_bank[0],16)
				stest = str(int(item_split_bank[0],16))
				print >> file_result, item_split_bank[0] + "\t\t\t" + stest + "\t\t\t" + item_split_bank[1]
				
		print >> file_result, "\n" + "total:"
		print >> file_result, str(hex(module_size)) + "\t\t\t" + str(module_size) + '\n'		
	
	if os.path.exists('sort_temp_file.map'):
		os.remove('sort_temp_file.map')
	file_sort_temp=open('sort_temp_file.map', 'w')
	
	for line in all_lines:
		item = filter(None, line.strip().split(' '))
		line_strip = (line.strip())
		item_split_bank=line_strip.split(' ')

		found=line.find('.a')

		if (found<0):
			stest = str(int(item_split_bank[0],16))
			print >> file_sort_temp, item_split_bank[0] + " " + stest + " " + item_split_bank[1]
	file_sort_temp.close()

	sort_lable_wifi=['wifi_','lwip_netconf', 'wifi_interactive_mode', 'atcmd_sys','atcmd_wifi', 'tcptest', 'wlan_', 'ping_']
	sort_lable_per=['rtl8721d', 'platform', 'main', 'example_entry', '_api.']
	sort_lable_shell=['shell', 'log_', 'monitor', 'low_level_io', 'rtl_trace']
	sort_lable_os=['croutine', 'event_groups', 'list', 'queue', 'tasks', 'timers', 'heap_5', 'port', 'freertos_', 'osdep_', 'device_lock']
	
	file_sort_temp_r=open('sort_temp_file.map', 'r')
	all_lines_sort = file_sort_temp_r.readlines()

	array_flag=[0 for i in range(len(all_lines_sort))]
	
	temp_flag=0
	
	print >> file_result, "================================================================="
	module_size=0
	for all_lines_sort_t in all_lines_sort:
		item = filter(None, all_lines_sort_t.strip().split(' '))
		num = len(item)

		line_strip = (all_lines_sort_t.strip())
		item_split_bank=line_strip.split(' ')
		temp_flag=0
		for idxn in sort_lable_wifi:
			found = item_split_bank[2].find(idxn)
			if found>=0:
				temp_flag=1
				break
		if (temp_flag==1):
			if(array_flag[all_lines_sort.index(all_lines_sort_t)]==0):
				module_size = module_size + int(item_split_bank[0],16)
				total_size += int(item_split_bank[0],16)
				stest = str(int(item_split_bank[0],16))
				print >> file_result, item_split_bank[0] + "\t\t\t" + stest + "\t\t\t" + item_split_bank[2]
				array_flag[all_lines_sort.index(all_lines_sort_t)]=1
	if module_size!=0:
		print >> file_result, "\n" + "total:"
		print >> file_result, str(hex(module_size)) + "\t\t\t" + str(module_size) + '\n'		
		print >> file_result, "================================================================="
	file_sort_temp_r.close()
	
	file_sort_temp_r=open('sort_temp_file.map', 'r')
	all_lines_sort = file_sort_temp_r.readlines()		
	module_size=0
	for all_lines_sort_t in all_lines_sort:
		item = filter(None, all_lines_sort_t.strip().split(' '))
		num = len(item)

		line_strip = (all_lines_sort_t.strip())
		item_split_bank=line_strip.split(' ')
		
		temp_flag=0
		for idxn in sort_lable_per:
			found = item_split_bank[2].find(idxn)
			if found>=0:
				temp_flag=1
				break
		if (temp_flag==1):
			if(array_flag[all_lines_sort.index(all_lines_sort_t)]==0):
				module_size = module_size + int(item_split_bank[0],16)
				total_size += int(item_split_bank[0],16)
				stest = str(int(item_split_bank[0],16))
				print >> file_result, item_split_bank[0] + "\t\t\t" + stest + "\t\t\t" + item_split_bank[2]
				array_flag[all_lines_sort.index(all_lines_sort_t)]=1
	if module_size!=0:
		print >> file_result, "\n" + "total:"
		print >> file_result, str(hex(module_size)) + "\t\t\t" + str(module_size) + '\n'		
		print >> file_result, "================================================================="

	file_sort_temp_r.close()
	
	file_sort_temp_r=open('sort_temp_file.map', 'r')
	all_lines_sort = file_sort_temp_r.readlines()		
	module_size=0
	for all_lines_sort_t in all_lines_sort:
		item = filter(None, all_lines_sort_t.strip().split(' '))
		num = len(item)

		line_strip = (all_lines_sort_t.strip())
		item_split_bank=line_strip.split(' ')
		
		temp_flag=0
		for idxn in sort_lable_shell:
			found = item_split_bank[2].find(idxn)
			if found>=0:
				temp_flag=1
				break
		if (temp_flag==1):
			if(array_flag[all_lines_sort.index(all_lines_sort_t)]==0):
				module_size = module_size + int(item_split_bank[0],16)
				total_size += int(item_split_bank[0],16)
				stest = str(int(item_split_bank[0],16))
				print >> file_result, item_split_bank[0] + "\t\t\t" + stest + "\t\t\t" + item_split_bank[2]
				array_flag[all_lines_sort.index(all_lines_sort_t)]=1
	if module_size!=0:
		print >> file_result, "\n" + "total:"
		print >> file_result, str(hex(module_size)) + "\t\t\t" + str(module_size) + '\n'		
		print >> file_result, "================================================================="

	file_sort_temp_r.close()

	file_sort_temp_r=open('sort_temp_file.map', 'r')
	all_lines_sort = file_sort_temp_r.readlines()		
	module_size=0
	for all_lines_sort_t in all_lines_sort:
		item = filter(None, all_lines_sort_t.strip().split(' '))
		num = len(item)

		line_strip = (all_lines_sort_t.strip())
		item_split_bank=line_strip.split(' ')
		
		temp_flag=0
		for idxn in sort_lable_os:
			found = item_split_bank[2].find(idxn)
			if found>=0:
				if idxn=='timers':
					if found==0:
						temp_flag=1
						break
				else:
					temp_flag=1
					break
		if (temp_flag==1):
			if(array_flag[all_lines_sort.index(all_lines_sort_t)]==0):
				module_size = module_size + int(item_split_bank[0],16)
				total_size += int(item_split_bank[0],16)
				stest = str(int(item_split_bank[0],16))
				print >> file_result, item_split_bank[0] + "\t\t\t" + stest + "\t\t\t" + item_split_bank[2]
				array_flag[all_lines_sort.index(all_lines_sort_t)]=1
	if module_size!=0:
		print >> file_result, "\n" + "total:"
		print >> file_result, str(hex(module_size)) + "\t\t\t" + str(module_size) + '\n'		
		print >> file_result, "================================================================="

	file_sort_temp_r.close()
	
	file_sort_temp_r=open('sort_temp_file.map', 'r')
	all_lines_sort = file_sort_temp_r.readlines()		
	module_size=0
	for all_lines_sort_t in all_lines_sort:
		item = filter(None, all_lines_sort_t.strip().split(' '))
		num = len(item)

		line_strip = (all_lines_sort_t.strip())
		item_split_bank=line_strip.split(' ')
		
		temp_flag=0
		for idxn in sort_lable_per:
			found = item_split_bank[2].find(idxn)
			if found>=0:
				temp_flag=1
				break
		for idxn in sort_lable_wifi:
			found = item_split_bank[2].find(idxn)
			if found>=0:
				temp_flag=1
				break
				
		for idxn in sort_lable_shell:
			found = item_split_bank[2].find(idxn)
			if found>=0:
				temp_flag=1
				break
		for idxn in sort_lable_os:
			found = item_split_bank[2].find(idxn)
			if found>=0:
				if idxn=='timers':
					if found==0:
						temp_flag=1
						break
				else:
					temp_flag=1
					break
				
		if (temp_flag==0):
			if(array_flag[all_lines_sort.index(all_lines_sort_t)]==0):
				module_size = module_size + int(item_split_bank[0],16)
				total_size += int(item_split_bank[0],16)
				stest = str(int(item_split_bank[0],16))
				print >> file_result, item_split_bank[0] + "\t\t\t" + stest + "\t\t\t" + item_split_bank[2]
				array_flag[all_lines_sort.index(all_lines_sort_t)]=1

	if module_size!=0:
		print >> file_result, "\n" + "total:"
		print >> file_result, str(hex(module_size)) + "\t\t\t" + str(module_size) + '\n'		
		print >> file_result, "================================================================="

	file_sort_temp_r.close()
	
	module_size=0
	if module_size!=0:
		print >> file_result, "================================================================="
	print >> file_result, "\n" + "Image size:"
	print >> file_result, str(hex(total_size)) + "\t\t\t" + str(total_size) + '\n'	
	
	file_result.close()
	file.close()
	
	return
	
if os.path.exists('image/text_image2_ns.map'):
	obj_list_gen()

	# flash code and data
	parse_text_map('flash')
	parse_text_map_2('flash')
	merge_size_and_objs_2('flash')

	#ram code and data
	parse_text_map('ram')
	parse_text_map_2('ram')
	merge_size_and_objs_2('ram')

	#remove temp files
	os.remove('obj_list.map')
	os.remove('parse_text.map')
	os.remove('parse_text_2.map')
	os.remove('merge_temp.map')
	os.remove('temp_text.map')
	os.remove('sort_temp_file.map')

	if os.path.exists('image/code_size_ram.map'):
		os.remove('image/code_size_ram.map')
	if os.path.exists('image/code_size_flash.map'):
		os.remove('image/code_size_flash.map')
	shutil.move('code_size_ram.map', 'image')
	shutil.move('code_size_flash.map', 'image')
