"""
UART Log Capturing Script
=========================
A simple script to capture and save logs from UART/Serial port.
Supports real-time log decoding using a mapping dictionary.

Requirements:
    pip install pyserial

Usage:
    # Normal mode (capture raw logs):
    python uart_logger.py --port COM3 --baud 115200 --output uart_logs.txt
    
    # Decoded mode (decode logs using mapping file):
    python uart_logger.py --port COM3 --baud 115200 --mapping mapping.txt --decode
    
    # Interactive mode with decoding:
    python uart_logger.py --port COM3 --baud 115200 --mapping mapping.txt --decode --input
"""

import serial
import serial.tools.list_ports
import argparse
import datetime
import os
import signal
import sys

# Import LogDecoder from the refactored module
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
try:
    from logs_decoder_refactored import LogDecoder
    DECODER_AVAILABLE = True
except ImportError:
    DECODER_AVAILABLE = False
    print("Warning: LogDecoder module not found. Decoding disabled.")


def list_available_ports():
    """List all available serial ports."""
    print("\nAvailable Serial Ports:")
    print("-" * 40)
    ports = serial.tools.list_ports.comports()
    if not ports:
        print("  No ports found!")
    else:
        for i, port in enumerate(ports, 1):
            print(f"  {i}. {port.device}")
            print(f"     Description: {port.description}")
            if port.hwid:
                print(f"     HWID: {port.hwid}")
    print("-" * 40)
    return ports


def create_output_file(output_path):
    """Create output file with header."""
    os.makedirs(os.path.dirname(output_path) if os.path.dirname(output_path) else '.', exist_ok=True)
    
    with open(output_path, 'w',encoding="latin-1") as f:
        f.write("=" * 60 + "\n")
        f.write(f"UART Log Capture - Started: {datetime.datetime.now()}\n")
        f.write("=" * 60 + "\n\n")
    
    return output_path


def capture_logs(port, baud_rate, output_file, timeout=0.01, enable_input=False,
                decoder=None, decode_enabled=False):
    """
    Capture logs from UART port and optionally send commands.
    
    Args:
        port: Serial port (e.g., 'COM3' or '/dev/ttyUSB0')
        baud_rate: Baud rate (e.g., 9600, 115200)
        output_file: Path to save logs
        timeout: Read timeout in seconds
        enable_input: If True, allow sending commands via terminal
        decoder: LogDecoder instance for real-time decoding
        decode_enabled: If True, decode logs using the decoder
    """
    print(f"\nConnecting to {port} at {baud_rate} baud...")
    
    try:
        # Open serial connection
        ser = serial.Serial(
            port=port,
            baudrate=baud_rate,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=timeout,
            write_timeout=timeout
        )
        
        print(f"Connected! Logging to: {output_file}")
        if decode_enabled:
            print("Decoding mode: ENABLED - Logs will be decoded in real-time")
        if enable_input:
            print("Input mode: ENABLED - Type commands to send to UART")
        print("Press Ctrl+C to stop capturing...\n")
        print("-" * 60)
        
        # Signal handler for graceful exit
        running = [True]
        def signal_handler(sig, frame):
            print("\n\nStopping capture...")
            running[0] = False
            ser.close()
            with open(output_file, 'a',encoding="latin-1") as f:
                f.write("\n" + "=" * 60 + "\n")
                f.write(f"Log Capture Ended: {datetime.datetime.now()}\n")
                f.write("=" * 60 + "\n")
            print(f"Logs saved to: {output_file}")
            sys.exit(0)
        
        signal.signal(signal.SIGINT, signal_handler)
        
        # Open file for appending
        with open(output_file, 'a',encoding="latin-1") as f:
            import threading
            import queue
            
            # Queue for user input
            input_queue = queue.Queue()
            
            # Input thread function
            def input_thread():
                try:
                    while running[0]:
                        try:
                            # Get user input
                            user_input = input()
                            if user_input.lower() in ['quit', 'exit']:
                                input_queue.put(None)  # Signal to exit
                                break
                            input_queue.put(user_input + '\n')
                        except (EOFError, KeyboardInterrupt):
                            input_queue.put(None)
                            break
                except:
                    pass
            
            # Start input thread if enabled
            if enable_input:
                t = threading.Thread(target=input_thread, daemon=True)
                t.start()
            
            # Main read/write loop
            while running[0]:
                try:
                    # Read line from serial port
                    if ser.in_waiting > 0:
                        line = ser.readline()
                        try:
                            # Try to decode as UTF-8
                            raw_line = line.decode('latin-1', errors='replace').rstrip()
                        except:
                            # Fallback to ascii
                            raw_line = line.decode('latin-1', errors='replace').rstrip()
                        
                        # Apply decoding if enabled
                        if decode_enabled and decoder:
                            try:
                                decoded_line = decoder.decode_line(raw_line)
                                display_line = decoded_line
                            except Exception as e:
                                # If decoding fails, use original line
                                display_line = raw_line
                        else:
                            display_line = raw_line
                        
                        # Clean up carriage returns - UART sends \r\n but we only want \n
                        # \r causes overwriting in terminals and messes up text files
                        display_line = display_line.replace('\r', '')
                        
                        # Skip empty lines to avoid extra blank lines in output
                        if not display_line:
                            continue
                        
                        # Print to console
                        print(display_line)
                        
                        # Write to file - only decoded line with proper newline
                        # This ensures output matches logs captured without encoding
                        f.write(display_line + '\n')
                        f.flush()
                    
                    # Check for user input to send
                    if enable_input and not input_queue.empty():
                        user_data = input_queue.get()
                        if user_data is None:  # Exit signal
                            print("\nExiting...")
                            break
                        
                        # Send to serial port
                        ser.write(user_data.encode('latin-1'))
                        ser.flush()
                        
                except serial.SerialException as e:
                    print(f"Serial Error: {e}")
                    break
                    
        ser.close()
        
    except serial.SerialException as e:
        print(f"Error: Could not open port {port}")
        print(f"Details: {e}")
        sys.exit(1)


