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
# File : gui_dumpParser.py
# Description:
# graphical user interface of dumpParser.
# Real parsing is operated at dumpParser.py
from Tkinter import *
import tkFileDialog
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
		temp = tkFileDialog.askopenfilename(parent=self)
		if len(temp) > 0:
			self.path.set(temp)

	def OnPressEnter(self,event):
		self.path.set(self.entry.get())

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
		resText.pack()
		if self.modevar.get() == 1:
			fd, path = tempfile.mkstemp()
			try:
				with os.fdopen(fd, 'w') as tmp:
					tmp.write(self.logtext.get("1.0",END))
				with os.popen("python dumpParser.py" + 
							  " -e " + self.elfpath.path.get() +
							  " -t " + path) as fd:
					output = fd.read()
					resText.insert(INSERT, output)
			finally:
				os.remove(path)
		elif self.modevar.get() == 2:
			with os.popen("python dumpParser.py"
						  " -e " + self.elfpath.path.get()+
						  " -t " + self.logpath.path.get()) as fd:
				output = fd.read()
				resText.insert(INSERT, output)
		elif self.modevar.get() == 3:
			with os.popen("python dumpParser.py"
						  " -e " + self.elfpath.path.get()+
						  " -r " + self.ramdumppath.path.get()) as fd:
				output = fd.read()
				resText.insert(INSERT, output)
		elif self.modevar.get() == 4:
			text = self.logtext.get("1.0",END)
			lines = filter(None, text.split("\n"))
			for line in lines:
				addr_start = line.find("[<")
				addr_end = line.find(">]")
				addr = line[addr_start+2:addr_end]
				cmd = ['addr2line', '-e', self.elfpath.path.get(), addr]
				fd_popen = subprocess.Popen(cmd, stdout=subprocess.PIPE).stdout
				data = fd_popen.read()
				resText.insert(INSERT, data)

if __name__ == "__main__":
	app = DumpParser()
	app.title("Dump Parser")
	app.mainloop()
