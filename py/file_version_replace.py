#!/usr/bin/python
# -*- coding: utf-8  -*-

import string,os,sys


ver_section = 3

if len(sys.argv) < 4:
    print('usage:./xxx.py replace_file replace_string to_replace_file')
    sys.exit(1)

def find_position(str):
    p_count, ch_count = 0, 0

    for ch in str:
        ch_count += 1
        if ('.' == ch):
            p_count += 1
            if (ver_section == p_count):
                break
    if (p_count < ver_section - 1):
        print('Illegal version number')
        sys.exit(0)
    return ch_count            

def create_version_str(ver):
    version_str = ''
    tmp_str     = ''
    pos1, pos2, num, num1 = 0, 0, 0, 0

    while True:
        num += 1
        num1 += 1
        pos2 = ver.find('.', pos1, -1)
        if pos2 < 0:
            tmp_str = ver[pos1::1]
        else:
            tmp_str = ver[pos1:pos2:1]

        if (int(tmp_str) > 999):
            print('version node > 999, version:' + ver + ', node:.' + tmp_str)
            sys.exit(2)
        if (1 == num):
            tmp_str = '%d' % int(tmp_str)
        else:
            tmp_str = '%02d' % int(tmp_str)
        if num1 < 3:
            version_str += (tmp_str + '.')
        else:
            version_str += tmp_str

        if pos2 < 0:
            break
        pos1 = pos2 + 1
    return version_str

try:
    file1 = open(sys.argv[1])
except IOError as e:
    print("%s" % e)
    sys.exit(3)

ver_str = ''

for line in file1:
    ver_str = line
    break
file1.close()

ver = ver_str[:find_position(ver_str) - 1]

ver = create_version_str(ver)

tmp_file_name = sys.argv[3] + '.tmp'

try:
    file2 = open(sys.argv[3], 'r')
except IOError as e:
    print("%s" % e)
    sys.exit(4)

file3 = open(tmp_file_name, 'w')

replace_str = sys.argv[2]

found = 0
for line in file2.readlines():
    if line.find(replace_str) > 0 and 0 == found:
        found = 1
        line = line[:line.index(replace_str) + len(replace_str)]
        file3.write(line.replace(replace_str, replace_str + ' = "' + ver + '";\n'))
    else:
        file3.write(line)

file2.close()
file3.close()

os.rename(tmp_file_name, sys.argv[3])

