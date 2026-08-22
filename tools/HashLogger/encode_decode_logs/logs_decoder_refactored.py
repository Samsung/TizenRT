"""
Refactored Log Decoder

Decodes encoded log files using a mapping dictionary.
Uses a class-based structure for better encapsulation and maintainability.
"""

import re
import sys
from typing import Dict, Tuple, Optional


class LogDecoder:
    """Decode encoded log files using a mapping dictionary."""
    
    def __init__(self, mapping: Dict[str, str]):
        """
        Initialize the LogDecoder with a mapping dictionary.
        
        Args:
            mapping: Dictionary mapping hexadecimal IDs to format strings
        """
        self.mapping = mapping
        self.FORMAT_SPECIFIER = re.compile(
            r'%([-#0 +]*)(\d*)?(\.\d+)?([lLh])?([xXd diuofFeEgGaAcspn])'
        )
    
    @classmethod
    def from_file(cls, mapping_file: str) -> 'LogDecoder':
        """
        Create a LogDecoder instance by loading mapping from a file.
        
        Args:
            mapping_file: Path to the mapping file (format: ID->string)
        
        Returns:
            LogDecoder instance with loaded mapping
        
        Raises:
            IOError: If the file cannot be read
            ValueError: If the file format is invalid
        """
        mapping = {}
        try:
            with open(mapping_file, "r", encoding="latin-1") as f:
                for line_num, line in enumerate(f, 1):
                    try:
                        line = line.rstrip()
                        if not line or "->" not in line:
                            continue
                        key, value = line.split("->", 1)
                        mapping[key] = value
                    except ValueError:
                        raise ValueError(
                            f"Invalid format at line {line_num}: {line.strip()}"
                        )
        except IOError as e:
            raise IOError(f"Failed to load mapping file '{mapping_file}': {e}")
        
        return cls(mapping)
    
    def _parse_format_specifiers(self, format_string: str, args: list) -> Tuple:
        """
        Convert string arguments to appropriate types based on format specifiers.
        
        Args:
            format_string: The format string with specifiers
            args: List of string arguments to convert
        
        Returns:
            Tuple of converted arguments
        """
        matches = list(self.FORMAT_SPECIFIER.finditer(format_string))
        
        if not matches:
            return tuple(args)
        
        converted_args = []
        arg_index = 0
        
        for match in matches:
            specifier = match.group(5)  # Get specifier letter (x, d, s, etc.)
            
            if arg_index >= len(args):
                break
                
            arg = args[arg_index]
            
            # Convert based on specifier type
            if specifier in ['x', 'X', 'd', 'i', 'u', 'o']:
                converted_arg = self._convert_to_int(arg, specifier)
            elif specifier in ['f', 'F', 'e', 'E', 'g', 'G', 'a', 'A']:
                converted_arg = self._convert_to_float(arg)
            else:
                converted_arg = arg
            
            converted_args.append(converted_arg)
            arg_index += 1
        
        return tuple(converted_args)
    
    def _convert_to_int(self, arg: str, specifier: str) -> int:
        """Convert string argument to integer based on specifier."""
        if isinstance(arg, (int, float)):
            return int(arg)
        
        if not isinstance(arg, str):
            return int(arg)
        
        # Strip quote characters
        arg_clean = arg.strip('\'"')
        
        # Handle empty strings
        if not arg_clean:
            return 0
        
        # Hexadecimal specifier - use base 16
        if specifier in ['x', 'X']:
            return int(arg_clean, 16)
        # Octal specifier - use base 8
        elif specifier == 'o':
            return int(arg_clean, 8)
        # Decimal/integer specifiers - use base 10
        else:
            return int(arg_clean, 10)
    
    def _convert_to_float(self, arg: str) -> float:
        """Convert string argument to float."""
        if isinstance(arg, (int, float)):
            return float(arg)
        
        if not isinstance(arg, str):
            return float(arg)
        
        arg_clean = arg.strip('\'"')
        
        if not arg_clean:
            return 0.0
        
        return float(arg_clean)
    
    def _parse_log_part(self, log_part: str, separator: str = ' ') -> Tuple[str, str, str]:
        """
        Parse log part into prefix, key, and remaining content.
        
        Args:
            log_part: The encoded log part to parse
            separator: Character that separates key from content
        
        Returns:
            Tuple of (prefix, key, remaining_content)
        """
        dollar_index = log_part.find('$')
        if dollar_index == -1:
            return log_part, "", ""
        
        space_index = log_part.find(separator, dollar_index)
        if space_index == -1:
            return log_part[:dollar_index], log_part[dollar_index + 1:], ""
        
        return (
            log_part[:dollar_index],
            log_part[dollar_index + 1:space_index],
            log_part[space_index + 1:]
        )
    
    def _decode_formatted_log_part(self, log_part: str) -> str:
        """
        Decode a formatted log part (with arguments).
        
        Args:
            log_part: The encoded log part with format arguments
        
        Returns:
            Decoded log string
        """
        prefix, key, remaining = self._parse_log_part(log_part, ' ')
        
        if not key or key not in self.mapping:
            return log_part
        
        # Replace %p (pointer) with %x for Python compatibility
        original_format = self.mapping[key].replace('%p', '%x')
        
        # Parse and convert arguments
        args = self._parse_format_specifiers(original_format, remaining.split("|"))
        
        # Format the string
        try:
            formatted = original_format % args
        except (TypeError, ValueError) as e:
            # If formatting fails, return original with debug info
            formatted = f"{original_format} [FORMAT ERROR: {e}]"
        
        return prefix + formatted
    
    def _decode_unformatted_log_part(self, log_part: str) -> str:
        """
        Decode an unformatted log part (no arguments).
        
        Args:
            log_part: The encoded log part without arguments
        
        Returns:
            Decoded log string
        """
        prefix, key, _ = self._parse_log_part(log_part, '#')
        
        if not key or key not in self.mapping:
            return log_part
        
        return prefix + self.mapping[key]
    
    def _decode_line(self, line: str) -> str:
        """
        Decode a single log line.
        
        Args:
            line: The encoded log line
        
        Returns:
            Decoded log line
        """
        if '$' not in line:
            return line
        
        # Determine separator and decode accordingly
        separator = '#' if '#' in line else '!'
        log_parts = [p for p in line.split(separator) if p]
        
        if separator == '#':
            # Unformatted logs
            decoded_parts = [
                self._decode_unformatted_log_part(part)
                for part in log_parts
            ]
        else:
            # Formatted logs
            decoded_parts = [
                self._decode_formatted_log_part(part)
                for part in log_parts
            ]
        
        decoded_line = ' '.join(decoded_parts)
        
        # Handle escape sequences
        try:
            decoded_line = decoded_line.encode('latin-1').decode('unicode_escape').rstrip()
        except (UnicodeDecodeError, UnicodeEncodeError):
            # If escape decoding fails, use the line as-is
            decoded_line = decoded_line.rstrip()
        
        return decoded_line
    
    def decode(self, logs_file: str, output_file: str) -> None:
        """
        Decode logs from input file and write to output file.
        
        Args:
            logs_file: Path to input log file
            output_file: Path to output decoded log file
        
        Raises:
            IOError: If file operations fail
        """
        decoded_lines = []
        
        try:
            with open(logs_file, "r", encoding="latin-1") as f:
                for line_num, line in enumerate(f, 1):
                    try:
                        decoded_line = self._decode_line(line.rstrip())
                        decoded_lines.append(decoded_line + '\n')
                    except Exception as e:
                        # Print warning but continue processing
                        print(f"Warning: Failed to decode line {line_num}: {e}")
                        decoded_lines.append(line)  # Keep original on error
        except IOError as e:
            raise IOError(f"Failed to read logs file '{logs_file}': {e}")
        
        try:
            with open(output_file, "w", encoding="latin-1") as f:
                f.writelines(decoded_lines)
        except IOError as e:
            raise IOError(f"Failed to write decoded logs to '{output_file}': {e}")
    
    def decode_line(self, line: str) -> str:
        """
        Decode a single log line.
        
        Args:
            line: The encoded log line to decode
        
        Returns:
            Decoded log line
        
        Example:
            >>> decoder = LogDecoder.from_file('mapping.txt')
            >>> decoded = decoder.decode_line('$A1B2 42!')
            >>> print(decoded)
            Value: 42
        """
        return self._decode_line(line.rstrip())


