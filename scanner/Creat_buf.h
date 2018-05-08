  #ifndef CREAT_BUF_
  #define CREAT_BUF_
  

  struct buffer{
    void *start;
    unsigned int length;
  };  


  void Creat_buf(struct buffer *buf);



  #endif

