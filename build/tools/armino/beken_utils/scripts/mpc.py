#!/usr/bin/env python3
import os
import logging
import csv

from .parse_csv import *

mpc_keys = [
    'Device',
    'Offset',
    'Size',
    'Secure'
]

class MPC(list):

    def __getitem__(self, key):
        return self.csv.dic[key];

    def __init__(self, csv_file):
        self.csv = Csv(csv_file, True, mpc_keys)
