#!/bin/bash
###########################################################################
#
# Copyright 2022 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
###########################################################################
# File 	 : dat.py

from distutils.log import error
from time import time
import sys
import tkinter as tk
from tkinter import *
from tkinter import ttk
import os
from click import command
from matplotlib.pyplot import text
import serial
from pynput.keyboard import Key,Controller
from tkinter.scrolledtext import ScrolledText
from tkinter import messagebox
from serial.serialutil import SerialException

def usage():
    print('*************************************************************')
    print('\nUsage: %s <device port>' % sys.argv[0])
    print('')
    print('syntax :')
    print('--------')
    print('python3 %s <device port>' % sys.argv[0])
    print('')
    print('Below example if you give device port as argument: ')
    print('--------------------------------------------')
    print('python3 %s /dev/ttyUSB1' % sys.argv[0])
    print('--------------------------------------------\n')
    print('*************************************************************')

#function use to setup the port Connection
def setupPort(userport):
    try:
        PORT = serial.Serial(userport, baudrate=115200, timeout=0.1)
        return PORT
    except SerialException:
        ErrorMsg = f"Failed to estabish UART connection for PORT '" + userport + "' Please check if the device  is connected to port or the permissions for port are correct"
        messagebox.showerror("showerror", ErrorMsg)

#function to get custom output
def getCustomOutputFromPort(command):
    PORT=setupPort(port)
    PORT.write((command).encode())
    PORT.write(("\n").encode())
    try:
        lines = PORT.readlines()
    except SerialException:
        ErrorMsg = f"Device disconnected or multiple port access"
        messagebox.showerror("showerror", ErrorMsg)

    output=""
    for line in lines:
        tmp = line.strip().lower()
        tmp=str(line.decode('UTF-8'))
        output+=tmp
    if  "Select Test Scenario." in output:
        PORT.write(("x").encode())
        PORT.write(("\n").encode())
        ErrorMsg = f"Terminating Test Mode of MicroController . Please Try again."
        messagebox.showerror("showerror", ErrorMsg)
        return
    if "not registered" in output:
        ErrorMsg = "'" + command + "'" " Command is not registered\n Enable appropriate command on the device and retry!"
        messagebox.showerror("showerror", ErrorMsg)
        return

    # print command output
    file = open(path+"/cmd_output.txt",'w+')
    file.write(output)
    file.close()
    newfile = open(path+'/cmd_output.txt', 'r')
    new = ''
    new += newfile.read()
    newfile.close()
    new = new.encode("utf-8", "ignore")

    st.configure(state="normal")
    st.insert("1.0","\n-------------------------------------------------------------\n")
    st.configure(state="disabled")

    st.configure(state="normal")
    st.insert("1.0",new)
    st.configure(state="disabled")

#function for custom user input
def customIp(ct):
    #custom input TextBox
    command = ct.get("1.0","end-1c")
    s=command.strip()
    getCustomOutputFromPort(s)

#function to setup frame1
def setupframe1():

    #aliveThreadButton in firstFrame
    aliveThreadListBtn = Button(frame, text = "Alive Thread List", command = aliveThreadList, bg = "grey", fg = "black")
    aliveThreadListBtn.grid()

    #schedulingList Button in firstFrame
    schedulingListBtn = Button(frame, text = " Scheduling List ", padx = 50, pady = 5, command = schedulingList, bg = "grey", fg = "black")
    schedulingListBtn.grid()

    #loadinglist Button in firstFrame
    loadingListBtn = Button(frame, text = " Loading  List   ", padx = 50, pady = 5, command = loadingList, bg = "grey", fg = "black")
    loadingListBtn.grid()

    #heapusageList Button in firstFrame
    heapUsageBtn= Button(frame, text = "Heap Usage", padx = 50, pady = 5, command = heapInfo, bg = "grey", fg = "black")
    heapUsageBtn.grid()

    #packing into grid of first frame
    aliveThreadListBtn.grid(row = 0, column = 0, padx = 10, pady = 10, sticky = EW)
    schedulingListBtn.grid(row = 2, column = 0, padx = 10, pady = 10, sticky = EW)
    loadingListBtn.grid(row = 6, column = 0, padx = 10, pady = 10, sticky = EW)
    heapUsageBtn.grid(row = 8, column = 0, padx = 10, pady = 10, sticky = EW)

#function to setup frame3
def setupFrame3():
    ct = Text(frame3, width = 50, height = 3)
    ct.pack(side=tk.LEFT,fill = X,expand=True, padx = 5)
    ct.configure(wrap=None)

    runBtn = Button(frame3, text = "Run", padx = 50, pady = 5, command = lambda:customIp(ct), bg = "grey", fg = "black")
    runBtn.pack(side = tk.LEFT , fill = X, padx = 5)

def myclick():
    st.configure(state="normal")
    st.delete("1.0","end")
    st.configure(state="normal")
    st.insert("1.0","Sample Text")
    st.configure(state="disabled")

