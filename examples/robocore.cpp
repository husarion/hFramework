@PORTS: stm32
@BOARDS: robocore
@NAME: adc
@DESC: This example shows how to use A/D converter.
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	// enable ADC on our ports
	hSens4.pinIntAdc.enableADC();
	hExt1.pin7.enableADC();
	while (true)
	{
		// read analog values (voltage)
		float v1 = hSens4.pinIntAdc.analogReadVoltage();
		float v2 = hExt1.pin7.analogReadVoltage();
		printf("%f %f\r\n", v1, v2);
		sys.delay(50);
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: gpio
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	hExt1.pin1.setOut();
	while (true)
	{
		hExt1.pin1.toggle();
		sys.delay(300);
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: gpio_inout
@CATEGORY: GPIO
#include <hFramework.h>

void hMain()
{
	// read from pin1 with pull down resistor
	hExt1.pin1.setIn_pd();
	hExt1.pin2.setOut();
	while (true)
	{
		// replicate value on pin1 to pin2
		bool value = hExt1.pin1.read();
		hExt1.pin2.write(value);
		sys.delay(300);
	}
}


void hMain2()
{
	hExt1.pin1.setOutOD();
	while (true)
	{
		// toggle between high impedance and low state
		hExt1.pin1.toggle();
		sys.delay(500);
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: buttons_callback
@CATEGORY: Interfaces
#include <hFramework.h>

void onPress()
{
	hLED1.on();
	hMot1.setPower(500);
}

void onRelease()
{
	hLED1.off();
	hMot1.setPower(0);
}

void hMain()
{
	// onPress will be invoked when hBtn1 is pressed
	hBtn1.setOnPressHandler(onPress);

	// onRelease will be invoked when hBtn1 is released
	hBtn1.setOnReleaseHandler(onRelease);
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: buttons_simple
@CATEGORY: Interfaces
#include <hFramework.h>

void hMain()
{
	while (true)
	{
		bool state = hBtn1.isPressed();
		printf("%d\r\n", state);

		// Is the button pressed?
		if (state)
		{
			// if it is, hLEDs will be lit
			hLED1.on();
			hLED3.on();
		}
		else
		{
			// if it is, hLEDs will be turned off
			hLED1.off();
			hLED3.off();
		}
		// wait 50 ms
		sys.delay(50);
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: buttons_threads
@CATEGORY: Interfaces
#include <hFramework.h>

void buttons_thread_loop()
{
	while (true)
	{
		hBtn1.waitForPressed();
		printf("PRESSED\r\n");
		hMot1.setPower(500);
		hBtn1.waitForReleased();
		printf("RELEASED\r\n");
		hMot1.setPower(0);
	}
}

void hMain()
{
	// create thread handling buttons, so you can do other work in hMain
	sys.taskCreate(buttons_thread_loop);

	while (true)
	{
		hLED1.toggle();
		sys.delay(500);
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: can_recv
@CATEGORY: Interfaces
#include <hFramework.h>

void hMain()
{
	CAN.init();

	CAN_frameRx frame;
	bool frameReceived = CAN.waitFrame(frame, 100);
	if (frameReceived)
	{
		// access the frame in `frame` variable
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: can_send
@CATEGORY: Interfaces
#include <hFramework.h>

void hMain()
{
	CAN.init();

	CAN_frameTx tx;
	tx.StdId = 0;
	tx.ExtId = 0;
	tx.IDE = 0;
	tx.RTR = 0;
	tx.DLC = 2;
	tx.data[0] = 0xab;
	tx.data[1] = sys.getRefTime();
	CAN.sendFrame(tx);
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: hLED
@CATEGORY: hLED
#include <hFramework.h>

void hMain()
{
	hLED1.on();
	sys.delay(300);
	hLED2.on();
	sys.delay(300);
	hLED3.on();
	sys.delay(300);
	for (;;)
	{
		hLED1.toggle();
		hLED2.toggle();
		hLED3.toggle();
		sys.delay(50);
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: hLED
@CATEGORY: hLED
#include <hFramework.h>

void hMain()
{
	hLED1.on();
	sys.delay(300);
	hLED2.on();
	sys.delay(300);
	hLED3.on();
	sys.delay(300);
	for (;;)
	{
		hLED1.toggle();
		hLED2.toggle();
		hLED3.toggle();
		sys.delay(50);
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: hLED_onoff
@CATEGORY: hLED
#include <hFramework.h>

void hMain()
{
	hLED1.off(); // initially off
	hLED2.on(); // hLED2 will stay on
	while (true)
	{
		hLED1.toggle(); // toggle hLED1
		sys.delay(500); // wait for 500 ms
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: motor
@CATEGORY: Motors
#include <hFramework.h>

void encoder()
{
	while (true)
	{
		// Print the current rotation of hMot1 in encoder ticks
		printf("pos: %d\r\n", hMot1.getEncoderCnt());
		hLED1.toggle();
		sys.delay(100);
	}
}

void hMain()
{
	// This creates a task that will execute `encoder` concurrently
	sys.taskCreate(encoder);
	while (true)
	{
		// Set motor power to 500
		hMot1.setPower(500);
		// Sleep for 1 second
		sys.delay(1000);
		// Set motor power to 500 (in reverse direction)
		hMot1.setPower(-500);
		// Sleep for 1 second
		sys.delay(1000);
	}
}
#####

@PORTS: stm32
@BOARDS: robocore
@NAME: motor_angle
@CATEGORY: Motors
#include <hFramework.h>

void hMain()
{
	hMot1.rotRel(10);// rotate 10 encoder ticks right -> relative
	hMot1.rotRel(10);// rotate another 10 encoder ticks right -> relative
	
	hMot1.rotAbs(15);// rotate motor 5 encoder tics left -> absolute -> 15(set) - 20(curent) = -5
	hMot1.rotAbs(0);// rotate motor 15 encoder tics left to original position -> absolute -> 0(set) - 15(curent) = -15
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: motor_pid
@CATEGORY: Motors
#include <hFramework.h>

hPIDRegulator pidReg, pidReg2;// Creating PID regulator objects

void hMain()
{
	pidReg.setScale(1);// Setting Scale for PID regulator
	pidReg.setKP(40.0);// Setting the proportional coefficient of the PID controller
	pidReg.setKI(0.05);// Setting the integral coefficient of the PID controller
	pidReg.setKD(1000);// Setting the derivative coefficient of the PID controller
	pidReg.dtMs = 5;// Setting Scale
	pidReg.stableRange = 10;// Setting Scale
	pidReg.stableTimes = 3;// Setting Scale

	pidReg2 = pidReg;// Copy pid instances

	hMot1.attachPositionRegulator(pidReg);// Attaching PID #1 regulator to motor1
	hMot2.attachPositionRegulator(pidReg2);// Attaching PID #1 regulator to motor2

	sys.taskCreate([]()// This creates a task that will execute concurrently
	{
		while (true)// Task loop
		{
			sys.delay(50);// Sleep for 50 miliseconds
			Serial.printf("encoder ticks: %5d %5d\r\n", hMot1.getEncoderCnt(), hMot2.getEncoderCnt());// Print on Serial currently counted encoders ticks
		}
	});

	sys.taskCreate([]()// This creates another task that will execute concurrently
	{
		while (true)// Task loop
		{
			hMot1.rotRel(180, 1000, 1);// Rotate relatively motor1 180 tics right with 1000 power and block task until finished
			sys.delay(300);// Sleep for 300 miliseconds
		}
	});

	sys.taskCreate([]()// This creates third task that will execute concurrently
	{
		while (true)// Task loop
		{
			hMot2.rotAbs(180, 1000, 1);// Rotate absolutely motor1 to 180 tics with 1000 power and block task until finished
			sys.delay(300);// Sleep for 300 miliseconds
			hMot2.rotAbs(0, 1000, 1);// Rotate absolutely motor1 to 0 tics with 1000 power and block task until finished
			sys.delay(300);// Sleep for 300 miliseconds
		}
	});
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: motor_simple
@CATEGORY: Motors
#include <hFramework.h>

void hMain()
{
	while(true)// Main program loop
	{
		hMot1.setPower(500);// Set motor power to 500
		sys.delay(1000);// Sleep for 1000 miliseconds
		hMot1.setPower(-500);// Set motor power to 500 in reverse direction
		sys.delay(1000);// Sleep for 1000 miliseconds
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: motor_slew_rate
@CATEGORY: Motors
#include <hFramework.h>

void hMain()
{
	sys.taskCreate([]()// This creates a task that will control two motors
	{
		hMot2.setSlewRate(0.01);// Set motor2 rotation rise time to 10s
		for (;;)// Task loop
		{
			hMot1.setPower(400);// Set motor1 power to 500
			hMot2.setPower(400);// Set motor2 power to 500
			sys.delay(4000);// Sleep for 4000 miliseconds
			hMot1.setPower(-400);// Set motor power to 500 in reverse direction
			hMot2.setPower(-400);// Set motor power to 500 in reverse direction
			sys.delay(4000);// Sleep for 4000 miliseconds
		}
	});

	sys.delay(10000);// Sleep for 10000 miliseconds
	hMot2.setSlewRate(0);// Set motor2 rotation rise time to 0s
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: serial
@CATEGORY: Serial I/O
#include <hFramework.h>

void hMain()
{
	while (true)// Main program loop
	{
		if (Serial.available() > 0)// Checking Serial availability
		{
			char c = Serial.getch();// Pulling one character from Serial
			switch (c)// Deciding based on pulhLED charakter
			{
			case 'q':// In case of geting q
				Serial.printf("function 1\r\n");// Printing on Serial "function 1"
				break;// Breaking switch
			case 'a':// In case of geting a
				Serial.printf("function 2\r\n");// Printing on Serial "function 2"
				break;// Breaking switch
			default:// In other case
				Serial.printf("you pressed '%c'\r\n", c);// Printing on Serial "you pressed " then pressed charakter
				break;// Breaking switch
			}
		}
		sys.delay(10);// Sleep for 10 miliseconds
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: serial_config
@CATEGORY: Serial I/O
#include <hFramework.h>

void hMain()
{
	hExt1.serial.init(9600, Parity::Odd, StopBits::One);// Configure hExt1 serial with baudrate == 9600, odd parity and with one stop bit
	hSens3.selectSerial();// Switch hSens3 to serial mode and configure it with baudrate == 19200 and default settings
	hSens3.getSerial().init(19200);// Configure hSens3 serial with baudrate == 9600,
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: serial_stdio
@CATEGORY: Serial I/O
#include <hFramework.h>

void hMain()
{
	sys.setLogDev(&Serial);

	while (true)// Main program loop
	{
		char c = getchar();// Pulling one character from Serial
		switch (c)// Deciding based on pulhLED charakter
		{
		case 'q':// In case of geting q
			printf("function 1\r\n");// Printing on Serial "function 1"
			break;// Breaking switch
		case 'a':// In case of geting a
			printf("function 2\r\n");// Printing on Serial "function 2"
			break;// Breaking switch
		default:// In other case
			printf("you pressed '%c'\r\n", c);// Printing on Serial "you pressed " then pressed charakter
			break;// Breaking switch
		}
	}
}
@PORTS: stm32
@BOARDS: robocore
@NAME: basic
#include <hFramework.h>

void hMain() // main task containing your code
{
	hLED1.on(); // turn on build-in hLED #1 on PCB
	
	for(;;) // common loop for contious run!
	{
		hLED1.toggle(); // switch state of hLED1
		sys.delay(500); // wait 500ms
	}
}
#####
@PORTS: stm32
@BOARDS: robocore,
@NAME: motor_servo
@CATEGORY: Servo
#include <hFramework.h>

void hMain()
{
	IServo& servo = hMot1.useAsServo();// Enable usage of motor1 as servo
	servo.calibrate(-90, 700, 90, 1500);// Calibrate servo -> minAngle, minWidth, maxAngle, maxWidth

	while (true)// Main program loop
	{
		int time = sys.getRefTime();// Read current system time
		float pos = sinf(time / 3000.0f * 2 * M_PI);// Calculate successive sinusoidal values depend on time
		servo.rotAbs(pos * 90.0f);// Rotate motor absolutely to set value
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: sd
@CATEGORY: Storage
#include <hFramework.h>

void hMain()
{
	int r;
	hFile file;

	r = SD.mount();
	if (r == 0)
	{
		r = SD.openFile(file, "a.txt", hFile::MODE_OPEN_EXISTING | hFile::MODE_READ);
		printf("opening file for read %d\r\n", r);
		if (r == 0)
		{
			char d[30];
			r = file.read(d, 30);
			printf("read %d\r\n", r);
			d[r - 1] = 0;
			file.close();
			printf("str: %s\r\n", d);
		}

		r = SD.openFile(file, "b.txt", hFile::MODE_CREATE_ALWAYS | hFile::MODE_WRITE);
		printf("opening file write %d\r\n", r);
		if (r == 0)
		{
			char d[30];
			uint16_t written;
			int len = sprintf(d, "test %d\r\n", sys.getRefTime());
			r = file.write(d, len, &written);
			printf("write %d %d\r\n", r, written);
			file.close();
		}
		SD.unmount();
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: servo_module
@CATEGORY: Servo

#####
@PORTS: stm32
@BOARDS: robocore
@NAME: storage
@CATEGORY: Storage
#include <hFramework.h>

void hMain()
{
	// store a number at location 0
	sys.getStorage().store<int>(0, 42);
	// load a number from location 0
	int answer;
	sys.getStorage().load<int>(0, answer);

	// store a string at location 4
	const char* str = "foo";
	sys.getStorage().store(4, str, 3);
	// load a string from location 4
	char newstr[4] = {0};
	sys.getStorage().load(4, newstr, 3);
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: sys_mutex
@CATEGORY: System
#include <hFramework.h>

int counter = 0;
hMutex counter_mutex;

void add_1()
{
	// Modify counter in mutex to ensure that the modification is not lost
	counter_mutex.take();
	int new_value = counter + 1;
	counter = new_value;
	counter_mutex.give();
}

void adder()
{
	while (true)
	{
		add_1();
		sys.delay(500);
	}
}

void hMain()
{
	sys.taskCreate(&adder);
	while (true)
	{
		add_1();
		printf("fizz\n");
		sys.delay(1000);
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: sys_queue
@CATEGORY: System
#include <hFramework.h>

// Create queue with capacity for 60 elements
hQueue<unsigned int> queue(60);

void consumer()
{
	while (true)
	{
		unsigned int number;
		// Pop element from the from of the queue. Will block if the queue is empty.
		queue.receive(number);
		printf("consumed %d\n", number);
	}
}

void hMain()
{
	sys.taskCreate(&consumer);
	unsigned int i = 0;
	while (true)
	{
		// Push element to the queue. Will block if the queue is full.
		queue.sendToBack(i);
		i++;
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: sys_task
@CATEGORY: System
@DESC: This program will repeatedly print fizz buzz
#include <hFramework.h>

void buzzer()
{
	while (true)
	{
		printf("buzz\n");
		sys.delay(500);
	}
}

void hMain(void)
{
	sys.taskCreate(&buzzer);
	sys.delay(500);
	while (true)
	{
		printf("fizz\n");
		sys.delay(1000);
	}
}
#####
@PORTS: stm32
@BOARDS: robocore
@NAME: tasks
@CATEGORY: System
#include <hFramework.h>
#include <cmath>

void task1()
{
	for (;;)
	{
		int time = sys.getRefTime();
		float pos = sinf(time / 3000.0f * 2 * M_PI);
		// robAbs with true as 'block' parameter blocks execution until motor reaches desired position
		// angle, max power, block
		hMotA.rotAbs(pos * 90.0f, 1000, true);
	}
}

void task2()
{
	for (;;)
	{
		hLED1.toggle();
		sys.delay(50);
	}
}

void hMain()
{
	sys.taskCreate(task1);
	sys.taskCreate(task2);
}
