#!/usr/bin/python
import sys
import os
import re
import glob
import operator

gpioPath = os.path.dirname(os.path.realpath(
    __file__)) + "/../ports/stm32/src/hPeriph/"

verMap = {
    "robocore": 2,
    "core2": 3,
    "core2mini": 4,
}


def genPath(path):
    base = os.path.basename(path)

    pins = []

    curNr = 0
    adcNr = 0
    lineNr = 1
    for line in open(path).readlines():
        parts = re.split("\s*", line.strip())

        entry = {}
        port = None
        if len(parts) >= 3:
            entry['port'] = parts[0]
            entry['pin'] = int(parts[1])
            entry['name'] = parts[2]
            entry['adc_nr'] = -1

        for p in parts[3:]:
            if p.startswith("EXTI"):
                entry['irq'] = p
            if p.startswith("ADC"):
                parts = p.split("_")
                entry['adc'] = p
                entry['adc_nums'] = [int(x) for x in parts[0][3:]]
                entry['adc_channel'] = int(parts[1][2:])
                entry['adc_nr'] = adcNr
                adcNr += 1
        # print(entry)

        if 'port' in entry:
            entry['nr'] = curNr
            curNr += 1
            pins.append(entry)
        lineNr += 1

# f = open(gpioPath + "myGPIO_" + base + "_defs.h", "wb")
# for pin in pins:
        # f.write("#define {0[name]:25s} {0[nr]:3d}\n".format(pin).encode("ascii"))
# f.close()

    f = open(gpioPath + "myGPIO_" + base + ".h", "wb")
    for pin in pins:
        if 'irq' in pin:
            f.write("DEFINE_PORT_IRQ({0[name]:28s}, {0[nr]:3d}, {0[port]:s}, {0[pin]:2d}, {0[adc_nr]:2d}, {0[irq]:14s})\n".format(
                pin).encode("ascii"))
        else:
            f.write("DEFINE_PORT____({0[name]:28s}, {0[nr]:3d}, {0[port]:s}, {0[pin]:2d}, {0[adc_nr]:2d})\n".format(
                pin).encode("ascii"))

    f.write(b"\n")

    # interrupts
    for extiNr in range(0, 16):
        gpio = list(filter(lambda x: x['pin'] == extiNr and 'irq' in x, pins))
        if len(gpio) > 1:
            raise ValueError("multiple irqs on the same pin number")
        elif len(gpio) > 0:
            gpio = gpio[0]
            f.write(
                "DEFINE_EXTI({0[name]:26s}) // {0[pin]}\n".format(gpio).encode("ascii"))
        else:
            f.write("DEFINE_EXTI({0[name]:26s}) // {0[pin]}\n".format(
                {'name': '-1', 'pin': extiNr}).encode("ascii"))

    f.write(b"\n")

    # balance adcs
    max_len = -1
    adcs_map = {}
    pin_to_adc_map = {}
    for pin in filter(lambda x: 'adc' in x, pins):
        adc_nums = pin['adc_nums']
        for num in adc_nums:
            adcs_map[num] = []
            max_len = max(max_len, len(adc_nums))

    def get_least_loaded(adc_nums):
        return min([(adc_num, len(v)) for adc_num, v in adcs_map.items() if adc_num in adc_nums],
                   key=operator.itemgetter(1))[0]

    for cur_len in range(1, max_len + 1):
        for pin in filter(lambda x: 'adc' in x, pins):
            adc_nums = pin['adc_nums']
            if len(adc_nums) == cur_len:
                least_loaded = get_least_loaded(adc_nums)
                adcs_map[least_loaded] += pin
                pin_to_adc_map[pin['nr']] = least_loaded

    # adc
    for pin in filter(lambda x: 'adc' in x, pins):
        adc = pin['adc']
        adcNum = pin_to_adc_map[pin['nr']]
        channel = pin['adc_channel']
        f.write("DEFINE_ADC({adcNum}, {channel:2d}) // {0[adc_nr]:2d} {0[adc]:11s} {0[name]:26s}\n".format(
            pin, adcNum=adcNum, channel=channel).encode("ascii"))
        print(pin, adcNum, channel)

    f.close()

path = sys.argv[1]

f = open(gpioPath + "myGPIO_include.h", "wb")

f.write(b"""
#ifndef DEFINE_PORT____
#define DEFINE_PORT____(name,nr,port,pin,adc)
#endif
#ifndef DEFINE_PORT_IRQ
#define DEFINE_PORT_IRQ(name,nr,port,pin,adc,irq)
#endif
#ifndef DEFINE_EXTI
#define DEFINE_EXTI(name)
#endif
#ifndef DEFINE_ADC
#define DEFINE_ADC(adcNum, channel)
#endif

#include <hTypes.h>

""".lstrip())

first = True
for defFile in glob.glob(path + "/*"):
    base = os.path.basename(defFile)

    parts = base.split("_")

    a = parts[0]
    b = parts[1]
    c = parts[2]
    type = parts[3].lower()

    print(base, defFile)

    f.write("""#{el}if BOARD_VERSION_EQ({type},{a},{b},{c})
#include "myGPIO_{a}_{b}_{c}_{typel}.h"
""".format(type=type.upper(), typel=type.lower(), typeNum=verMap[type], a=a, b=b, c=c, el="" if first else "el").encode("ascii"))

    first = False
    genPath(defFile)

f.write(b"""#else
#error no board
#endif

#undef DEFINE_PORT____
#undef DEFINE_PORT_IRQ
#undef DEFINE_EXTI
#undef DEFINE_ADC
""")

exit()
