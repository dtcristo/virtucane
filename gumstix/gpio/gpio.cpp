#include "gpio.h"

using namespace std;

void initGpio();
void setGpio(int pin);
void resetGpio(int pin);

int main(){
	initGpio();
	return 0;
}

void initGpio(){
	// Setup special pins
	system("devmem2 0x480021c8 h 0x10c");
	system("devmem2 0x480021ca h 0x10c");
	system("devmem2 0x480021cc h 0x10c");
	system("devmem2 0x480021ce h 0x10c");
	system("devmem2 0x480021d0 h 0x10c");
	cout << "done: special setup" << endl;
	
	// Export pins
	system("echo 171 > /sys/class/gpio/export");
	system("echo 172 > /sys/class/gpio/export");
	system("echo 174 > /sys/class/gpio/export");
	system("echo 173 > /sys/class/gpio/export");
	system("echo 175 > /sys/class/gpio/export");
	system("echo 146 > /sys/class/gpio/export");
	system("echo 145 > /sys/class/gpio/export");
	system("echo 147 > /sys/class/gpio/export");
	system("echo 144 > /sys/class/gpio/export");
	cout << "done: export" << endl;
	
	// Set as output pins
	system("echo out > /sys/class/gpio/gpio171/direction");
	system("echo out > /sys/class/gpio/gpio172/direction");
	system("echo out > /sys/class/gpio/gpio174/direction");
	system("echo out > /sys/class/gpio/gpio173/direction");
	system("echo out > /sys/class/gpio/gpio175/direction");
	system("echo out > /sys/class/gpio/gpio146/direction");
	system("echo out > /sys/class/gpio/gpio145/direction");
	system("echo out > /sys/class/gpio/gpio147/direction");
	system("echo out > /sys/class/gpio/gpio144/direction");
	cout << "done: set output" << endl;
	
	// Reset values to 0
	system("echo 0 > /sys/class/gpio/gpio171/value");
	system("echo 0 > /sys/class/gpio/gpio172/value");
	system("echo 0 > /sys/class/gpio/gpio174/value");
	system("echo 0 > /sys/class/gpio/gpio173/value");
	system("echo 0 > /sys/class/gpio/gpio175/value");
	system("echo 0 > /sys/class/gpio/gpio146/value");
	system("echo 0 > /sys/class/gpio/gpio145/value");
	system("echo 0 > /sys/class/gpio/gpio147/value");
	system("echo 0 > /sys/class/gpio/gpio144/value");
	cout << "done: reset values" << endl;
}

void setGpio(int pin){
	switch (pin)
	{
	case 1:
		system("echo 1 > /sys/class/gpio/gpio171/value");
		break;
	case 2:
		system("echo 1 > /sys/class/gpio/gpio172/value");
		break;
	case 3:
		system("echo 1 > /sys/class/gpio/gpio174/value");
		break;
	case 4:
		system("echo 1 > /sys/class/gpio/gpio173/value");
		break;
	case 5:
		system("echo 1 > /sys/class/gpio/gpio175/value");
		break;
	case 6:
		system("echo 1 > /sys/class/gpio/gpio146/value");
		break;
	case 7:
		system("echo 1 > /sys/class/gpio/gpio145/value");
		break;
	case 8:
		system("echo 1 > /sys/class/gpio/gpio147/value");
		break;
	case 9:
		system("echo 1 > /sys/class/gpio/gpio144/value");
		break;
	// Set all pins
	default:
		system("echo 1 > /sys/class/gpio/gpio171/value");
		system("echo 1 > /sys/class/gpio/gpio172/value");
		system("echo 1 > /sys/class/gpio/gpio174/value");
		system("echo 1 > /sys/class/gpio/gpio173/value");
		system("echo 1 > /sys/class/gpio/gpio175/value");
		system("echo 1 > /sys/class/gpio/gpio146/value");
		system("echo 1 > /sys/class/gpio/gpio145/value");
		system("echo 1 > /sys/class/gpio/gpio147/value");
		system("echo 1 > /sys/class/gpio/gpio144/value");
		break;
	}
}

void resetGpio(int pin){
	switch (pin)
	{
	case 1:
		system("echo 0 > /sys/class/gpio/gpio171/value");
		break;
	case 2:
		system("echo 0 > /sys/class/gpio/gpio114/value");
		break;
	case 3:
		system("echo 0 > /sys/class/gpio/gpio172/value");
		break;
	case 4:
		system("echo 0 > /sys/class/gpio/gpio174/value");
		break;
	case 5:
		system("echo 0 > /sys/class/gpio/gpio173/value");
		break;
	case 6:
		system("echo 0 > /sys/class/gpio/gpio175/value");
		break;
	case 7:
		system("echo 0 > /sys/class/gpio/gpio151/value");
		break;
	case 8:
		system("echo 0 > /sys/class/gpio/gpio148/value");
		break;
	case 9:
		system("echo 0 > /sys/class/gpio/gpio186/value");
		break;
	// Reset all pins
	default:
		system("echo 0 > /sys/class/gpio/gpio171/value");
		system("echo 0 > /sys/class/gpio/gpio172/value");
		system("echo 0 > /sys/class/gpio/gpio174/value");
		system("echo 0 > /sys/class/gpio/gpio173/value");
		system("echo 0 > /sys/class/gpio/gpio175/value");
		system("echo 0 > /sys/class/gpio/gpio146/value");
		system("echo 0 > /sys/class/gpio/gpio145/value");
		system("echo 0 > /sys/class/gpio/gpio147/value");
		system("echo 0 > /sys/class/gpio/gpio144/value");
		break;
	}
}