#! /usr/bin/python
import os
import sys
import subprocess

def Excute(cmd):
    popen = subprocess.Popen(cmd, stdout=subprocess.PIPE,
                             universal_newlines=True)
    stdout = popen.communicate()[0]
    print(stdout)


def ReadFile():
    file_list = ['gcov']
    with open("object_list.txt", "r") as f:
        while True:
            line = f.readline()
            if not line:
                break
            file_list.append(line[:-1])
    print(file_list)
    Excute(file_list)


# gcda, gcno
def DeleteFile():
    print("Delete files\n")
    rm_cmd = ['rm']
    gcda_list = []
    gcno_list = []
    with open('object_list.txt', "r") as f:
        while True:
            line = f.readline()
            if not line: break
            gcda_list.append(line[:-1].replace(".c", ".gcda"))
            gcno_list.append(line[:-1].replace(".c", ".gcno"))
    rm_gcda_list = rm_cmd + gcda_list
    rm_gcno_list = rm_cmd + gcno_list
    Excute(rm_gcda_list)
    Excute(rm_gcno_list)
    os.system("rm ./*.gcov")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Invalid Usage")

    if sys.argv[1] == '1':
        ReadFile()
    else:
        DeleteFile()