#function use to take output from Device on specific button pressed in GUI
def getOutputFromPort(command,startRegex,endRegex):
    PORT=setupPort(port)
    PORT.write((command).encode())
    PORT.write(("\n").encode())
    try:
        lines = PORT.readlines()
    except SerialException:
        ErrorMsg = f"Device disconnected or multiple port access"
        messagebox.showerror("showerror", ErrorMsg)

    output=""
    for line in lines:
        tmp = line.strip().lower()
        output+=str(line.decode('UTF-8'))
    if  "Select Test Scenario." in output:
        PORT.write(("x").encode())
        PORT.write(("\n").encode())
        ErrorMsg = f"Terminating Test Mode of MicroController . Please Try again."
        messagebox.showerror("showerror", ErrorMsg)
        return
    if "not registered" in output:
        ErrorMsg = "'" + command + "'" " Command is not registered\n Enable appropriate command on the device and retry!"
        messagebox.showerror("showerror", ErrorMsg)
        return

    file = open(path+"/cmd_output.txt",'w+')
    file.write(output)
    file.close()
    newfile = open(path+'/cmd_output.txt', 'r')
    if os.path.exists(path+'/cmd_history.txt'):
        prevfile = open(path+'/cmd_history.txt', 'r')
    else:
        prevfile = open(path+'/cmd_history.txt', 'w+')
    new = ''
    new += newfile.read()

    prev = ''
    prev += prevfile.read()
    prevfile.close()

    prevfile=open(path+'/cmd_history.txt','w')

    if startRegex in new and endRegex in new:
        start = new.index(startRegex)
        end = new.index(endRegex)
        formatedNew = new[start:end]

        temp = formatedNew

        new = temp+ "\n-------------------------------------------------------------------""\n" + prev

        prevfile.write(new)
        prevfile.close()

        writeToTextView(new)
    else:
        prevfile.write(prev)
        prevfile.close()

#function use to setup alive thread list submenu  in frame 2
def aliveThreadList():

    #function for stack Range
    def aliveList():
        getOutputFromPort("dat tcb",'TCB Information >>','test_gtasklist_info')

    #function for currentSp
    def gtaskList():
        getOutputFromPort("dat tcb",'\ntest_gtasklist_info: <', 'tasklist_stackdump')

    #function for stack peak
    def callStack():
        getOutputFromPort("dat tcb",'>\ntasklist_stackdump:','End of TCB Information')

    clear()

    frame2.config(text = "Alive Thread list SubMenu")

    aliveThreadListBtn = Button(frame2, text = "Alive Thread List", padx = 50, pady = 5, command = aliveList, bg = "grey", fg = "black")
    aliveThreadListBtn.grid(row = 0, column = 0, padx = 10, pady = 10, sticky = EW)

    gtaskListBtn = Button(frame2, text = " GtaskList Table ", padx = 50, pady = 5, command = gtaskList, bg = "grey", fg = "black")
    gtaskListBtn.grid(row = 1, column = 0, padx = 10, pady = 10, sticky = EW)

    callStackBtn = Button(frame2, text = " Alive List Call Stack ", padx = 50, pady = 5, command = callStack, bg = "grey", fg = "black")
    callStackBtn.grid(row = 2, column = 0, padx = 10, pady = 10, sticky = EW)

#function use to setup scheduling list submenu  in frame 2
def schedulingList():

    def taskList():
        getOutputFromPort("sysdbg read",'sysdbg_print: Displaying the TASK SCHEDULING HISTORY','************************ End of TASK SCHEDULING HISTORY ************************')

    def irq():
        getOutputFromPort("sysdbg read",'sysdbg_print: Displaying the IRQ SCHEDULING HISTORY','*End of IRQ SCHEDULING HISTORY*')

    def semaphoreList():
        getOutputFromPort("sysdbg read",'sysdbg_print: Displaying the SEMAPHORE HISTORY','********************** End of SEMAPHORE SCHEDULING HISTORY **********************')

    def taskIrq():
        getOutputFromPort("sysdbg read",'sysdbg_print: Displaying the (TASK + IRQ) SCHEDULING HISTORY',
                '*********************************** End of (TASK + IRQ) SCHEDULING HISTORY ************************************')

    clear()

    frame2.config(text = "Scheduling list SubMenu")

    taskListBtn = Button(frame2, text = "Task List", padx = 50, pady = 5, command = taskList, bg = "grey", fg = "black")
    taskListBtn.grid(row = 0, column = 0, padx = 10, pady = 10, sticky = EW)

    irqBtn = Button(frame2, text = "IRQ List", padx = 50, pady = 5, command = irq, bg = "grey", fg = "black")
    irqBtn.grid(row = 1, column = 0, padx = 10, pady = 10, sticky = EW)

    semaphoreListBtn = Button(frame2, text = "Semaphore List", padx = 50, pady = 5, command = semaphoreList, bg = "grey", fg = "black")
    semaphoreListBtn.grid(row = 2, column = 0, padx = 10, pady = 10, sticky = EW)

    taskIrqBtn = Button(frame2, text = "Task+IRQ List", padx = 50, pady = 5, command = taskIrq, bg = "grey", fg = "black")
    taskIrqBtn.grid(row = 3, column = 0, padx = 10, pady = 10, sticky = EW)