def decode_uart_logs(
    serial_port: str,
    baud_rate: int,
    mapping_file: str,
    output_file: Optional[str] = None,
    timeout: float = 1.0,
    print_console: bool = True
) -> None:
    """
    Decode logs from UART/serial port in real-time.
    
    This function continuously reads from a serial port and decodes each line
    using the mapping dictionary. Results can be printed to console and/or
    saved to a file.
    
    Args:
        serial_port: Serial port name (e.g., 'COM3' on Windows, '/dev/ttyUSB0' on Linux)
        baud_rate: Baud rate for serial communication (e.g., 115200)
        mapping_file: Path to mapping.txt file
        output_file: Optional path to save decoded logs (None to skip file output)
        timeout: Read timeout in seconds (default: 1.0)
        print_console: Whether to print decoded logs to console (default: True)
    
    Raises:
        IOError: If serial port or file operations fail
        ImportError: If pyserial is not installed
    
    Example:
        >>> decode_uart_logs('COM3', 115200, 'mapping.txt', output_file='uart_logs.txt')
        # Reads and decodes logs from COM3 at 115200 baud
    """
    try:
        import serial
    except ImportError:
        raise ImportError(
            "pyserial is required for UART decoding. "
            "Install it with: pip install pyserial"
        )
    
    # Load decoder
    print(f"Loading mapping from {mapping_file}...")
    decoder = LogDecoder.from_file(mapping_file)
    print(f"Loaded {len(decoder.mapping)} mappings")
    
    # Configure serial connection
    try:
        ser = serial.Serial(
            port=serial_port,
            baudrate=baud_rate,
            timeout=timeout,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS
        )
        print(f"Connected to {serial_port} at {baud_rate} baud")
        print("Reading logs... (Press Ctrl+C to stop)")
        
    except serial.SerialException as e:
        raise IOError(f"Failed to open serial port {serial_port}: {e}")
    
    # Open output file if specified
    output_handle = None
    if output_file:
        try:
            output_handle = open(output_file, "a", encoding="latin-1")
            print(f"Writing logs to {output_file}")
        except IOError as e:
            raise IOError(f"Failed to open output file {output_file}: {e}")
    
    from datetime import datetime
    
    try:
        while True:
            if ser.in_waiting > 0:
                # Read line
                line = ser.readline().decode('latin-1', errors='ignore').strip()
                
                if not line:
                    continue
                
                # Decode the line
                try:
                    decoded_line = decoder.decode_line(line)
                except Exception as e:
                    # If decoding fails, use original line
                    decoded_line = line
                    print(f"[Warning] Decoding failed: {e}")
                
                # Add timestamp
                timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]
                log_entry = f"[{timestamp}] {decoded_line}"
                
                # Print to console
                if print_console:
                    print(log_entry)
                
                # Write to file
                if output_handle:
                    output_handle.write(log_entry + '\n')
                    output_handle.flush()  # Ensure immediate write
                    
    except KeyboardInterrupt:
        print("\n\nStopping UART logger...")
    except Exception as e:
        print(f"\nError: {e}")
    finally:
        # Clean up
        ser.close()
        if output_handle:
            output_handle.close()
        print("UART connection closed")


