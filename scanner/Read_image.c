  #include"Read_image.h"

  #include<stdio.h>
  #include<errno.h>
  #include<sys/mman.h>
  #include<linux/videodev2.h>
  #include<sys/ioctl.h>
  #include<unistd.h>
  #include<fcntl.h>



  void Read_image(int fd,struct buffer *buffers)
  {
	///


	type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl(fd,VIDIOC_STREAMON,&type);
	///
	struct v4l2_buffer buf;
	buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	buf.memory=V4L2_MEMORY_MMAP;
	int filenum=0;
	char *fname=NULL;
	while(1)
	{
	ioctl(fd,VIDIOC_DQBUF,&buf);//从缓冲队列中取出一个帧缓冲
	//
	printf("write\t");
	sprintf(fname,"/3dscanner/image%d",filenum);
	printf("\n");
	FILE *file_fd=fopen(fname);
	//将图像写入文件以便进一步处理
	fwrite(buffers[buf.index].start,buffers[buf.index].length,1,file_fd);
	fclose(file_fd);
	}
  }
