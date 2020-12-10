#!/usr/bin/env python
###########################################################################
#
# Copyright 2017 Samsung Electronics All Rights Reserved.
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
# File : gui_ramdumpParser.py
# Description:
# graphical user interface of ramdumpParser.
# Real parsing is operated at ramdumpParser.py

from __future__ import print_function
from tkinter import *
import tkinter.ttk
import tkinter.filedialog
import os
import tempfile
import subprocess

modes = (
    ("AssertLog",1),
    ("AssertLogFile",2),
    ("Ramdump",3),
	("CallStackLog",4),
)

g_elfpath = "../../build/output/bin/tinyara"
file_data = 'HeapInfo'

class PathFrame(Frame):
	def __init__(self, parent, labelname="path", path=None):
		Frame.__init__(self, parent)
		self.path = StringVar()
		self.path.set(path)
		self.labelname = labelname
		self.initialize()

	def initialize(self):
		self.label = Label(self, text=self.labelname)
		self.label.grid(column=0, row=0, sticky="EW")
		self.entry = Entry(self, textvariable=self.path)
		self.entry.grid(column=1, row=0, sticky="EW")
		self.entry.bind("<Return>", self.OnPressEnter)
		btn = Button(self, text="Browse", command=self.OnButtonClick)
		btn.grid(column=2, row=0)

	def OnButtonClick(self):
		temp = tkinter.filedialog.askopenfilename(parent=self)
		if len(temp) > 0:
			self.path.set(temp)

	def OnPressEnter(self,event):
		self.path.set(self.entry.get())

class curry:
	def __init__(self, func, *args, **kwargs):
		self.func = func
		self.pending = args[:]
		self.kwargs=kwargs.copy()

	def __call__(self, *args, **kwargs):
		if kwargs and self.kwargs:
			kw=self.kwargs.copy()
			kw.update(kwargs)
		else:
			kw=kwargs or self.kwargs
		return self.func(*(self.pending+args), **kw)

class HeapInfo(Tk):

	def __init__(self):
		Tk.__init__(self)
		self.wm_title("Heap Information")

		WIDTH_SIZE = 180
		MIN_SIZE = 100

		frame = Frame(self, relief="solid", bd = 0, width = WIDTH_SIZE)
		frame.pack(fill = "both")
		self.heap_label = Label(frame, justify = 'left',anchor = 'nw', bg = 'white', padx = 1, bd = 1  , width = 50 , height = 4)
		self.heap_label.pack(side = 'left')

		frame = Frame(self, relief="solid", bd = 0, width = WIDTH_SIZE)
		frame.pack(fill = "both")
		label = Label(frame, text ='FREE', bg = "white", padx = 1, bd = 1  , width = 10)
		label.pack(side = 'right')
		label = Label(frame, text ='STACK', bg = "red", padx = 1, bd = 1  , width = 10)
		label.pack(side = 'right')
		label = Label(frame, text ='ALLOC', bg = "blue", padx = 1, bd = 1  , width = 10)
		label.pack(side = 'right')

		len = 0
		remain = 0
		frame = Frame(self, relief="solid", bd = 1, width = WIDTH_SIZE)
		frame.pack(fill = "both")

		with open(file_data) as f:
			for line in f:
				lines = line.split()
				size = (int(lines[0]) + MIN_SIZE - 1) / MIN_SIZE
				len += size + 1
				if len > WIDTH_SIZE :
					remain = len - WIDTH_SIZE
					size -= remain
					len = 0

				separator = tkinter.ttk.Separator(frame, orient="vertical")
				separator.pack(side = 'left')

				# linens[0] = heap size, linens[1] = heap status, linens[2] = Mem address, linens[3] = pid, linens[4] = Owner
				if lines[1] == '0': # alloc
					alloc_button = Button(frame, bg = "blue", padx = 1, bd = 0, width = size, command = curry(self.alloc_event, lines[0], lines[2], lines[3], lines[4]))
					alloc_button.pack(side = 'left')
				elif lines[1] == '1': # stack
					stack_button = Button(frame, bg = "red", padx = 1, bd = 0, width = size, command = curry(self.alloc_event, lines[0], lines[2], lines[3], lines[4]))
					stack_button.pack(side = 'left')
				else : # free
					free_button = Button(frame, bg = "white", padx = 1, bd = 0, width = size, command = curry(self.free_event, lines[0], lines[2]))
					free_button.pack(side = 'left')

				while remain != 0 :
					# Add new line
					frame = Frame(self, relief = "solid", bd=1)
					frame.pack(fill = "both")
					if remain + 1 > WIDTH_SIZE :
						size = WIDTH_SIZE - 1
					else :
						size = remain
						len = remain + 1

					if lines[1] == '0': # alloc
						alloc_button1 = Button(frame, bg = "blue", padx = 1, bd = 0, width = size, command = curry(self.alloc_event, lines[0], lines[2], lines[3], lines[4]))
						alloc_button1.pack(side = 'left')
					elif lines[1] == '1': # stack
						stack_button1 = Button(frame, bg = "red", padx = 1, bd = 0, width = size, command = curry(self.alloc_event, lines[0], lines[2], lines[3], lines[4]))
						stack_button1.pack(side = 'left')
					else : # free
						free_button1 = Button(frame, bg = "white" , padx = 1, bd = 0, width = size, command = curry(self.free_event, lines[0], lines[2]))
						free_button1.pack(side = 'left')
					remain -= size

				if len == WIDTH_SIZE :
					# Add new line
					frame = Frame(self, relief = "solid", bd = 1)
					frame.pack(fill = "both")
					len = 0

	def alloc_event(self, Size, MemAddr, Pid, Owner):
		self.heap_label['text'] = 'MemAddr : ' + MemAddr + '\n' + 'Size : ' +  Size + '\n' + 'Pid : ' + Pid + '\n' + 'Owner : ' + Owner

	def free_event(self, Size, MemAddr):
		self.heap_label['text'] = 'MemAddr : ' + MemAddr + '\n' + 'Size : ' +  Size


