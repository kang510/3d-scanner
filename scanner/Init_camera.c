  #include"Init_camera.h"
  #include<stdio.h>
  #include<stdlib.h>
  #include<errno.h>
  #include<string.h>
  #include<sys/mman.h>
  #include<sys/ioctl.h>
  #include<linux/videodev2.h>
  #include<unistd.h>
  #include<fcntl.h>


  void Init_camera(int fd)
  { 
      /////
     //   printf("TK------->>>fd is %d\n",fd);  
      //////  
      struct v4l2_capability cap;  
      ioctl(fd,VIDIOC_QUERYCAP,&cap);  
      
      
      printf("TK---------->>>>>Driver Name:%s\nCard Name:%s\nBus info:%s\n",cap.driver,cap.card,cap.bus_info);  
      //////获取并打印摄像头信息
      


      struct v4l2_fmtdesc fmtdesc;  
      fmtdesc.index = 0; fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
      while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc) != -1){  
       printf("TK-------->>>>>fmtdesc.description is %s\n",fmtdesc.description);  
       fmtdesc.index ++;  
      }  
      //////  查询并打印摄像头支持格式
      struct v4l2_format fmt;  
      fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      ioctl(fd,VIDIOC_G_FMT,&fmt);  
      printf("TK----------->>>>>fmt.fmt.width is %d\nfmt.fmt.pix.height is %d\nfmt.fmt.pix.colorspace is %d\n",fmt.fmt.pix.width,fmt.fmt.pix.height,fmt.fmt.pix.colorspace);  
      //修改帧格式为jpeg
      fmt.fmt.pix.width       = 1920; 
      fmt.fmt.pix.height      = 1080;
      fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG;  
      ioctl(fd,VIDIOC_S_FMT,&fmt);  
  }
