# RGB565 Image Converter and Rotator

A Python tool for converting JPG and BMP images to RGB565 format with optional rotation capabilities.

## Overview

This tool converts JPG and BMP images to RGB565 format, which is commonly used in embedded systems and display applications. It also provides optional image rotation by 90, 180, or 270 degrees before conversion.

## Features

- Convert JPG and BMP images to RGB565 format
- Optional image clockwise rotation (90°, 180°, or 270°)
- Automatic file naming or custom output naming
- Generates both rotated images and RGB565 data files
- Command-line interface for easy integration

## Requirements

- Python 3.10.12
- Pillow (PIL) library

Install the required dependencies:
```bash
pip install Pillow
```

## Usage

### Basic Syntax

```bash
python convert_rgb565_rotate_image.py -filename INPUT_FILE [-rotatedegree {90,180,270}] [-output OUTPUT_FILE]
```

### Parameters

| Parameter | Description | Required |
|-----------|-------------|----------|
| `-filename` | Input JPG or BMP file to process | Yes |
| `-rotatedegree` | Rotation degree clockwise (90, 180, or 270) - rotation is skipped if not provided | No |
| `-output` | Output image file name (optional) | No |
| `-h, --help` | Show help message and exit | No |

### Examples

1. **Convert image to RGB565 without rotation:**
   ```bash
   python convert_rgb565_rotate_image.py -filename image.jpg
   ```

2. **Convert and rotate image by 90 degrees:**
   ```bash
   python convert_rgb565_rotate_image.py -filename image.jpg -rotatedegree 90
   ```

3. **Convert with custom output file name:**
   ```bash
   python convert_rgb565_rotate_image.py -filename image.jpg -rotatedegree 180 -output rotated_image.jpg
   ```

## Output Files

The tool generates two types of output files:

1. **Rotated Image File** (when rotation is applied):
   - Format: JPG or BMP (same as input)
   - Naming: `original_filename_rotated_DEGREE.extension` (automatic) or custom name

2. **RGB565 Data File**:
   - Format: Text file containing raw RGB565 data as comma-separated byte values
   - Naming: `original_filename_rotated_DEGREE.txt` (with rotation) or `original_filename_original.txt` (without rotation)

## How It Works

1. **Image Processing**:
   - Loads the input image (JPG/BMP)
   - Rotates the image if specified (90°, 180°, or 270°) in clockwise direction
   - Saves the rotated image as a new file

2. **RGB565 Conversion**:
   - Converts each pixel from RGB888 to RGB565 format
   - RGB565 format uses:
     - 5 bits for Red
     - 6 bits for Green
     - 5 bits for Blue
   - Outputs raw RGB565 data as comma-separated byte values

3. **File Generation**:
   - Creates a text file with raw RGB565 data
   - Includes metadata (dimensions, data points count)
   - Saves file in the same directory as the script

## License

Copyright 2025 Samsung Electronics All Rights Reserved.
#
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
either express or implied. See the License for the specific
language governing permissions and limitations under the License.