#function for loading list submenu in frame2
def loadingList():

    def noOfBinary():
        getOutputFromPort("dat apps",'Number of binaries :','App[1] runtime :')

    def runTime():
        getOutputFromPort("dat apps",'App[1] runtime :','\ntest_get_app_memory_addr:')

    def textRo():
        getOutputFromPort("dat apps",'test_get_app_memory_addr: Memory information','End of Application Information\n')

    def heapInfoSubmenu():
        getOutputFromPort("heapinfo -b app2",'app2 HEAP INFORMATION\n','** NOTE **')
        getOutputFromPort("heapinfo -b app1",'app1 HEAP INFORMATION\n','** NOTE **')
        getOutputFromPort("heapinfo",'KERNEL HEAP INFORMATION\n','\nTASH')

    clear()

    frame2.configure(text = "Loading List Submenu")

    noOfBinaryBtn = Button(frame2, text = "No. of Binaries", padx = 50, pady = 5, command = noOfBinary, bg = "grey", fg = "black")
    noOfBinaryBtn.grid(row = 0, column = 0, padx = 10, pady = 10, sticky=EW)

    textRoBtn = Button(frame2, text = " Memory(Text,Ro,bss,data) ", padx = 50, pady = 5, command = textRo, bg = "grey", fg = "black")
    textRoBtn.grid(row = 1, column = 0, padx = 10, pady = 10, sticky = EW)

    heapInfoBtn = Button(frame2, text = " Application Runtimes ", padx = 50, pady = 5, command = runTime, bg = "grey", fg = "black")
    heapInfoBtn.grid(row = 2, column = 0, padx = 10, pady = 10, sticky = EW)

    heapInfoBtn = Button(frame2, text = " Heap Info ", padx = 50, pady = 5, command = heapInfoSubmenu, bg = "grey", fg = "black")
    heapInfoBtn.grid(row = 3, column = 0, padx = 10, pady = 10, sticky = EW)

#function for loading heapInfo submenu in frame2
def heapInfo():

    def kernal():
        getOutputFromPort("heapinfo",'KERNEL HEAP INFORMATION\n','\nTASH')

    def app1():
        getOutputFromPort("heapinfo -b app1",'app1 HEAP INFORMATION\n','** NOTE **')

    def app2():
        getOutputFromPort("heapinfo -b app2",'app2 HEAP INFORMATION\n','** NOTE **')

    clear()

    frame2.configure(text = "Heap Usage Submenu")

    kernalBtn = Button(frame2, text = "Kernal", padx = 50, pady = 5, command = kernal, bg = "grey", fg = "black")
    kernalBtn.grid(row = 0, column = 0, padx = 10, pady = 10, sticky = EW)

    app1Heap= Button(frame2, text = " APP 1 ", padx = 50, pady = 5, command = app1, bg = "grey", fg = "black")
    app1Heap.grid(row = 1, column = 0, padx = 10, pady = 10, sticky = EW)

    app2Heap = Button(frame2, text = " APP 2 ", padx = 50, pady = 5, command = app2, bg = "grey", fg = "black")
    app2Heap.grid(row = 2, column = 0, padx = 10, pady = 10, sticky = EW)

#function for writing the output on the screen
def writeToTextView(outputText):
    st.configure(state="normal")
    st.delete("1.0","end")
    st.configure(state="normal")
    st.insert("1.0",outputText)
    st.configure(state="disabled")

#function to clear the widgets in the second frame
def clear():
    for w in frame2.winfo_children():
        w.destroy()

if __name__ =="__main__":

    if (len(sys.argv) == 1):
        print('Usage error:')
        usage()
        sys.exit(1)

    port = sys.argv[1]
    os.system("sudo chmod 777 " + port)

    root = Tk()
    root.title('Debug Analyser UI')

    #first frame setup
    frame = LabelFrame(root, text = "User's Menu", padx = 20, pady = 20)
    frame.pack(fill = Y, side = tk.LEFT, padx = 10, pady = 10)
    frame.grid_anchor(CENTER)

    #setFrame1 view
    setupframe1()

    #second frame for sub-menus
    frame2= LabelFrame(root, text = "subs Menu", padx = 20, pady = 20)
    frame2.pack(fill = Y, side = tk.LEFT, padx = 50, pady = 50)
    frame2.grid_anchor(CENTER)

    #third frame for custom input
    frame3= LabelFrame(root, text = "Custom Input", padx = 20, pady = 20)
    frame3.pack(fill = X, padx = 10, pady = 10)

    #setupFrame3 View
    setupFrame3()

    #main screen size
    root.geometry("1000x2000")

    #for getting current directory path
    path=os.path.dirname(os.path.realpath(__file__))
    keyboard=Controller()

    #output screen setup
    st = ScrolledText(root, width = 50, height = 10, padx = 20, pady = 20)
    st.pack(fill = tk.BOTH, expand = True, padx = 10, pady = 10)
    st.configure(state = 'disabled')

    root.mainloop()
