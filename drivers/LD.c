  #include <linux/module.h>

  #include <linux/fs.h>
  #include <linux/errno.h>
  #include <linux/miscdevice.h>
  #include <linux/kernel.h>
  #include <linux/major.h>
  #include <linux/mutex.h>
  #include <linux/proc_fs.h>
  #include <linux/seq_file.h>
  #include <linux/stat.h>
  #include <linux/init.h>
  #include <linux/device.h>
  #include <linux/tty.h>
  #include <linux/kmod.h>
  #include <linux/gfp.h>
  #include <linux/init.h>
  #include <linux/fs.h>
  #include <linux/sched.h>
  #include <linux/pm.h>
  #include <linux/slab.h>
  #include <linux/sysctl.h>
  #include <linux/proc_fs.h>
  #include <linux/delay.h>
  #include <linux/gpio.h>
  #include <linux/poll.h>


  #define GPIO_TO_PIN(bank,gpio) (32*(bank)+(gpio))

  #define DEVICE_NAME	"LD"

  #ifdef CONFIG_LD_DEBUG
  #define dbg(format,x...)	pr_notice("led: "format,##x)
  #else
  #define dbg(format,x...)
  #endif


  #define LD_ON		1
  #define LD_OFF	0

  #define LD0		0

  typedef struct __GPIO_DESC {
	int gpio;
	const char *desc;
  }ld_desc;

  static ld_desc ld_data={GPIO_TO_PIN(1,16),"LD"};


  static long ld_ioctl(struct file *file,unsigned int cmd,unsigned long arg)
  {
	dbg("%s called,cmd=0x%x,arg=ox%x,\n",__func__,cmd,arg);
	if(cmd !=LD_ON && cmd != LD_OFF)
	{
		printk("ioctl cmd error!\n");
		return -1;
	}
	gpio_set_value(ld_data.gpio,cmd);
	return 0;
  }

//打开设备
  static int ld_open(struct inode *inode,struct file *file)
  {
	int i=0;
	int ret=-1;
	dbg("%s called\n",__func__);
	ret = gpio_request(ld_data.gpio,ld_data.desc);
	if(ret<0)
	{
		printk("failed to request GPIO %d{%s},error %d\n",ld_data.gpio,ld_data.desc,ret);
		return ret;
	}
	gpio_direction_output(ld_data.gpio,0);
	gpio_set_value(ld_data.gpio,1);
	msleep(500);
	gpio_set_value(ld_data.gpio,0);
	return 0;
  }

//关闭设备
  static int ld_close(struct inode *inode,struct file *file)
  {
	dbg("Close deviceLD sucessfully\n");
	gpio_free(ld_data.gpio);
	return 0;
  }


//接口注册

static struct file_operations ld_fops=
  {
	.owner		=	THIS_MODULE,
	.unlocked_ioctl	=	ld_ioctl,
	.open		=	ld_open,
	.release	=	ld_close,
  };

//设备结构设置
  static struct miscdevice misc ={
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= DEVICE_NAME,
	.fops	= &ld_fops,
  };

//设备初始化函数

  static int __init ld_t_init(void)
  {
	int ret;
	ret=misc_register(&misc);
	
	if(ret!=0)
	{
		printk("init ld unsuccessfully!\n");
		return -1;
	}
	printk("Register ld successfully!\n");
	return 0;
  }

//卸载函数

  static void __exit ld_t_exit(void)
  {
	misc_deregister(&misc);
	printk("ld exit!\n");
  }




  module_init(ld_t_init);
  module_exit(ld_t_exit);
  MODULE_LICENSE("GPL");
  MODULE_AUTHOR("kang.chen <759102187@qq.com> 2018.5.7");
  MODULE_DESCRIPTION("module Driver");
