#include <hFramework.h>

void hMain()
{
    // setup USB comunication
	sys.setSysLogDev(&devNull);
	sys.setLogDev(&Serial);
	Serial.init(115200);
    
    hFile file;     // declare file object
    char d[30];     // make space for new data to write
    char s[60];     // make place for read data
	
    int len = sprintf(d, "test %d\n\r", (int)sys.getRefTime());  // make a string to write
    printf("\r\n\tbytes to write:\r\n%d\n\r\tstring:\r\n%s\r\n", len, d); // show it

    SD.mount(); // attach SD card to the system

    // open file with hFile::FileMode arguments
    SD.openFile(file, "a.txt", hFile::MODE_CREATE_ALWAYS | hFile::MODE_WRITE | hFile::MODE_READ); 

    file.printf("%s",d);    // write string using 'printf' method
    file.puts(d);           // write string using 'puts' method
    for(int i=0;i<len;i++){
        file.putch(d[i]);    // write all chars using 'putch' method
    }

    file.seek(0);   // back to begin of file

	int num = file.read(s, 60); // try to read 60 chars, return number of bytes read from the file
    printf("\r\n\tread bytes:\r\n%d\n\r\tstring:\r\n%s\r\n", num, s); // show it
	file.close();   // close file atfer all operations on it

	SD.unmount();   // detach SD card from the system
}
