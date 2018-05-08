	#include<linux/module.h>
	#include<linux/fs.h>
	#include<linux/errno.h>
	#include<linux/miscdevice.h>
	#include<linux/kernel.h>
	#include<linux/major.h>
	#include<linux/mutex.h>
	#include<linux/proc_fs.h>
	#include<linux/seq_file.h>
	#include<linux/stat.h>
	#include<linux/init.h>
	#include<linux/device.h>
	#include<linux/tty.h>
	#include<linux/kmod.h>
	#include<linux/gfp.h>
	#include<linux/sched.h>
	#include<linux/pm.h>
	#include<linux/slab.h>
	#include<linux/sysctl.h>
	#include<linux/delay.h>
	#include<linux/gpio.h>
	#include<linux/poll.h>
	#include<linux/delay.h>


	#define GPIO_TO_PIN(bank,gpio) (32*(bank)+(gpio))

	#define DEVICE_NAME        "stepmotor"


	#ifdef CONFIG_LED_DEBUG
	#define dbg(format,x...)       pr_notice("IN:"format,##x)
	#else
	#define dbg(format,x...)
	#endif

	#define FORWARD		1
	#define REVERSAL	0

	#define IN1		0
	#define IN2		1
	#define IN3		2
	#define IN4		3


	unsigned int step[4][4]={{1,0,1,0},
				  {0,1,1,0},
				  {0,1,0,1},
				  {1,0,0,1}};//步进电机四拍


	typedef struct __GPIO_DESC{
		int gpio;
		const char *desc;
	}stepmotor_desc;



	static stepmotor_desc stepmotor_data[]={
		{GPIO_TO_PIN(1,28),"IN1"},
		{GPIO_TO_PIN(1,31),"IN2"},
		{GPIO_TO_PIN(3,19),"IN3"},
		{GPIO_TO_PIN(3,21),"IN4"},
	};//步进电机四线数据
	

	static long stepmotor_ioctl(struct file *file,unsigned int cmd,unsigned long delayms)
	{	
		int i,j;
		if(cmd !=FORWARD && cmd != REVERSAL)
		{
			printk("ioctl cmd error!\n");
			return -1;
		}
		if(delayms<=0)
		{
			printk("ioctl delayms error!\n");
		}
		switch(cmd)
		{
		case FORWARD:
		for(i=0;i<4;i++)
		{
		for(j=0;j<4;j++)//gpio置值
		{
			gpio_set_value(stepmotor_data[j].gpio,step[i][j]);
		}//步进电机正转行进一步
		//gpio 值输出
		for(j=0;j<4;j++)
		{
			printk("%s's value:%d\n",stepmotor_data[j].desc,gpio_get_value(stepmotor_data[j].gpio));
		}
		printk("one step has set\n");		
		mdelay(delayms);
		}
		break;
		case REVERSAL:
		for(i=3;i>=0;i--)
		{
		for(j=0;j<4;j++)
		{
			gpio_set_value(stepmotor_data[j].gpio,step[i][j]);
		}
		mdelay(delayms);
		}
		break;
		default:
		printk("ioctl error!\n");
		return -1;
		}
		return 0;
	}
	static int stepmotor_open(struct inode *inode,struct file *file)
	{
		int i=0;
		int ret=-1;
		dbg("%s called\n",__func__);
		for(i=0;i<sizeof(stepmotor_data)/sizeof(stepmotor_desc);i++)
		{
		ret=gpio_request(stepmotor_data[i].gpio,stepmotor_data[i].desc);
		if(ret<0)
		{
			printk("failed to request GPIO %d{%s},error %d\n",stepmotor_data[i].gpio,stepmotor_data[i].desc,ret);
			return ret;
		}
		gpio_direction_output(stepmotor_data[i].gpio,0);
		printk("%s(%d) has set\n",stepmotor_data[i].desc,i);
		}
		//gpio测试
		for (i=0;i<4;i++)
		{
		printk("%s(%d) set to %d\n",stepmotor_data[i].desc,i,gpio_get_value(stepmotor_data[i].gpio));
		}
		return 0;
	}


	static int stepmotor_close(struct inode *inode,struct file *file)
	{
		int i=0;
		dbg("Close device successfully!\n");
		for(i=0;i<sizeof(stepmotor_data)/sizeof(stepmotor_desc);i++)
		{
			gpio_free(stepmotor_data[i].gpio);
		}
		return 0;
	}//关闭设备接口
	static struct file_operations stepmotor_fops=
	{
		.owner=THIS_MODULE,
		.unlocked_ioctl=stepmotor_ioctl,
		.open=stepmotor_open,
		.release=stepmotor_close,
	};//接口注册
	

	//设备结构设置
	static struct miscdevice misc=
	{
		.minor=		MISC_DYNAMIC_MINOR,
		.name=		DEVICE_NAME,
		.fops=		&stepmotor_fops,
	};



	//设备初始化函数

	static int __init stepmotor_init(void)
	{
		int ret;
		ret=misc_register(&misc);
		if(ret!=0)
		{
			printk("init stepmotor unsuccessfully!\n");
			return -1;
		}
		printk("Register stepmotor successfully!\n");
		return 0;
	}

	//卸载函数
	static void __exit stepmotor_exit(void)
	{
		misc_deregister(&misc);
		printk("led exit!\n");
	}

	module_init(stepmotor_init);
	module_exit(stepmotor_exit);
	MODULE_LICENSE("GPL");
	MODULE_AUTHOR("Kang Chen <759102187@qq.com> 2018.5.5");
	MODULE_DESCRIPTION("module Driver");
