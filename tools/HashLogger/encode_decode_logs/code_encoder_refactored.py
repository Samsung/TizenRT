"""
Code Encoder - Refactored
==========================

Encodes C printf and dbg statements by replacing string literals with
hexadecimal references and generating a mapping file.

Features:
- Class-based encapsulation
- Proper error handling
- Type hints
- Comprehensive docstrings
- Configurable encoding options

Compatible with Python 3.5.2+
"""

import re
import os
import sys
from typing import Dict, List, Optional, Tuple


class EncodingStats:
    """Statistics for encoding operations."""
    
    def __init__(self):
        self.files_processed = 0
        self.strings_encoded = 0
        self.formatted_strings = 0
        self.plain_strings = 0


class CodeEncoder:
    """
    Encodes C printf and dbg statements in source files.
    
    Replaces string literals with hexadecimal references and generates
    a mapping file for decoding.
    """
    
    # Regular expressions
    STRING_LITERAL = r'"(?:\\.|[^"\\])*"'
    PRINTF_REGEX = re.compile(
        r'.*?(printf|dbg)\s*\(\s*(' + STRING_LITERAL + r')\s*(,..*)?\);'
    )
    FORMAT_SPECIFIER = re.compile(
        r'%(?:\d+\$)?[-#0 +]*\d*(?:\.\d+)?(?:h|hh|l|ll|j|z|t|L)?[diouxXeEfgGaAcspn]'
    )
    
    def __init__(self, output_mapping_file):
        """
        Initialize the CodeEncoder.
        
        Args:
            output_mapping_file: Path where mapping.txt will be saved
        """
        self.output_mapping_file = output_mapping_file
        self.string_map = {}
        self.counter = 0
        self.stats = EncodingStats()
    
    def _to_hexadecimal(self, number):
        """
        Convert integer to uppercase hexadecimal string without '0x' prefix.
        
        Args:
            number: Integer to convert
            
        Returns:
            Uppercase hexadecimal string
            
        Raises:
            ValueError: If input is not an integer
        """
        if not isinstance(number, int):
            raise ValueError("Input must be an integer, got {}".format(type(number)))
        
        return hex(number)[2:].upper()
    
    def _parse_string_literal(self, line, match):
        """
        Parse the string literal from a matched line.
        
        Args:
            line: The full line containing the string literal
            match: Regex match object
            
        Returns:
            Tuple of (full_string_with_quotes, content_without_quotes, suffix)
        """
        full_string = match.group(2)  # The quoted string
        content = full_string[1:-1]  # Remove quotes
        
        # Extract suffix (like \\n)
        suffix = ""
        base = content
        
        if content.endswith("\\n"):
            base = content[:-2]
            suffix = "\\n"
        
        return full_string, base, suffix
    
    def _encode_formatted_string(self, base, suffix):
        """
        Encode a formatted printf string (with format specifiers).
        
        Args:
            base: String content without suffix
            suffix: String suffix (e.g., "\\n")
            
        Returns:
            Tuple of (hex_key, encoded_string)
        """
        counter_hash = self._to_hexadecimal(self.counter)
        self.string_map[counter_hash] = base
        
        # Find all format specifiers
        specs = self.FORMAT_SPECIFIER.findall(base)
        spec_part = " " + "|".join(specs) if specs else ""
        
        encoded_string = '"${}{}!{}"'.format(counter_hash, spec_part, suffix)
        
        return counter_hash, encoded_string
    
    def _encode_plain_string(self, base, suffix):
        """
        Encode a plain string (no format specifiers).
        
        Args:
            base: String content without suffix
            suffix: String suffix (e.g., "\\n")
            
        Returns:
            Tuple of (hex_key, encoded_string)
        """
        counter_hash = self._to_hexadecimal(self.counter)
        self.string_map[counter_hash] = base
        
        encoded_string = '"${}#{}"'.format(counter_hash, suffix)
        
        return counter_hash, encoded_string
    
    def encode_file(self, input_file):
        """
        Encode a single C source file.
        
        Args:
            input_file: Path to the C source file
            
        Raises:
            IOError: If file operations fail
            ValueError: If file path is invalid
        """
        if not os.path.exists(input_file):
            raise FileNotFoundError("File not found: {}".format(input_file))
        
        if not input_file.endswith('.c'):
            print("Warning: {} is not a .c file, skipping...".format(input_file))
            return
        
        print("Processing: {}".format(os.path.basename(input_file)))
        
        try:
            # Read file as bytes to preserve exact line endings
            with open(input_file, "rb") as f:
                data = f.read()
            
            # Decode to text for processing
            lines = data.decode('latin-1').splitlines(True)  # keepends=True
            
            # Check if file contains any printf or dbg statements before processing
            content = ''.join(lines)
            if 'printf' not in content and 'dbg' not in content:
                print("  No printf or dbg statements found")
                return
            
            # Process lines
            file_encoded = False
            output_lines = []
            
            for line in lines:
                # Quick check: only process lines that might need encoding
                if 'printf' not in line and 'dbg' not in line:
                    output_lines.append(line)
                    continue
                
                match = self.PRINTF_REGEX.search(line)
                
                if not match:
                    output_lines.append(line)
                    continue
                
                # Parse the string literal
                full_string, base, suffix = self._parse_string_literal(line, match)
                
                # Skip empty strings with just newline
                if base == "":
                    output_lines.append(line)
                    continue
                
                # Encode the string
                try:
                    if "%" in base:
                        # Formatted string
                        hex_key, encoded_string = self._encode_formatted_string(base, suffix)
                        self.stats.formatted_strings += 1
                    else:
                        # Plain string
                        hex_key, encoded_string = self._encode_plain_string(base, suffix)
                        self.stats.plain_strings += 1
                    
                    # Only replace if encoded string is shorter than original
                    # This optimization ensures binary size reduction
                    if len(encoded_string) < len(full_string):
                        # Replace in line, preserve exact line ending
                        line_ending = line[len(line.rstrip('\r\n')):] if line.rstrip('\r\n') != line else ''
                        encoded_line = line.replace(full_string, encoded_string, 1)
                        # Strip ending and add back the original ending
                        encoded_line = encoded_line.rstrip('\r\n') + line_ending
                        output_lines.append(encoded_line)
                        
                        self.counter += 1
                        self.stats.strings_encoded += 1
                        file_encoded = True
                    else:
                        # Skip encoding - original string is shorter or same length
                        # Keep the original line to avoid increasing binary size
                        output_lines.append(line)
                        # Decrement stats since we're not using this encoding
                        if "%" in base:
                            self.stats.formatted_strings -= 1
                        else:
                            self.stats.plain_strings -= 1
                        print("  Skipped encoding (encoded longer than original): '{}' (original: {} chars, encoded: {} chars)".format(base, len(full_string), len(encoded_string)))
                    
                except Exception as e:
                    print("Error encoding line: {}".format(e))
                    output_lines.append(line)
            
            # Write output as bytes to preserve exact line endings
            with open(input_file, "wb") as f:
                f.write(''.join(output_lines).encode('latin-1'))
            
            if file_encoded:
                self.stats.files_processed += 1
            
        except IOError as e:
            raise IOError("Failed to process file {}: {}".format(input_file, e))
    
    def encode_directory(self, directory, recursive=True):
        """
        Encode all .c files in a directory.
        
        Args:
            directory: Path to directory containing C source files
            recursive: If True, process subdirectories recursively
        """
        if not os.path.exists(directory):
            raise FileNotFoundError("Directory not found: {}".format(directory))
        
        if not os.path.isdir(directory):
            raise NotADirectoryError("Not a directory: {}".format(directory))
        
        # Find all .c files
        c_files = []
        if recursive:
            for root, dirs, files in os.walk(directory):
                for file in files:
                    if file.endswith('.c'):
                        c_files.append(os.path.join(root, file))
        else:
            for file in os.listdir(directory):
                if file.endswith('.c'):
                    c_files.append(os.path.join(directory, file))
        
        if not c_files:
            print("No .c files found in {}".format(directory))
            return
        
        print("Found {} .c file(s) to process".format(len(c_files)))
        print("-" * 60)
        
        # Process each file
        for c_file in c_files:
            try:
                self.encode_file(c_file)
            except Exception as e:
                print("Error processing {}: {}".format(c_file, e))
        
        print("-" * 60)
        print("Encoding complete!")
        self._print_stats()
    
    def save_mapping(self):
        """
        Save the string mapping to the output file.
        
        Raises:
            IOError: If file operations fail
        """
        # Create output directory if it doesn't exist
        output_path = self.output_mapping_file
        output_dir = os.path.dirname(output_path)
        if output_dir:
            os.makedirs(output_dir, exist_ok=True)
        
        try:
            with open(output_path, "w", encoding="latin-1") as f:
                # Write mappings in hexadecimal order
                for key in sorted(self.string_map.keys(), key=lambda x: int(x, 16) if x.isdigit() else 0):
                    value = self.string_map[key]
                    f.write("{}->{}\n".format(key, value))
            
            print("\nMapping saved to: {}".format(output_path))
            
        except IOError as e:
            raise IOError("Failed to save mapping file: {}".format(e))
    
    def _print_stats(self):
        """Print encoding statistics."""
        print("\nEncoding Statistics:")
        print("  Files processed: {}".format(self.stats.files_processed))
        print("  Strings encoded: {}".format(self.stats.strings_encoded))
        print("  - Formatted (%%): {}".format(self.stats.formatted_strings))
        print("  - Plain strings: {}".format(self.stats.plain_strings))
        print("  Total mappings: {}".format(len(self.string_map)))
    
    @staticmethod
    def _get_timestamp():
        """Get current timestamp as string."""
        from datetime import datetime
        return datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
    def get_mapping(self):
        """
        Get the current string mapping.
        
        Returns:
            Dictionary mapping hexadecimal keys to original strings
        """
        return self.string_map.copy()
    
    def parse_config(self, config_path):
        """
        Parse the .config file to extract CONFIG_ARCH_BOARD and CONFIG_ARCH_FAMILY.
        
        Args:
            config_path: Path to the .config file
            
        Returns:
            Tuple of (arch_board, arch_family) values
        """
        if not os.path.exists(config_path):
            raise FileNotFoundError("Config file not found: {}".format(config_path))
        
        arch_board = None
        arch_family = None
        
        with open(config_path, 'r') as f:
            for line in f:
                line = line.strip()
                if line.startswith('CONFIG_ARCH_BOARD='):
                    arch_board = line.split('=')[1].strip('"').strip()
                elif line.startswith('CONFIG_ARCH_FAMILY='):
                    arch_family = line.split('=')[1].strip('"').strip()
        
        if not arch_board or not arch_family:
            raise ValueError("Could not find CONFIG_ARCH_BOARD or CONFIG_ARCH_FAMILY in config file")
        
        print("Detected configuration:")
        print("  CONFIG_ARCH_BOARD: {}".format(arch_board))
        print("  CONFIG_ARCH_FAMILY: {}".format(arch_family))
        
        return arch_board, arch_family
    
    def encode_with_config_filter(self, tizenrt_root):
        """
        Encode all .c files in the codebase, but only encode selected board and architecture.
        
        For os/board/: only encode the selected board (skip other boards)
        For os/arch/arm/src/: only encode the selected architecture (skip other architectures)
        All other directories are encoded normally.
        
        Args:
            tizenrt_root: Path to the tizenrt root directory
        """
        if not os.path.exists(tizenrt_root):
            raise FileNotFoundError("TizenRT root directory not found: {}".format(tizenrt_root))
        
        # Parse the config file
        config_path = os.path.join(tizenrt_root, "os", ".config")
        arch_board, arch_family = self.parse_config(config_path)
        
        # Define paths to skip/allow
        board_base_dir = os.path.join(tizenrt_root, "os", "board")
        arch_base_dir = os.path.join(tizenrt_root, "os", "arch", "arm", "src")
        tools_dir = os.path.join(tizenrt_root, "os", "tools")
        
        print("\nEncoding whole codebase with filters:")
        print("  Board filter: only encoding '{}'".format(arch_board))
        print("  Architecture filter: only encoding '{}'".format(arch_family))
        print("  Skipping: os/tools/ (build tools)")
        print("-" * 60)
        
        # Find all .c files in the entire codebase
        c_files = []
        for root, dirs, files in os.walk(tizenrt_root):
            for file in files:
                if file.endswith('.c'):
                    c_files.append(os.path.join(root, file))
        
        print("Found {} .c file(s) in total".format(len(c_files)))
        
        files_to_encode = []
        files_skipped = 0
        
        for c_file in c_files:
            # Check if file is in os/tools/ directory (build tools - skip these)
            if os.path.exists(tools_dir) and c_file.startswith(tools_dir):
                files_skipped += 1
                continue
            
            # Check if file is in os/board/ directory
            if os.path.exists(board_base_dir) and c_file.startswith(board_base_dir):
                # File is in board directory, check if it's the selected board
                rel_path = os.path.relpath(c_file, board_base_dir)
                board_name = rel_path.split(os.sep)[0] if rel_path else None
                if board_name and board_name != arch_board:
                    # Skip this file - it's a different board
                    files_skipped += 1
                    continue
            
            # Check if file is in os/arch/arm/src/ directory
            if os.path.exists(arch_base_dir) and c_file.startswith(arch_base_dir):
                # File is in arch directory, check if it's the selected architecture
                rel_path = os.path.relpath(c_file, arch_base_dir)
                arch_name = rel_path.split(os.sep)[0] if rel_path else None
                if arch_name and arch_name != arch_family:
                    # Skip this file - it's a different architecture
                    files_skipped += 1
                    continue
            
            files_to_encode.append(c_file)
        
        print("Files to encode: {} (skipped: {})".format(len(files_to_encode), files_skipped))
        print("-" * 60)
        
        # Process each file
        for c_file in files_to_encode:
            try:
                self.encode_file(c_file)
            except Exception as e:
                print("Error processing {}: {}".format(c_file, e))
        
        print("-" * 60)
        print("Encoding complete!")
        self._print_stats()


