  #include<stdio.h>
  #include<stdlib.h>
  #include<unistd.h>
  #include<sys/ioctl.h>
  #include<sys/stat.h>
  #include<sys/types.h>
  #include<fcntl.h>

  int main()
  {
	//打开设备文件
	int fd_ld,fd_step,fd_video;
	//fd_ld=open("/dev/LD",O_RDWR);
	fd_step=open("/dev/stepmotor",O_RDWR);
	//fd_video=open("/dev/video0",O_RDWR);
	//初始化各个设备
	//Init_camera(fd_video);
	//创建摄像头缓冲区
	//Creat_camera(fd_video);
	//电机回转，准备扫描
	int i=0;
	for(;i<9;i++)
	{
		ioctl(fd_step,0,30);
	}
	//打开激光器
	//ioctl(fd_ld,1,0);
	//创建图像读取线程同时步进电机开始转动，扫描开始
	//pthread_creat()
	for(i=0;i<18;i++)
	{
		ioctl(fd_step,1,30);
	}
	//通知图像采集线程结束
	//
	//步进电机返回初始位置，关闭设备文件
	for(i=0;i<9;i++)
	{
		ioctl(fd_step,0,30);
	}
	//close(fd_ld);
	close(fd_step);
	//close(fd_video);
	return 0;
  }
  
  
