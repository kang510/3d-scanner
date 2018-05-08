  #include"Creat_buf.h"
  #include<stdio.h>
  #include<errno.h>
  #include<sys/mman.h>
  #include<linux/videodev2.h>
  #include<sys/ioctl.h>
  #include<unistd.h>
  #include<fcntl.h>
  



  void Creat_buf(struct buffer *buffers,struct v4l2_requestbuffers *req)
  { 
    ///
        
      req->count = 4;  
      req->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
      req->memory = V4L2_MEMORY_MMAP;  
      ioctl(fd,VIDIOC_REQBUFS,req);  //帧缓冲 v4l2_requestbuffers并为req分配内存  
      buffers = (struct buffer*)calloc (req->count, sizeof(*buffers));  
      unsigned int n_buffers = 0;  
      for(n_buffers = 0; n_buffers < req->count; ++n_buffers){  
        struct v4l2_buffer buf;  
        memset(&buf,0,sizeof(buf));  
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
        buf.memory = V4L2_MEMORY_MMAP;  
        buf.index = n_buffers;  
        if(ioctl(fd,VIDIOC_QUERYBUF,&buf) == -1){  
          printf("TK---------_>>>>>>error\n");  
          close(fd);  
          exit(-1);  
        }  //映射地址到用户空间
        buffers[n_buffers].length = buf.length;  
        buffers[n_buffers].start = mmap(NULL,buf.length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,buf.m.offset);  
        if(MAP_FAILED == buffers[n_buffers].start){  
          printf("TK--------__>>>>>error 2\n");  
          close(fd);  
          exit(-1);  
        }  
      }
  }