def main():
    """Main entry point for the log decoder."""
    import argparse
    
    parser = argparse.ArgumentParser(
        description="Decode encoded log files using a mapping dictionary",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Decode log file:
  %(prog)s logs.txt mapping.txt
  %(prog)s logs.txt mapping.txt -o output.txt
  
  # Decode UART logs in real-time:
  %(prog)s --uart COM3 115200 mapping.txt
  %(prog)s --uart COM3 115200 mapping.txt -o uart_logs.txt
        """
    )
    
    parser.add_argument(
        "logs_file",
        nargs='?',
        help="Path to input log file (required if not using --uart)"
    )
    
    parser.add_argument(
        "mapping_file",
        help="Path to mapping.txt file (format: ID->string)"
    )
    
    parser.add_argument(
        "-o", "--output",
        default="decoded_logs.txt",
        help="Output file path for log file mode (default: decoded_logs.txt)"
    )
    
    parser.add_argument(
        "--uart",
        nargs=2,
        metavar=('PORT', 'BAUD'),
        help="UART mode: serial port and baud rate (e.g., COM3 115200)"
    )
    
    args = parser.parse_args()
    
    # UART mode
    if args.uart:
        try:
            serial_port = args.uart[0]
            baud_rate = int(args.uart[1])
            
            decode_uart_logs(
                serial_port=serial_port,
                baud_rate=baud_rate,
                mapping_file=args.mapping_file,
                output_file=args.output if args.output != "decoded_logs.txt" else None,
                print_console=True
            )
        except ValueError as e:
            print(f"Error: {e}", file=sys.stderr)
            sys.exit(1)
    
    # File decoding mode
    else:
        if not args.logs_file:
            parser.error("logs_file is required when not using --uart mode")
        
        try:
            print(f"Loading mapping from {args.mapping_file}...")
            decoder = LogDecoder.from_file(args.mapping_file)
            print(f"Loaded {len(decoder.mapping)} mappings")
            
            print(f"Decoding logs from {args.logs_file}...")
            decoder.decode(args.logs_file, args.output)
            
            print(f"Successfully decoded logs to {args.output}")
            
        except IOError as e:
            print(f"IO Error: {e}", file=sys.stderr)
            sys.exit(1)
        except ValueError as e:
            print(f"Value Error: {e}", file=sys.stderr)
            sys.exit(1)
        except Exception as e:
            print(f"Unexpected error: {e}", file=sys.stderr)
            sys.exit(1)


if __name__ == "__main__":
    main()
