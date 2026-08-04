#!/usr/bin/env python3
"""
Code Decoder
============

Decodes C printf and dbg statements that were encoded by code_encoder_refactored.py.
Replaces hexadecimal references with original string literals using the mapping file.
"""

import re
import os
import sys


class DecodingStats:
    """Statistics for decoding operations."""
    def __init__(self):
        self.files_processed = 0
        self.strings_decoded = 0
        self.formatted_strings = 0
        self.plain_strings = 0
        self.errors = 0


class CodeDecoder:
    """
    Decodes C printf and dbg statements that were encoded with hexadecimal references.
    """
    
    # Regex patterns (same as encoder)
    STRING_LITERAL = r'"(?:\\.|[^"\\])*"'
    PRINTF_REGEX = re.compile(
        r'.*?(printf|dbg)\s*\(\s*(' + STRING_LITERAL + r')\s*(,.*)?\);'
    )
    
    def __init__(self, mapping_file, skip_dirs=None, verbose=False):
        """
        Initialize the CodeDecoder.
        
        Args:
            mapping_file: Path to the mapping.txt file
            skip_dirs: List of directory paths to skip during decoding
            verbose: Enable verbose output
        """
        self.mapping_file = mapping_file
        self.skip_dirs = skip_dirs or []
        self.verbose = verbose
        self.string_map = {}
        self.stats = DecodingStats()
    
    def load_mapping(self):
        """
        Load the string mapping from the mapping file.
        
        Raises:
            FileNotFoundError: If mapping file doesn't exist
            IOError: If file cannot be read
        """
        if not os.path.exists(self.mapping_file):
            raise FileNotFoundError("Mapping file not found: " + self.mapping_file)
        
        print("Loading mapping from: " + self.mapping_file)
        
        try:
            with open(self.mapping_file, "r", encoding="latin-1") as f:
                lines = f.readlines()
            
            for line_num, line in enumerate(lines, 1):
                # Remove only trailing newline, preserve all other whitespace
                line = line.rstrip('\n')
                
                # Skip comments and empty lines
                if not line or line.startswith("#") or line.startswith("="):
                    continue
                
                # Parse mapping: key->value
                if "->" in line:
                    parts = line.split("->", 1)
                    key = parts[0].strip()
                    value = parts[1]  # Keep all spaces including trailing ones
                    
                    # Validate hex key
                    try:
                        int(key, 16)  # Verify it's a valid hex string
                        self.string_map[key] = value
                    except ValueError:
                        print("Warning: Invalid hex key '" + key + "' on line " + str(line_num) + ", skipping...")
                else:
                    print("Warning: Invalid mapping format on line " + str(line_num) + ": " + line)
            
            print("Loaded " + str(len(self.string_map)) + " mappings")
            
        except IOError as e:
            raise IOError("Failed to read mapping file: " + str(e))
    
    def _decode_formatted_string(self, content):
        """
        Decode a formatted string (with format specifiers).
        Format: "$hex_key specs!suffix"
        Example: "$BFD %s %d\n"
        Replacement: Replace "$hex_key specs!" with string from mapping
        
        Args:
            content: The encoded string content without quotes
            
        Returns:
            Tuple of (decoded_string, success)
        """
        # Find the ! delimiter
        exclamation_pos = content.find('!')
        if exclamation_pos == -1:
            print("Warning: No ! delimiter found in formatted string: " + content)
            return '"' + content + '"', False
        
        # Extract hex key (from $ until space)
        # Content is like "$BFD %s %d\n"
        before_exclamation = content[:exclamation_pos]  # "$BFD %s %d"
        suffix = content[exclamation_pos + 1:]  # "\n"
        
        # Extract hex key (everything after $ until space)
        hex_key_match = re.match(r'\$([0-9A-Fa-f]+)', before_exclamation)
        if not hex_key_match:
            print("Warning: Invalid hex key in formatted string: " + content)
            return '"' + content + '"', False
        
        hex_key = hex_key_match.group(1)
        
        # Look up in mapping
        if hex_key not in self.string_map:
            print("Warning: Hex key '" + hex_key + "' not found in mapping")
            return '"' + content + '"', False
        
        base = self.string_map[hex_key]
        decoded = '"' + base + suffix + '"'
        
        return decoded, True
    
    def _decode_unformatted_string(self, content):
        """
        Decode an unformatted string (no format specifiers).
        Format: "$hex_key#suffix"
        Example: "$BF5\n"
        Replacement: Replace "$hex_key#" with string from mapping
        
        Args:
            content: The encoded string content without quotes
            
        Returns:
            Tuple of (decoded_string, success)
        """
        # Extract hex key (from $ until #)
        hash_pos = content.find('#')
        if hash_pos == -1:
            print("Warning: No # delimiter found in unformatted string: " + content)
            return '"' + content + '"', False
        
        hex_key = content[1:hash_pos]  # Everything between $ and #
        suffix = content[hash_pos + 1:]  # Everything after #
        
        # Look up in mapping
        if hex_key not in self.string_map:
            print("Warning: Hex key '" + hex_key + "' not found in mapping")
            return '"' + content + '"', False
        
        base = self.string_map[hex_key]
        decoded = '"' + base + suffix + '"'
        
        return decoded, True
    
    def decode_line(self, line):
        """
        Decode encoded strings in a single line.
        
        Args:
            line: Line of C source code
            
        Returns:
            Decoded line
        """
        decoded = line
        
        # Use the same regex as encoder to find printf/dbg statements
        match = self.PRINTF_REGEX.search(line)
        
        if not match:
            return decoded
        
        # Get the string literal from the match
        full_string = match.group(2)  # The quoted string
        content = full_string[1:-1]  # Remove quotes
        
        # Check if this is an encoded string (starts with $)
        if not content.startswith('$'):
            return decoded
        
        # Determine if it's unformatted (contains #) or formatted
        if '#' in content and '!' not in content:
            # Unformatted string: "$BF5\n"
            self.stats.plain_strings += 1
            decoded_string, success = self._decode_unformatted_string(content)
        else:
            # Formatted string: "$BFD %s %d\n"
            self.stats.formatted_strings += 1
            decoded_string, success = self._decode_formatted_string(content)
        
        if success:
            decoded = line.replace(full_string, decoded_string, 1)
            self.stats.strings_decoded += 1
        else:
            self.stats.errors += 1
        
        return decoded
    
    def decode_file(self, input_file):
        """
        Decode a single C source file.
        
        Args:
            input_file: Path to the C source file
            
        Raises:
            IOError: If file operations fail
            FileNotFoundError: If file doesn't exist
        """
        if not os.path.exists(input_file):
            raise FileNotFoundError("File not found: " + input_file)
        
        if not input_file.endswith('.c'):
            print("Warning: " + input_file + " is not a .c file, skipping...")
            return
        
        # Check if file should be skipped based on skip_dirs
        for skip_dir in self.skip_dirs:
            if skip_dir in input_file:
                if self.verbose:
                    print("Skipping " + input_file + " (in skip directory " + skip_dir + ")")
                return
        
        if self.verbose:
            print("Processing: " + os.path.basename(input_file))
        
        try:
            # Read file as bytes to preserve exact line endings
            with open(input_file, "rb") as f:
                data = f.read()
            
            # Decode to text for processing
            lines = data.decode('latin-1').splitlines(True)  # keepends=True
            
            # Check if file contains any encoded strings before processing
            content = ''.join(lines)
            if '$' not in content:
                print("  No encoded strings found")
                return
            
            # Process lines
            file_decoded = False
            output_lines = []
            
            for line in lines:
                # Quick check: only process lines that might need decoding
                if ('printf' in line or 'dbg' in line) and '$' in line:
                    decoded_line = self.decode_line(line)
                    
                    # Preserve exact line ending
                    line_ending = line[len(line.rstrip('\r\n')):] if line.rstrip('\r\n') != line else ''
                    decoded_line = decoded_line.rstrip('\r\n') + line_ending
                    output_lines.append(decoded_line)
                    
                    if decoded_line != line:
                        file_decoded = True
                else:
                    # Line doesn't need processing, keep as-is
                    output_lines.append(line)
            
            # Write output as bytes to preserve exact line endings
            with open(input_file, "wb") as f:
                f.write(''.join(output_lines).encode('latin-1'))
            
            if file_decoded:
                self.stats.files_processed += 1
            
        except IOError as e:
            raise IOError("Failed to process file " + input_file + ": " + str(e))
    
    def decode_directory(self, directory, recursive=True):
        """
        Decode all .c files in a directory.
        
        Args:
            directory: Path to directory containing C source files
            recursive: If True, process subdirectories recursively
        """
        if not os.path.exists(directory):
            raise FileNotFoundError("Directory not found: " + directory)
        
        if not os.path.isdir(directory):
            raise NotADirectoryError("Not a directory: " + directory)
        
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
            print("No .c files found in " + directory)
            return
        
        print("Found " + str(len(c_files)) + " .c file(s) to process")
        print("-" * 60)
        
        # Process each file
        for c_file in c_files:
            try:
                self.decode_file(c_file)
            except Exception as e:
                print("Error processing " + c_file + ": " + str(e))
        
        print("-" * 60)
        print("Decoding complete!")
        self._print_stats()
    
    def _print_stats(self):
        """Print decoding statistics."""
        print("\nDecoding Statistics:")
        print("  Files processed: " + str(self.stats.files_processed))
        print("  Strings decoded: " + str(self.stats.strings_decoded))
        print("  - Formatted (%%): " + str(self.stats.formatted_strings))
        print("  - Plain strings: " + str(self.stats.plain_strings))
        print("  Errors: " + str(self.stats.errors))
    
    def parse_config(self, config_path):
        """
        Parse the .config file to extract CONFIG_ARCH_BOARD and CONFIG_ARCH_FAMILY.
        
        Args:
            config_path: Path to the .config file
            
        Returns:
            Tuple of (arch_board, arch_family) values
        """
        if not os.path.exists(config_path):
            raise FileNotFoundError("Config file not found: " + config_path)
        
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
        print("  CONFIG_ARCH_BOARD: " + arch_board)
        print("  CONFIG_ARCH_FAMILY: " + arch_family)
        
        return arch_board, arch_family
    
    def decode_with_config_filter(self, tizenrt_root):
        """
        Decode all .c files in the codebase, but only decode selected board and architecture.
        
        For os/board/: only decode the selected board (skip other boards)
        For os/arch/arm/src/: only decode the selected architecture (skip other architectures)
        Skips os/tools/ directory (build tools).
        
        Args:
            tizenrt_root: Path to the tizenrt root directory
        """
        if not os.path.exists(tizenrt_root):
            raise FileNotFoundError("TizenRT root directory not found: " + tizenrt_root)
        
        # Parse the config file
        config_path = os.path.join(tizenrt_root, "os", ".config")
        arch_board, arch_family = self.parse_config(config_path)
        
        # Define paths to skip/allow
        board_base_dir = os.path.join(tizenrt_root, "os", "board")
        arch_base_dir = os.path.join(tizenrt_root, "os", "arch", "arm", "src")
        tools_dir = os.path.join(tizenrt_root, "os", "tools")
        
        print("\nDecoding whole codebase with filters:")
        print("  Board filter: only decoding '" + arch_board + "'")
        print("  Architecture filter: only decoding '" + arch_family + "'")
        print("  Skipping: os/tools/ (build tools)")
        print("-" * 60)
        
        # Find all .c files in the entire codebase
        c_files = []
        for root, dirs, files in os.walk(tizenrt_root):
            for file in files:
                if file.endswith('.c'):
                    c_files.append(os.path.join(root, file))
        
        print("Found " + str(len(c_files)) + " .c file(s) in total")
        
        files_to_decode = []
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
            
            files_to_decode.append(c_file)
        
        print("Files to decode: " + str(len(files_to_decode)) + " (skipped: " + str(files_skipped) + ")")
        print("-" * 60)
        
        # Process each file
        for c_file in files_to_decode:
            try:
                self.decode_file(c_file)
            except Exception as e:
                print("Error processing " + c_file + ": " + str(e))
        
        print("-" * 60)
        print("Decoding complete!")
        self._print_stats()