def main():
    parser = argparse.ArgumentParser(
        description='Capture logs from UART/Serial port with optional real-time decoding',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
    # Normal mode (raw logs):
    python uart_logger.py --port COM3 --baud 115200
    
    # Decoded mode:
    python uart_logger.py --port COM3 --baud 115200 --mapping mapping.txt --decode
    
    # Interactive mode with decoding:
    python uart_logger.py --port COM3 --baud 115200 --mapping mapping.txt --decode --input
    
    # Linux example:
    python uart_logger.py --port /dev/ttyUSB0 --baud 9600 --mapping mapping.txt --decode
        """
    )
    
    parser.add_argument('--port', '-p', type=str, default=None,
                        help='Serial port (e.g., COM3, /dev/ttyUSB0)')
    parser.add_argument('--baud', '-b', type=int, default=115200,
                        help='Baud rate (default: 115200)')
    parser.add_argument('--output', '-o', type=str, default='encode_decode_logs/uart_logs.txt',
                        help='Output file path (default: encode_decode_logs/uart_logs.txt)')
    parser.add_argument('--list', '-l', action='store_true',
                        help='List available serial ports')
    parser.add_argument('--timeout', '-t', type=float, default=0.01,
                        help='Read timeout in seconds (default: 0.01)')
    parser.add_argument('--input', '-i', action='store_true',
                        help='Enable terminal input to send commands to UART')
    parser.add_argument('--mapping', '-m', type=str, default=None,
                        help='Path to mapping.txt file for log decoding')
    parser.add_argument('--decode', '-d', action='store_true',
                        help='Enable real-time log decoding (requires --mapping)')
    
    args = parser.parse_args()
    
    # List ports if requested
    if args.list:
        list_available_ports()
        return
    
    # Check if port is specified
    if not args.port:
        print("Error: No port specified!")
        print("Use --list to see available ports")
        print("Use --port <PORT> to specify a port")
        list_available_ports()
        return
    
    # Initialize decoder if requested
    decoder = None
    decode_enabled = False
    
    if args.decode:
        if not DECODER_AVAILABLE:
            print("Error: LogDecoder module not available. Cannot enable decoding.")
            print("Make sure 'logs_decoder_refactored.py' is in the same directory.")
            return
        
        if not args.mapping:
            print("Error: --decode requires --mapping <mapping_file>")
            return
        
        try:
            print(f"Loading mapping from {args.mapping}...")
            decoder = LogDecoder.from_file(args.mapping)
            print(f"Loaded {len(decoder.mapping)} mappings")
            decode_enabled = True
        except Exception as e:
            print(f"Error loading mapping file: {e}")
            return
    
    # Create output file and start capturing
    create_output_file(args.output)
    capture_logs(args.port, args.baud, args.output, args.timeout, args.input,
                decoder, decode_enabled)


if __name__ == "__main__":
    main()