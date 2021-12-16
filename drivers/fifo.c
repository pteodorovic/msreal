#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/cdev.h>


#if 0
  #define DPRINT(args...) printk(args)
#else
  #define DPRINT(args...)
#endif
#define xDPRINT(args...)

#define FIFO_MAJOR 240
#define BUFF_SIZE 16
static unsigned long strToInt(const char* pStr, int len, int base);
static void write(char*);
static void read(void);
static char chToUpper(char ch);
static int fifo_open(struct inode* inode, 
                    struct file* file);
                    
static int fifo_close(struct inode* inode, 
                     struct file* file);
                     
		      							
static ssize_t fifo_write(struct file *p_file, 
	                			 const char *p_buf, 
		        						 size_t count, 
                         loff_t *p_pos);

static struct file_operations fifo_fops = {
  .owner   = THIS_MODULE,
  .write   = fifo_write,
  .open    = fifo_open,
  .release = fifo_close,
};
/******************************************************************************
 * Global variables
 *****************************************************************************/
static struct cdev* fifo_cdev = NULL;
static int RefCnt=0;
u8 fifo_buff[BUFF_SIZE];
u8 *pFirst=&fifo_buff[0];
u8 *pLast=&fifo_buff[0];
int offset = 0;


/******************************************************************************
 * Local functions
 *****************************************************************************/
//****************************
static char chToUpper(char ch)
{
  if((ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'))
  {
    return ch;
  }
  else
  {
    return ch - ('a'-'A');
  }   
}
//********************************
static unsigned long strToInt(const char* pStr, int len, int base)
{
  //                      0,1,2,3,4,5,6,7,8,9,:,;,<,=,>,?,@,A ,B ,C ,D ,E ,F
  static const int v[] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15}; 
  int i   = 0;
  unsigned long val = 0;
  int dec = 1;
  int idx = 0; 

  for(i = len; i > 0; i--)
  {
    idx = chToUpper(pStr[i-1]) - '0';    

    if(idx > sizeof(v)/sizeof(int))
    {
      printk("strToInt: illegal character %c\n", pStr[i-1]);
      continue;
    }
    
    val += (v[idx]) * dec;
    dec *= base;
  }

  return val; 
}

//********************************************
static void write(char* str)
{
	 int br,len=0;
	 len=strlen(str);
	
	 if(strchr(str, '\n') != NULL)
     { str[len]='\0';
		 len--;
	 }
	
    br=strToInt(str, len, 10);
	 if(br>255)
	 {
	 	printk("Prekoracenje, max 255\n");
	 	return;
	 }
	 if(offset>=BUFF_SIZE)
	 {
		printk("Buffer is full\n");
		return;
	 }
	 *pLast=br;
	 if(pLast==&fifo_buff[BUFF_SIZE-1])
	 {
	 	pLast=fifo_buff;
	 }
	 else
	 {
	 	pLast++;
	 }
	 offset++;		
		
}
//**********************************
static void read(void)
{

	if(offset==0)
	{
		printk("Buffer empty\n");
		return;
	}
	printk("%d\n",*pFirst);
	if(pFirst==&fifo_buff[BUFF_SIZE-1])
	{
		pFirst=fifo_buff;
	}
	else
	{
		pFirst++;
	}
	offset--;

}





//*******************
static int fifo_open(struct inode* inode,struct file* file)
{
  int minor=0;
 
  minor = MINOR(inode->i_rdev);

  DPRINT("fifo_open \n");

  if(minor != 0)
  {
    return -ENODEV;
  }

  RefCnt++;

  return 0;
}
//*********************
static int fifo_close(struct inode* inode,struct file* file)
{
  int minor = 0;

  minor = MINOR(inode->i_rdev);

  DPRINT("fifo_close \n");

  if(minor != 0)
  {
    return -ENODEV;
  }

  RefCnt--;

  return 0;
}

//***********************
static ssize_t fifo_write(struct file *p_file, const char *p_buf,size_t count, loff_t *p_pos)
{                         
    char pReg [100];;
    char* ptr=NULL;
	 char* nl = NULL;
    char* zarez=NULL;
    int len=0,i=0;
    int num_read=0;

	 /*
	 for (i=0; i<100; i++)
		 pReg[i]=0;
   */ 
	 copy_from_user(pReg, p_buf, count);
    if(!strncmp(pReg,"W",1))
    {	
    	ptr=strchr(pReg,':');
    	if(ptr==NULL)
    	{
    		printk("Wrong entry.");
    		return 0;
    	}
    	ptr=ptr+1;
    	zarez=pReg;
    	while(strchr(zarez,',')!=NULL)
    	{
    		
    		zarez=strchr(zarez,',');
    		*zarez='\0';
    		write(ptr);
    		ptr=zarez+1;
    		zarez++;
    	}
    	
    	write(ptr);
    	
    }
    else if(!strncmp(pReg,"R",1))
    {	
    	ptr=strchr(pReg,':');
    	if(ptr==NULL)
    	{
    		printk("Wrong entry.");
    		return 0;
    	}
    	ptr=ptr+1;
    	len=strlen(ptr);
    	if(strchr(ptr, '\n') != NULL)
	  	{ 
			nl = strchr(ptr,'\n');
			*nl = '\0';
		 	len=strlen(ptr);
	  	}
    	
    	num_read=strToInt(ptr, len, 10);
    	for (i=0;i<num_read;i++)read();
    }
    else
    {
    	printk("Wrong entry, count");
    }
  	
    return count;             
}

static void __exit fifo_mod_exit(void)
{
  dev_t dev;

  DPRINT("fifo_mod_exit\n");

  dev = MKDEV(FIFO_MAJOR, 0);

  cdev_del(fifo_cdev);

  unregister_chrdev_region(dev, 1);
}



//*****************************

static int __init fifo_mod_init(void){
  int ret;
  dev_t dev;

  DPRINT("fifo_mod_init\n");

  dev = MKDEV(FIFO_MAJOR, 0);

  ret = register_chrdev_region(dev, 1, "fifo");

  if(ret)
  {
    printk("fifo: failed to register char device\n");
    return ret;
  }

  fifo_cdev = cdev_alloc();
  fifo_cdev->ops   = &fifo_fops;
  fifo_cdev->owner = THIS_MODULE;

  ret = cdev_add(fifo_cdev, dev,1 );

  if (ret)
  {
    printk("fifo: Error adding fifo\n");
    //posto nije uspela registracija cdev strukture, tj. drajvera, vrati nazad 
    //prethodno registrovanu strukturu kernelu na koriscenje
    unregister_chrdev_region(dev, 1);
  }

  return ret;
}


MODULE_LICENSE("GPL");

module_init(fifo_mod_init);
module_exit(fifo_mod_exit);