class DumpParser(Tk):
	def __init__(self):
		Tk.__init__(self)
		self.modevar = IntVar()
		self.modevar.set(1)
		self.log = StringVar()
		self.initialize()

	def initialize(self):
		self.elfpath = PathFrame(self, "ELF path              ", g_elfpath)
		self.elfpath.pack(anchor=W)

		self.modeframe = Frame(self)
		self.modeframe.pack(anchor=W)
		for mode, val in modes:
			Radiobutton(self.modeframe, text=mode, variable=self.modevar, value=val, command=self.OnRadioClick).pack(anchor=W)

		self.dataframe = Frame(self)
		self.dataframe.pack(anchor=W)
		self.logtext = Text(self.dataframe)
		self.logtext.pack(anchor=W)
		self.logpath = PathFrame(self.dataframe, "AssertLogFile path")
		self.ramdumppath = PathFrame(self.dataframe, "Ramdump path    ")

		btn = Button(self, text="Run DumpParser", command=self.RunDumpParser)
		btn.pack(anchor=W)

	def OnRadioClick(self):
		self.logtext.pack_forget()
		self.logpath.pack_forget()
		self.ramdumppath.pack_forget()
		if self.modevar.get() == 1:
			self.logtext.pack(anchor=W)
		elif self.modevar.get() == 2:
			self.logpath.pack(anchor=W)
		elif self.modevar.get() == 3:
			self.ramdumppath.pack(anchor=W)
		elif self.modevar.get() == 4:
			self.logtext.pack(anchor=W)

	def RunDumpParser(self):
		resWin = Toplevel(self)
		resWin.wm_title("Dump Information")
		resText = Text(resWin)
		resText.pack(fill = "both")

		if self.modevar.get() == 1:
			fd, path = tempfile.mkstemp()
			try:
				with os.fdopen(fd, 'w') as tmp:
					tmp.write(self.logtext.get("1.0",END))
				with os.popen("python ramdumpParser.py" + 
							  " -e " + self.elfpath.path.get() +
							  " -t " + path) as fd:
					output = fd.read()
					resText.insert(INSERT, output)
			finally:
				os.remove(path)
		elif self.modevar.get() == 2:
			with os.popen("python ramdumpParser.py"
						  " -e " + self.elfpath.path.get()+
						  " -t " + self.logpath.path.get()) as fd:
				output = fd.read()
				resText.insert(INSERT, output)
		elif self.modevar.get() == 3:
			with os.popen("python ramdumpParser.py"
						  " -e " + self.elfpath.path.get()+
						  " -r " + self.ramdumppath.path.get()) as fd:
				output = fd.read()
				resText.insert(INSERT, output)

			if os.path.isfile(file_data):
				HeapInfo()
				os.remove(file_data)

		elif self.modevar.get() == 4:
			text = self.logtext.get("1.0",END)
			lines = [_f for _f in text.split("\n") if _f]
			for line in lines:
				addr_start = line.find("[<")
				addr_end = line.find(">]")
				addr = line[addr_start+2:addr_end]
				cmd = ['addr2line', '-e', self.elfpath.path.get(), addr]
				fd_popen = subprocess.Popen(cmd, stdout=subprocess.PIPE).stdout
				data = fd_popen.read()
				resText.insert(INSERT, data)

if __name__ == "__main__":
	os.chdir(os.path.dirname(os.path.realpath(__file__)))
	app = DumpParser()
	app.title("Ramdump Parser")
	app.mainloop()
