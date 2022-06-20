#!/usr/bin/env python

from __future__ import print_function
import sys
import subprocess
import os
import re
import shutil


flash_start = '__flash_text_start__ = .'
flash_end = '__flash_text_end__ = .'

ram_start = '__ram_text_start__ '
ram_end = '__ram_text_end__ = .'

image_start='__ram_image2_text_start__ = .'
image_end='__ram_image2_text_end__ = .'

bss_start='__bss_start__ = .'
bss_end='__bss_end__ = .'

def obj_list_gen():
    if os.path.exists('obj_list.map'):
        os.remove('obj_list.map')
    file = open('obj_list.map', 'w')

    parse_file = open('image/ram_size.txt', 'r')
    all_lines = parse_file.readlines()

    for line in all_lines:
        item = list(filter(None, line.strip().split('\t')))
        num = len(item)


        line_strip = (line.strip())
        item_split_component = line_strip.split('component')

        found = line.find('component')
        if found > 0:
            print('component' + item_split_component[1], file=file)
        else:
            if num == 1:
                print(item[0], file=file)

    parse_file.close()
    file.close()
    return



def parse_text_map(str):

    if os.path.exists('parse_text.map'):
        os.remove('parse_text.map')
    file = open('parse_text.map', 'w')

    if os.path.exists('temp_text.map'):
        os.remove('temp_text.map')
    temp_file = open('temp_text.map', 'w')

    text_file = open('image/text_image2_ns.map', 'r')

    text_all_lines = text_file.readlines()

    IF_Write = False
    if str == 'ram':
        for text_line in text_all_lines:
            if text_line.find(ram_start) > 0:
                    IF_Write = True

            if text_line.find(ram_end) > 0:
                    IF_Write = False

            if IF_Write:
                temp_file.write(text_line)
    elif str == 'flash':
        for text_line in text_all_lines:
            if text_line.find(flash_start) > 0:
                IF_Write = True

            if text_line.find(flash_end) > 0:
                IF_Write = False

            if IF_Write:
                temp_file.write(text_line)
    elif str == 'BSS':
        for text_line in text_all_lines:
            if text_line.find(bss_start) > 0:
                IF_Write = True

            if text_line.find(bss_end) > 0:
                IF_Write = False

            if IF_Write:
                temp_file.write(text_line)
    elif str == 'image2':
        for text_line in text_all_lines:
            if text_line.find(image_start) > 0:
                IF_Write = True

            if text_line.find(image_end) > 0:
                IF_Write = False

            if IF_Write:
                temp_file.write(text_line)
    temp_file.close()
    parse_file = 'temp_text.map'
    command = "grep -i" + " -e 0x " + parse_file + " | grep -e '\.o)$' -e '\.o$'"
    proc = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)

    #
    for line in proc.stdout:
        line = line.decode()
        items = list(filter(None, line.strip().split(' ')))

        num = len(items)
        if num == 3:
            if (items[0].find('0x') == 0 and (items[-1].endswith('.o') or items[-1].endswith('.o)'))):
                print(items[0] + " " + items[1] + " " + items[2], file=file)

        if num == 4:
            if (items[1].find('0x') == 0 and (items[-1].endswith('.o') or items[-1].endswith('.o)'))):
                print(items[1] + " " + items[2] + " " + items[3], file=file)
    #
    file.close()
    return



def parse_text_map_2(str):
    if os.path.exists('parse_text_2.map'):
        os.remove('parse_text_2.map')
    file = open('parse_text_2.map', 'w')

    parse_file = 'parse_text.map'
    first = 1
    size = 0
    command = "grep -i" + " -e 0x " + parse_file + " | grep -e '\.o)$' -e '\.o$'"
    proc = subprocess.Popen(command, stdout=subprocess.PIPE, shell=True)
    filename_list = []
    size_dict = {}
    #
    for i, line in enumerate(proc.stdout):
        line = line.decode()
        items = list(filter(None, line.strip().split(' ')))
        name = os.path.split(items[2])[1]
        if name not in filename_list:
            size_dict[name] = 0 + int(items[1], 16)
            filename_list.append(name)
        else:
            size_dict[name] = size_dict[name] + int(items[1], 16)

    for key in filename_list:
        print(hex(size_dict[key]) + " " + key, file=file)

    file.close()
    return


