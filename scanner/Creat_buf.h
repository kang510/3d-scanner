  #ifndef CREAT_BUF_
  #define CREAT_BUF_
  

  #include<linux/videodev2.h>

  struct buffer{
    void *start;
    unsigned int length;
  };  


  void Creat_buf(struct buffer *buf,struct v4l2_requestbuffers *req);



  #endif

