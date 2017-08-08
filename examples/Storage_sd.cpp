@PORTS: stm32
@BOARDS: core2,core2mini,robocore
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
			int len = sprintf(d, "test %ld\r\n", sys.getRefTime());
			r = file.write(d, len, &written);
			printf("write %d %d\r\n", r, written);
			file.close();
		}
		SD.unmount();
	}
}
