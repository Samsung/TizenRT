import gdb
import os

# TizenRT build output directory. The path is taken from a GDB variable set in launch.json.
BUILD_OUTPUT_PATH = gdb.convenience_variable("build_output_path").string()

class ElfSymbolLoader(gdb.Breakpoint):
    """
    Automatically loads symbols into GDB when a binary is loaded.
    Sets a breakpoint at binfmt_loadbinary.c:192 to detect when a binary is loaded.
    """
    def __init__(self):
        super(ElfSymbolLoader, self).__init__('binfmt_loadbinary.c:192', gdb.BP_BREAKPOINT, internal=False)
        self.loaded_symbols = []

    def stop(self):
        """
        This method is executed when the breakpoint is hit.
        It reads information from the 'load_attr' and 'bin' variables to load the symbol file.
        """
        try:
            # Get the current stack frame.
            frame = gdb.selected_frame()

            # Find the 'load_attr' local variable.
            load_attr_var = frame.read_var('load_attr')
            if not load_attr_var:
                print("[AUTOSYM] Could not find 'load_attr' variable.")
                return False # Continue execution.

            # Find the 'bin' local variable.
            bin_var = frame.read_var('bin')
            if not bin_var:
                print("[AUTOSYM] Could not find 'bin' variable.")
                return False # Continue execution.

            # Extract the binary name from the load_attr struct.
            load_attr_val = load_attr_var.dereference()
            elf_name_ptr = load_attr_val['bin_name']
            elf_name = elf_name_ptr.string()

            # Extract the .text section address from the bin struct.
            bin_val = bin_var.dereference()
            text_addr = bin_val['sections'][0] # sections[BIN_TEXT] == sections[0]

            if not elf_name or text_addr == 0:
                print("[AUTOSYM] ELF name or .text address not available.")
                return False # Continue execution.

            # Create the symbol file path on the host. Use the file with '_dbg' suffix which includes debug symbols.
            host_elf_path = os.path.join(BUILD_OUTPUT_PATH, elf_name + '_dbg')

            if not os.path.exists(host_elf_path):
                print(f"[AUTOSYM] Symbol file not found on host: {host_elf_path}")
                return False # Continue execution.

            # Create and execute the GDB command to load symbols.
            cmd = f"add-symbol-file {host_elf_path} {text_addr}"
            print(f"[AUTOSYM] Executing: {cmd}")
            gdb.execute(cmd)
            self.loaded_symbols.append(host_elf_path)

        except gdb.error as e:
            print(f"[AUTOSYM] GDB Error: {e}")
        except Exception as e:
            print(f"[AUTOSYM] An unexpected error occurred: {e}")

        # Do not stop at the breakpoint, continue execution immediately.
        return False

class SymbolCleaner(gdb.Breakpoint):
    """
    Initializes the loaded symbols when the system reboots.
    Sets a breakpoint at '__start' to detect a reboot.
    """
    def __init__(self, symbol_loader):
        super(SymbolCleaner, self).__init__('__start', gdb.BP_BREAKPOINT, internal=False)
        self.symbol_loader = symbol_loader

    def stop(self):
        """
        This method is executed when the breakpoint is hit.
        It removes all symbols loaded by ElfSymbolLoader.
        """
        print("[AUTOSYM] System reboot detected. Clearing loaded symbols.")
        for symbol_file in self.symbol_loader.loaded_symbols:
            try:
                cmd = f"remove-symbol-file {symbol_file}"
                print(f"[AUTOSYM] Executing: {cmd}")
                gdb.execute(cmd)
            except gdb.error as e:
                print(f"[AUTOSYM] GDB Error while removing symbol file: {e}")
        self.symbol_loader.loaded_symbols.clear()
        # Do not stop at the breakpoint, continue execution immediately.
        return False

# Instantiate breakpoint handlers when the script is sourced.
elf_loader = ElfSymbolLoader()
SymbolCleaner(elf_loader)

print("[AUTOSYM] ELF symbol loader and cleaner scripts loaded.")
print("[AUTOSYM] Breakpoint for loading symbols set at binfmt_loadbinary.c:192.")
print("[AUTOSYM] Breakpoint for cleaning symbols set at __start.")
