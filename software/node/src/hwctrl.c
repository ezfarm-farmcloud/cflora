
#include "hwctrl.h"



int KeyInput(void)
{
		int fd;	
		int ret;
		char dev_name[20]= "/dev/input/event3"; // 키입력 디바이스 드라이버 
		struct input_event *key_event; 



		fd = open(dev_name, O_RDONLY);
		if(fd < 0)
		{
			printf("error to open /dev/input/event3\n");
		} 
		key_event = malloc(sizeof(struct input_event));
		memset(key_event,0,sizeof(key_event));
		
		
		ret = read(fd,key_event,sizeof(struct input_event)); 
		if(ret < 0)
		{
			printf("key event read error\n");
		}

		close(fd);		
		
		if(key_event->type == EV_KEY && key_event->value==1)
		{
		
			return key_event->code ;
		}
		

		return 0 ;
}

void Buzzer( char onoff )
{
	int fd;
	int ret = 0;
	int buzzer_on= 8;
	int buzzer_off= 7;

	unsigned int i, num;
	char read_pin_buf;

	fd = open("/dev/ctl_io",O_RDWR);
	if(fd < 0)
	{
		perror("error to open /dev/ctl_io\n");
		return ;
	}
	if( onoff == 1 )
			ret = write(fd, &buzzer_on,4);
	 else 
			ret = write(fd, &buzzer_off,4);
	
	if(fd < 0)
	{
		printf("error to open /dev/ctl_io\n");
		return ;
	}
		
	close(fd);
}

void BuzzerBeep( void )
{
		Buzzer(1) ; 
		usleep(200*1000) ;
		Buzzer(0) ;
		usleep(100*1000) ;
}