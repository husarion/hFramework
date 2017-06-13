from __future__ import print_function
import sys

header = '''/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */'''

def fix(name):
    data = open(name).readlines()
    end_notice = 0

    while True:
        if end_notice >= len(data):
            break

        while data[end_notice].strip() == '':
            end_notice += 1

        if not data[end_notice].strip().startswith('/*'):
            break

        while not data[end_notice].strip().startswith('*/'):
            end_notice += 1

        end_notice += 1

    newdata = header + '\n\n' + ''.join(data[end_notice:]).lstrip()
    with open(name, 'w') as f:
        f.write(newdata)

for name in sys.argv[1:]:
    print('processing', name)
    fix(name)
