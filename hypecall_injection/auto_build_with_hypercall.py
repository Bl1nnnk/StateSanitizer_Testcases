
#coding:utf-8

import re
import os
import sys
import shutil

out_dir = 'out'
hc_fname = 'hypercall'

cc = 'cl.exe'
asm_cc = 'ml64.exe'
linker = "link.exe"

def usage():
    print("*.py testcase_file_name")
    
def build(source_name, hc_fname, out_dir):
    shutil.copyfile('agent.h', f'{out_dir}/agent.h')
    shutil.copyfile(f'{hc_fname}.asm', f'{out_dir}/{hc_fname}.asm')

    os.chdir(out_dir)
        
    if 0!= os.system(f'{asm_cc} /c {hc_fname}.asm'):
        return
    
    if 0 != os.system(f'{cc} -DWIN32 /c {source_name}.cpp'):
        return
        
    os.system(f'{linker} -out:{source_name}.exe {source_name}.obj {hc_fname}.obj ')

def inject_hc(filename):
    # 打开待读取的文件function_name.cpp
    with open(f'{filename}.cpp', 'r') as file_a, open(f'./{out_dir}/{filename}.cpp', 'w+') as file_b:

        # 在新文件b.cpp开头添加#include "agent.h"
        file_b.write('#include "agent.h"\n')

        # 标记是否已找到main函数
        found_main = False
        found_main_comma = False

        # 逐行读取文件a.cpp
        for code in file_a:

            # 将当前行写入新文件b.cpp
            file_b.write(code)

            # 找到main函数开始的第一行
            if not found_main and code.strip().startswith('int main('):
                found_main = True
            if found_main and not found_main_comma and code.strip().startswith('{'):
                file_b.write('    ss_hypercall(SS_HC_SUBMIT_CR3, 0);\n')
                file_b.write('    ss_hypercall(SS_HC_BEGIN_TEST, 0);\n')
                found_main_comma = True

        # 关闭文件
        file_a.close()
        file_b.close()
        
def main():
    if len(sys.argv) != 2:
        usage()
        return
        
    if not os.path.exists(out_dir):
        os.mkdir(out_dir)
        
    filename = sys.argv[1]
    inject_hc(filename)
    build(filename, hc_fname, out_dir)
        
if __name__ == "__main__":
    main()