def main():
    """Main entry point for the code encoder."""
    import argparse
    
    parser = argparse.ArgumentParser(
        description="Encode C printf and dbg statements with hexadecimal references",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Encode all .c files in a directory:
  %(prog)s /path/to/source /output/path
  
  # Encode only top-level directory (non-recursive):
  %(prog)s /path/to/source /output/path --no-recursive
  
  # Encode whole codebase with board/architecture filter from .config:
  # (encodes all files except other boards in os/board/ and other architectures in os/arch/arm/src/)
  %(prog)s /path/to/tizenrt /output/path --use-config
        """
    )
    
    parser.add_argument(
        "source_path",
        help="Path to source directory or C file to encode"
    )
    
    parser.add_argument(
        "output_path",
        help="Path where mapping.txt will be saved"
    )
    
    parser.add_argument(
        "--no-recursive",
        action="store_true",
        help="Do not process subdirectories recursively"
    )
    
    parser.add_argument(
        "--use-config",
        action="store_true",
        help="Read .config file and encode only selected board/architecture directories"
    )
    
    args = parser.parse_args()
    
    # Validate paths
    if not os.path.exists(args.source_path):
        print("Error: Source path not found: {}".format(args.source_path))
        sys.exit(1)
    
    # Create encoder
    output_mapping = os.path.join(args.output_path, "mapping.txt")
    encoder = CodeEncoder(output_mapping)
    
    try:
        # Process source
        if args.use_config:
            # Encode whole codebase with board/architecture filter from .config
            encoder.encode_with_config_filter(args.source_path)
        elif os.path.isfile(args.source_path):
            # Single file
            encoder.encode_file(args.source_path)
            encoder._print_stats()
        else:
            # Directory
            encoder.encode_directory(
                args.source_path,
                recursive=not args.no_recursive
            )
        
        # Save mapping
        encoder.save_mapping()
        
        print("\nEncoding completed successfully!")
        
    except Exception as e:
        print("\nError: {}".format(e))
        sys.exit(1)


if __name__ == "__main__":
    main()