def merge_size_and_objs_2(strx):
    if (strx == 'flash'):
        des_file = 'code_size_flash.map'
    elif(strx == 'ram'):
        des_file = 'code_size_ram.map'
    elif (strx == 'BSS'):
        des_file = 'code_size_BSS.map'
    elif (strx == 'image2'):
        des_file = 'code_size_image2.map'

    if os.path.exists(des_file):
        os.remove(des_file)
    file_result = open(des_file, 'w')

    file = open('parse_text_2.map', 'r')
    all_lines = file.readlines()
    total_size = 0
    module_size = 0
    #
    lable = []
    #
    for line in all_lines:
        item = list(filter(None, line.strip().split(' ')))
        num = len(item)

        #
        line_strip = (line.strip())
        item_split_bank = line_strip.split(' ')

        found_a = item_split_bank[1].find('.a')

        if found_a > 0:
            item_split_bank_a = item_split_bank[1].split('.a')
            if item_split_bank_a[0] not in lable:
                lable.append(item_split_bank_a[0])

    for i in range(len(lable)):
        print("=================================================================", file=file_result)
        module_size = 0
        for line in all_lines:
            item = list(filter(None, line.strip().split(' ')))
            num = len(item)

            line_strip = (line.strip())
            item_split_bank = line_strip.split(' ')

            found = line.find(lable[i])

            if (found > 0):
                module_size = module_size + int(item_split_bank[0], 16)
                total_size += int(item_split_bank[0], 16)
                stest = str(int(item_split_bank[0], 16))
                print(item_split_bank[0] + "\t\t\t" + stest + "\t\t\t" + item_split_bank[1], file=file_result)

        print("\n" + "total:", file=file_result)
        print(str(hex(module_size)) + "\t\t\t" + str(module_size) + '\n', file=file_result)

    if os.path.exists('sort_temp_file.map'):
        os.remove('sort_temp_file.map')
    file_sort_temp = open('sort_temp_file.map', 'w')

    for line in all_lines:
        item = filter(None, line.strip().split(' '))
        line_strip = (line.strip())
        item_split_bank = line_strip.split(' ')

        found = line.find('.a')

        if (found < 0):
            stest = str(int(item_split_bank[0], 16))
            print(item_split_bank[0] + " " + stest + " " + item_split_bank[1], file=file_sort_temp)
    file_sort_temp.close()

    sort_lable_wifi = ['wifi_', 'lwip_netconf', 'wifi_interactive_mode', 'atcmd_sys', 'atcmd_wifi', 'tcptest', 'wlan_',
                       'ping_']
    sort_lable_per = ['rtl8721d', 'platform', 'main', 'example_entry', '_api.']
    sort_lable_shell = ['shell', 'log_', 'monitor', 'low_level_io', 'rtl_trace']
    sort_lable_os = ['croutine', 'event_groups', 'list', 'queue', 'tasks', 'timers', 'heap_5', 'port', 'freertos_',
                     'osdep_', 'device_lock']

    file_sort_temp_r = open('sort_temp_file.map', 'r')
    all_lines_sort = file_sort_temp_r.readlines()

    array_flag = [0 for i in range(len(all_lines_sort))]

    temp_flag = 0

    print("=================================================================", file=file_result)
    module_size = 0
    for all_lines_sort_t in all_lines_sort:
        item = list(filter(None, all_lines_sort_t.strip().split(' ')))
        num = len(item)

        line_strip = (all_lines_sort_t.strip())
        item_split_bank = line_strip.split(' ')
        temp_flag = 0
        for idxn in sort_lable_wifi:
            found = item_split_bank[2].find(idxn)
            if found >= 0:
                temp_flag = 1
                break
        if (temp_flag == 1):
            if (array_flag[all_lines_sort.index(all_lines_sort_t)] == 0):
                module_size = module_size + int(item_split_bank[0], 16)
                total_size += int(item_split_bank[0], 16)
                stest = str(int(item_split_bank[0], 16))
                print(item_split_bank[0] + "\t\t\t" + stest + "\t\t\t" + item_split_bank[2], file=file_result)
                array_flag[all_lines_sort.index(all_lines_sort_t)] = 1
    if module_size != 0:
        print("\n" + "total:", file=file_result)
        print(str(hex(module_size)) + "\t\t\t" + str(module_size) + '\n', file=file_result)
        print("=================================================================", file=file_result)
    file_sort_temp_r.close()

    file_sort_temp_r = open('sort_temp_file.map', 'r')
    all_lines_sort = file_sort_temp_r.readlines()
    module_size = 0
    for all_lines_sort_t in all_lines_sort:
        item = list(filter(None, all_lines_sort_t.strip().split(' ')))
        num = len(item)

        line_strip = (all_lines_sort_t.strip())
        item_split_bank = line_strip.split(' ')

        temp_flag = 0
        for idxn in sort_lable_per:
            found = item_split_bank[2].find(idxn)
            if found >= 0:
                temp_flag = 1
                break
        if (temp_flag == 1):
            if (array_flag[all_lines_sort.index(all_lines_sort_t)] == 0):
                module_size = module_size + int(item_split_bank[0], 16)
                total_size += int(item_split_bank[0], 16)
                stest = str(int(item_split_bank[0], 16))
                print(item_split_bank[0] + "\t\t\t" + stest + "\t\t\t" + item_split_bank[2], file=file_result)
                array_flag[all_lines_sort.index(all_lines_sort_t)] = 1
    if module_size != 0:
        print("\n" + "total:", file=file_result)
        print(str(hex(module_size)) + "\t\t\t" + str(module_size) + '\n', file=file_result)
        print("=================================================================", file=file_result)

    file_sort_temp_r.close()

    file_sort_temp_r = open('sort_temp_file.map', 'r')
    all_lines_sort = file_sort_temp_r.readlines()
    module_size = 0
    for all_lines_sort_t in all_lines_sort:
        item = list(filter(None, all_lines_sort_t.strip().split(' ')))
        num = len(item)

        line_strip = (all_lines_sort_t.strip())
        item_split_bank = line_strip.split(' ')

        temp_flag = 0
        for idxn in sort_lable_shell:
            found = item_split_bank[2].find(idxn)
            if found >= 0:
                temp_flag = 1
                break
        if (temp_flag == 1):
            if (array_flag[all_lines_sort.index(all_lines_sort_t)] == 0):
                module_size = module_size + int(item_split_bank[0], 16)
                total_size += int(item_split_bank[0], 16)
                stest = str(int(item_split_bank[0], 16))
                print(item_split_bank[0] + "\t\t\t" + stest + "\t\t\t" + item_split_bank[2], file=file_result)
                array_flag[all_lines_sort.index(all_lines_sort_t)] = 1
    if module_size != 0:
        print("\n" + "total:", file=file_result)
        print(str(hex(module_size)) + "\t\t\t" + str(module_size) + '\n', file=file_result)
        print("=================================================================", file=file_result)

    file_sort_temp_r.close()

    file_sort_temp_r = open('sort_temp_file.map', 'r')
    all_lines_sort = file_sort_temp_r.readlines()
    module_size = 0
    for all_lines_sort_t in all_lines_sort:
        item = list(filter(None, all_lines_sort_t.strip().split(' ')))
        num = len(item)

        line_strip = (all_lines_sort_t.strip())
        item_split_bank = line_strip.split(' ')

        temp_flag = 0
        for idxn in sort_lable_os:
            found = item_split_bank[2].find(idxn)
            if found >= 0:
                if idxn == 'timers':
                    if found == 0:
                        temp_flag = 1
                        break
                else:
                    temp_flag = 1
                    break
        if (temp_flag == 1):
            if (array_flag[all_lines_sort.index(all_lines_sort_t)] == 0):
                module_size = module_size + int(item_split_bank[0], 16)
                total_size += int(item_split_bank[0], 16)
                stest = str(int(item_split_bank[0], 16))
                print(item_split_bank[0] + "\t\t\t" + stest + "\t\t\t" + item_split_bank[2], file=file_result)
                array_flag[all_lines_sort.index(all_lines_sort_t)] = 1
    if module_size != 0:
        print("\n" + "total:", file=file_result)
        print(str(hex(module_size)) + "\t\t\t" + str(module_size) + '\n', file=file_result)
        print("=================================================================", file=file_result)

    file_sort_temp_r.close()

    file_sort_temp_r = open('sort_temp_file.map', 'r')
    all_lines_sort = file_sort_temp_r.readlines()
    module_size = 0
    for all_lines_sort_t in all_lines_sort:
        item = list(filter(None, all_lines_sort_t.strip().split(' ')))
        num = len(item)

        line_strip = (all_lines_sort_t.strip())
        item_split_bank = line_strip.split(' ')

        temp_flag = 0
        for idxn in sort_lable_per:
            found = item_split_bank[2].find(idxn)
            if found >= 0:
                temp_flag = 1
                break
        for idxn in sort_lable_wifi:
            found = item_split_bank[2].find(idxn)
            if found >= 0:
                temp_flag = 1
                break

        for idxn in sort_lable_shell:
            found = item_split_bank[2].find(idxn)
            if found >= 0:
                temp_flag = 1
                break
        for idxn in sort_lable_os:
            found = item_split_bank[2].find(idxn)
            if found >= 0:
                if idxn == 'timers':
                    if found == 0:
                        temp_flag = 1
                        break
                else:
                    temp_flag = 1
                    break

        if (temp_flag == 0):
            if (array_flag[all_lines_sort.index(all_lines_sort_t)] == 0):
                module_size = module_size + int(item_split_bank[0], 16)
                total_size += int(item_split_bank[0], 16)
                stest = str(int(item_split_bank[0], 16))
                print(item_split_bank[0] + "\t\t\t" + stest + "\t\t\t" + item_split_bank[2], file=file_result)
                array_flag[all_lines_sort.index(all_lines_sort_t)] = 1

    if module_size != 0:
        print("\n" + "total:", file=file_result)
        print(str(hex(module_size)) + "\t\t\t" + str(module_size) + '\n', file=file_result)
        print("=================================================================", file=file_result)

    file_sort_temp_r.close()

    module_size = 0
    if module_size != 0:
        print("=================================================================", file=file_result)
    print("\n" + "Image size:", file=file_result)
    print(str(hex(total_size)) + "\t\t\t" + str(total_size) + '\n', file=file_result)

    file_result.close()
    file.close()

    return


if os.path.exists('image/text_image2_ns.map'):
    obj_list_gen()
    str_list=['flash','ram','BSS','image2']
    for strr in str_list:
        parse_text_map(strr)
        parse_text_map_2(strr)
        merge_size_and_objs_2(strr)

    # remove temp files
    os.remove('obj_list.map')
    os.remove('parse_text.map')
    os.remove('parse_text_2.map')
    os.remove('temp_text.map')
    os.remove('sort_temp_file.map')

    if os.path.exists('image/code_size_ram.map'):
        os.remove('image/code_size_ram.map')
    if os.path.exists('image/code_size_flash.map'):
        os.remove('image/code_size_flash.map')
    if os.path.exists('image/code_size_BSS.map'):
        os.remove('image/code_size_BSS.map')
    if os.path.exists('image/code_size_image2.map'):
        os.remove('image/code_size_image2.map')
    shutil.move('code_size_ram.map', 'image')
    shutil.move('code_size_flash.map', 'image')
    shutil.move('code_size_BSS.map', 'image')
    shutil.move('code_size_image2.map', 'image')



