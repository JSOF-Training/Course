#! /usr/bin/env python3

from elftools.elf.elffile import ELFFile




LAYER1_MAGIC = 0x566ba210
STR_PATTERN = 0xb4dc0ffeeb4dc0d3
FUNC_PATTERN = 0xbe31337dec0deb17
BURN_MAGIC = 0x693bd700c1583d8c


TOP_BIT = 1 << 63
BITMASK = (1 << 64) - 1
class LFSR():
    def __init__(self, pattern, iv):
        self.pattern = pattern & BITMASK
        self.iv = iv & BITMASK
        self.buffer = iv & BITMASK

    def roll_once(self):
        if (0 != TOP_BIT & self.buffer):
            self.buffer = ((self.buffer << 1) & BITMASK) ^ self.pattern
        else:
            self.buffer = ((self.buffer << 1) & BITMASK)
        return self.buffer

    def roll_n(self, n):
        for i in range(n):
            self.roll_once()
        return self.buffer

class Patches():
    def __init__(self, elf):
        self.elf = elf
    def get_patch(self, function):
        symtab = self.elf.get_section_by_name('.symtab')
        text = self.elf.get_section_by_name('.text')
        vr_offset = text['sh_offset'] - text['sh_addr']
        start = symtab.get_symbol_by_name(function.get_start())[0]['st_value']
        end = symtab.get_symbol_by_name(function.get_end())[0]['st_value']
        start_f = start + vr_offset
        end_f = end + vr_offset
        return {"start": start_f, "end": end_f, "magic": function.get_magic()}

    def get_patches(self, funcs):
        patches = [self.get_patch(f) for f in funcs]
        patches.sort(key = lambda p:p['start'])
        return patches

def get_layer1(elf):
    symtab = elf.get_section_by_name('.symtab')
    text = elf.get_section_by_name('.text')
    vr_offset = text['sh_offset'] - text['sh_addr']
    start = symtab.get_symbol_by_name('layer1_start')[0]['st_value']
    end = symtab.get_symbol_by_name('layer1_end')[0]['st_value']
    start_f = start + vr_offset
    end_f = end + vr_offset
    return (start_f, end_f)




class Function():
    def __init__(self, name, magic):
        self.name = name
        self.start = name
        self.end = name + "_e"
        self.magic_name = name.upper() + "_MAGIC"
        self.magic = magic

    def get_start(self):
        return self.start

    def get_end(self):
        return self.end

    def get_magic(self):
        return self.magic

    def get_end_dec(self):
        return f"void {self.end}(void);\n"

    def get_magic_def(self):
        return f"#define {self.magic_name} ({hex(self.magic)})\n"

    def get_patched_function(self, elffile):
        with open(infile, 'rb') as f:
            elf = ELFFile(f)
            # Find the symbol table.
            symtab = elf.get_section_by_name('.symtab')
            text = elf.get_section_by_name('.text')
            vr_offset = text['sh_offset'] - text['sh_addr']
            start = symtab.get_symbol_by_name(self.start)[0]['st_value']
            end = symtab.get_symbol_by_name(self.end)[0]['st_value']
            size = (end - start)
            location = value + vr_offset


function_list = [
    #("my_read", 0x55c14053fb1f3b25),
    #("my_write",0x709ad78fbd34b385),
    #("unscramble_func",0),
    #("my_puts",0x27fd5d025cc764bf),
    #("my_gets",0x823870d6c1bbf7ea),
    ("actual_code",0xdd0ee72e3c422637),
    ("patch_jump",0x27fd5d025cc764bf),
    ("real_check_pass",0x29b5c577705c6eea),
    ("fake_check_pass",0x9ca689fef82f3057),
    ("scramble_string",0x1ccf6c3175684d08),
    ("unscramble_string",0x400acd706dd5b6c8),
    ("getstr_and_patch",0x823870d6c1bbf7ea)
]

funcs = [Function(name, magic) for name, magic in function_list]

def get_all_patches(elf_file):
    p = Patches(elf_file)

    return p.get_patches(funcs)

def get_func_defs():
    return ("".join([f.get_magic_def() for f in funcs]) + "\n" +
            "".join([f.get_end_dec() for f in funcs]))
