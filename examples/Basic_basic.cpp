@NAME: basic
@CATEGORY: Basic
#include <hFramework.h>

void hMain() {
    // your code!
}
#####
@PORTS: stm32
@BOARDS: robocore,core2,core2mini
@NAME: basic_led
@CATEGORY: Basic
#include <hFramework.h>

void hMain() {
    while(true) {
        LED1.toggle();
        sys.delay(500);
    }
}
#####
@PORTS: stm32
@NAME: basic_time
@CATEGORY: Basic
#include <hFramework.h>

void hMain() {
    while(true) {
        printf("uptime %d", sys.getRefTime());
        sys.delay(500);
    }
}
