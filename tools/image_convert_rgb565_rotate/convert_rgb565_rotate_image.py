#!/usr/bin/env python
###########################################################################
#
# Copyright 2025 Samsung Electronics All Rights Reserved.
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
# File : convert_rgb565_rotate_image.py
# Description: Script to optionally rotate JPG and BMP images by 90, 180, or 270 degrees in clockwise direction
#
# usage: 
#    convert_rgb565_rotate_image.py [-h] -filename FILENAME [-rotatedegree {90,180,270}] [-output OUTPUT]

#    Optionally rotate JPG and BMP images by 90, 180, or 270 degrees in clockwise direction
#
#    options:
#    -h, --help                    show this help message and exit
#    -filename FILENAME            Input JPG or BMP file to process
#    -rotatedegree {90,180,270}    Rotation degree (90, 180, or 270) - rotation is skipped if not provided
#    -output OUTPUT                Output image file name (optional)

import argparse
from PIL import Image
import os

def rotate_image(input_file, degree, output_file=None):
    img = Image.open(input_file)
    rotated_img = img.rotate(-degree, expand=True)

    if output_file is None:
        name, ext = os.path.splitext(input_file)
        output_file = f"{name}_rotated_{degree}{ext}"

    rotated_img.save(output_file)
    print(f"Rotated image by {degree} degrees and saved as: {output_file}")
    return output_file

def convert_to_rgb565_array(input_image, degree=None):
    img = Image.open(input_image).convert("RGB")
    # Only rotate if degree is specified
    if degree is not None:
        img = img.rotate(-degree, expand=True)
    width, height = img.size
    rgb565_array = []

    for y in range(height):
        for x in range(width):
            r, g, b = img.getpixel((x,y))
            r5 = (r >> 3) & 0x1F
            g6 = (g >> 2) & 0x3F
            b5 = (b >> 3) & 0x1F
            rgb565 = (r5 << 11) | (g6 << 5) | b5
            rgb565_array.append((rgb565 >> 8) & 0xFF)
            rgb565_array.append(rgb565 & 0xFF)
    return rgb565_array, width, height

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Optionally rotate JPG and BMP images by 90, 180, or 270 degrees")
    parser.add_argument("-filename", required=True, help="Input JPG or BMP file to process")
    parser.add_argument("-rotatedegree", type=int, choices=[90, 180, 270], required=False, help="Rotation degree (90, 180, or 270) - rotation is skipped if not provided")
    parser.add_argument("-output", help="Output image file name (optional)")

    args = parser.parse_args()

    if not os.path.exists(args.filename):
        print(f"Error: File '{args.filename}' not found.")
        exit(1)

    name, ext = os.path.splitext(args.filename)
    if ext.lower() not in ['.jpg', '.jpeg', '.bmp']:
        print(f"Warning: File '{args.filename}' is not a JPG or BMP file. Proceeding anyway...")

    if args.rotatedegree is not None:
        output_image = rotate_image(args.filename, args.rotatedegree, args.output)
    else:
        output_image = args.filename
        if args.output is not None:
            from PIL import Image
            img = Image.open(args.filename)
            img.save(args.output)
            output_image = args.output
            print(f"Saved copy of original image as: {output_image}")

    if args.rotatedegree is not None:
        if args.output is None:
            name, ext = os.path.splitext(args.filename)
            rgb565_file = f"{name}_rotated_{args.rotatedegree}.txt"
        else:
            name, ext = os.path.splitext(args.output)
            rgb565_file = f"{name}.txt"
        rgb565_data, width, height = convert_to_rgb565_array(args.filename, args.rotatedegree)
        rotation_info = f" (rotated {args.rotatedegree} degrees)"
    else:
        if args.output is None:
            name, ext = os.path.splitext(args.filename)
            rgb565_file = f"{name}_original.txt"
        else:
            name, ext = os.path.splitext(args.output)
            rgb565_file = f"{name}.txt"
        rgb565_data, width, height = convert_to_rgb565_array(args.filename, None)
        rotation_info = " (no rotation)"

    with open(rgb565_file, 'w') as f:
        f.write(f"# RGB565 Data for {args.filename}{rotation_info}\n")
        f.write(f"# Image dimensions: {width}x{height}\n")
        f.write(f"# Total data points: {len(rgb565_data)}\n")

        rgb565_str = ", ".join(str(val) for val in rgb565_data)
        f.write(rgb565_str)

    print(f"RGB565 data saved to: {rgb565_file}")