def main():
    """Main entry point for the code decoder."""
    import argparse
    
    parser = argparse.ArgumentParser(
        description="Decode C printf and dbg statements encoded by code_encoder_refactored.py",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Decode all .c files in a directory:
  %(prog)s /path/to/source /path/to/mapping.txt
  
  # Decode only top-level directory (non-recursive):
  %(prog)s /path/to/source /path/to/mapping.txt --no-recursive
  
  # Decode with skip directories:
  %(prog)s /path/to/source /path/to/mapping.txt --skip-dirs ../apps
        """
    )
    
    parser.add_argument(
        "source_path",
        help="Path to source directory or C file to decode"
    )
    
    parser.add_argument(
        "mapping_file",
        help="Path to the mapping.txt file generated by encoder"
    )
    
    parser.add_argument(
        "--skip-dirs",
        action="append",
        default=[],
        help="Directories to skip during decoding (can be specified multiple times)"
    )
    
    parser.add_argument(
        "--no-recursive",
        action="store_true",
        help="Do not process subdirectories recursively"
    )
    
    parser.add_argument(
        "--verbose",
        action="store_true",
        help="Enable verbose output"
    )
    
    parser.add_argument(
        "--use-config",
        action="store_true",
        help="Read .config file and decode only selected board/architecture directories"
    )
    
    args = parser.parse_args()
    
    # Validate paths
    if not os.path.exists(args.source_path):
        print("Error: Source path not found: " + args.source_path)
        sys.exit(1)
    
    if not os.path.exists(args.mapping_file):
        print("Error: Mapping file not found: " + args.mapping_file)
        sys.exit(1)
    
    # Create decoder with all options
    decoder = CodeDecoder(
        args.mapping_file,
        skip_dirs=args.skip_dirs,
        verbose=args.verbose
    )
    
    try:
        # Load mapping
        decoder.load_mapping()
        
        # Process source
        if args.use_config:
            # Decode whole codebase with board/architecture filter from .config
            decoder.decode_with_config_filter(args.source_path)
        elif os.path.isfile(args.source_path):
            # Single file
            decoder.decode_file(args.source_path)
            decoder._print_stats()
        else:
            # Directory
            decoder.decode_directory(
                args.source_path,
                recursive=not args.no_recursive
            )
        
        print("\nDecoding completed successfully!")
        
    except Exception as e:
        print("\nError: " + str(e))
        sys.exit(1)


if __name__ == "__main__":
    main()