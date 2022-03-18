/* This kernel module gives stack area details of child process of pid passed through character device
 * It hooks wake_up_new_task function to get task struct of process 
 * */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <asm-generic/set_memory.h>
#include <linux/kallsyms.h>
#include <linux/ftrace.h>
#include <linux/linkage.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/kprobes.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/ptrace.h>
#include <linux/list.h>
#include <linux/pid.h>

#define VA_SIZE 15
#define NUM_THREADS 64


static int mychardev_open(struct inode *inode, struct file *file);
static int mychardev_release(struct inode *inode, struct file *file);
static long mychardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static ssize_t mychardev_read(struct file *file, char __user *buf, size_t count, loff_t *offset);
static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);

uint8_t databuf[10];


static const struct file_operations mychardev_fops = {
    .owner      = THIS_MODULE,
    .open       = mychardev_open,
    .release    = mychardev_release,
    .unlocked_ioctl = mychardev_ioctl,
    .read       = mychardev_read,
    .write       = mychardev_write
};

struct mychar_device_data {
    struct cdev cdev;
};

struct ftrace_hook {
        const char *name;
        void *function;
        void *original; 
        unsigned long address;
        struct ftrace_ops ops;
};

struct list_head sysfs_list;

struct sdata{
    unsigned pid;
    unsigned long start;
    unsigned long end;
};

struct sysfs_data{
    struct sdata sd;
    struct kobj_attribute sysfs_attr;
    struct list_head list;
};

static asmlinkage void (*real_wake_up_new_task)(struct task_struct *p);

//static pid_t bench_pid = 0;
static int dev_major = 0;

static struct class *mychardev_class = NULL;

static struct mychar_device_data mychardev_data;

static struct kobject *example_kobject;


char name_list[NUM_THREADS+1][4];


static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
                      char *buf)
{
    struct list_head * temp_list_head;
    struct sysfs_data * temp_sysfs_data;
    unsigned int check = 0;
    list_for_each(temp_list_head, &sysfs_list){
        temp_sysfs_data = list_entry(temp_list_head, struct sysfs_data, list);
	if(!strcmp(temp_sysfs_data->sysfs_attr.attr.name,attr->attr.name)){
            memcpy(buf, &temp_sysfs_data->sd,sizeof(struct sdata));
	    check = 1;
	}
    }
    
    if(!check){ return check; }

    return sizeof(struct sysfs_data);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
                     const char *buf, size_t count)
{
    pr_info("Not used in this module");
    return count;
}


static asmlinkage void fh_wake_up_new_task(struct task_struct *tsk){

    pid_t driver_pid;
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    struct pt_regs *childregs, *currentregs;
    int j = 0;
    int error = 0;
    int check_flag = 0;
    int set_current_entry = 0;
    unsigned int sysfs_index;
    unsigned long start, end, length;
    vm_flags_t flags;
    struct task_struct *parent_tsk;

    struct list_head * temp_list_head;
    struct sysfs_data * temp_sysfs_data;
    
    if(kstrtoint(databuf,10,&driver_pid) != 0){}
    if(driver_pid == current->pid) 
           goto call_real;

    parent_tsk = current->real_parent;
    childregs = task_pt_regs(tsk);
    currentregs = task_pt_regs(current);
    //pr_info("tsk_pid:%u,comm:%s, current_pid:%u parent_pid:%u\n",tsk->pid,tsk->comm,current->pid,parent_tsk->pid);

     
    if(parent_tsk->pid == driver_pid){ 
	//pr_info("pid:%d task stack: %lx\n",tsk->pid,childregs->sp);
        //pr_info("pid: %d,tgid: %d \n",tsk->pid,tsk->tgid);
	mm = tsk->mm;
        vma = mm->mmap;
        down_read(&mm->mmap_sem);
        while (vma) {
            j++;
            start = vma->vm_start;
            end = vma->vm_end;
            length = end - start;
            flags = vma->vm_flags;
            //pr_info("%6d: %16p %12lx %12lx %8ld\n",j, vma, start, end, length);
            if((start<= childregs->sp && childregs->sp<=end) ||
	       (start <= currentregs->sp && currentregs->sp <= end)) {
                pr_info("stack vma\n");
		sysfs_index = 0;
		check_flag = 0;
		set_current_entry = 0;
                list_for_each(temp_list_head, &sysfs_list){
                    temp_sysfs_data = list_entry(temp_list_head, struct sysfs_data, list);
		    sysfs_index += 1;
		    if(temp_sysfs_data->sd.pid == current->pid){
		        set_current_entry = 1;
		    }
                    if((temp_sysfs_data->sd.pid == tsk->pid || temp_sysfs_data->sd.pid == current->pid) && (temp_sysfs_data->sd.start == start) && (temp_sysfs_data->sd.end == end)){
	                check_flag = 1; //there is a entry for this stack vma, proceed to next vma
		        break;
		    }
		}

		if(1 == check_flag){
                    pr_info("pid: %d stack entry exists with start:%lx end:%lx\n",temp_sysfs_data->sd.pid,start,end);
                    vma = vma->vm_next;
		    continue;
		}
                temp_sysfs_data = (struct sysfs_data*) kmalloc(sizeof(struct sysfs_data),GFP_KERNEL);
                temp_sysfs_data->sd.pid = tsk->pid;
		if(!set_current_entry && (start <= currentregs->sp && currentregs->sp <= end))
                     temp_sysfs_data->sd.pid = current->pid;

		temp_sysfs_data->sd.start = start;
		temp_sysfs_data->sd.end = end;
		temp_sysfs_data->sysfs_attr.attr.name = name_list[sysfs_index];
		temp_sysfs_data->sysfs_attr.attr.mode = 0666;
		temp_sysfs_data->sysfs_attr.show = foo_show;
		temp_sysfs_data->sysfs_attr.store = foo_store;
		list_add(&temp_sysfs_data->list,&sysfs_list);
		error = sysfs_create_file(example_kobject, &(temp_sysfs_data->sysfs_attr.attr));
		if (error == -EEXIST) 
			 pr_info("sysfs exists:");
		pr_info("Added pid:%u sysfs:%s %6d: %16p %12lx %12lx %8ld\n",temp_sysfs_data->sd.pid,temp_sysfs_data->sysfs_attr.attr.name,j, vma, start, end, length);
	    }
            vma = vma->vm_next;
	}
        up_read(&mm->mmap_sem);
    }
call_real:   
    real_wake_up_new_task(tsk);
}


static int mychardev_uevent(struct device *dev, struct kobj_uevent_env *env)
{
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}

static int resolve_hook_address(struct ftrace_hook * hook){
    hook->address = kallsyms_lookup_name(hook->name);
    if (!hook->address){
        pr_debug("unresolved symbol: %s\n", hook->name);
        return -ENOENT;
    }

    *((unsigned long *)hook->original) = hook->address;
    return 0;
}

static void notrace fh_ftrace_thunk(unsigned long ip, unsigned long parent_ip,
                struct ftrace_ops *ops, struct pt_regs *regs){

    struct ftrace_hook *hook = container_of(ops, struct ftrace_hook, ops);
    
    if (!within_module(parent_ip, THIS_MODULE)){
        regs->ip = (unsigned long) hook->function;
    }
}

int fh_install_hook(struct ftrace_hook *hook){
    int err;
    err = resolve_hook_address(hook);
    if (err)
        return err;
    
    hook->ops.func = fh_ftrace_thunk;
    hook->ops.flags = FTRACE_OPS_FL_SAVE_REGS|FTRACE_OPS_FL_IPMODIFY;

    err = ftrace_set_filter_ip(&hook->ops, hook->address, 0, 0);
    if (err) {
        pr_debug("ftrace_set_filter_ip() failed: %d\n", err);
        return err;
     }

    err = register_ftrace_function(&hook->ops);
    if (err) {
        pr_debug("register_ftrace_function() failed: %d\n", err);
        ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
        return err;
        }
    return 0;
}

void fh_remove_hook(struct ftrace_hook *hook)
{
    int err;
    err = unregister_ftrace_function(&hook->ops);
    if (err) {
        pr_debug("unregister_ftrace_function() failed: %d\n", err);
    }
    
    err = ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
    if (err) {
        pr_debug("ftrace_set_filter_ip() failed: %d\n", err);
    }
    pr_info("fh_remove_hook called\n");
}

struct ftrace_hook fh = {.name="wake_up_new_task", .function = fh_wake_up_new_task, .original = &real_wake_up_new_task};

static int __init mychardev_init(void)
{
    int err;
    
    dev_t dev;

    err = alloc_chrdev_region(&dev, 0, 1, "mychardev");
    if(err)
        return err;

    dev_major = MAJOR(dev);

    mychardev_class = class_create(THIS_MODULE, "mychardev");
    mychardev_class->dev_uevent = mychardev_uevent;

    cdev_init(&mychardev_data.cdev, &mychardev_fops);
    mychardev_data.cdev.owner = THIS_MODULE;
    cdev_add(&mychardev_data.cdev, MKDEV(dev_major, 0), 1);
    device_create(mychardev_class, NULL, MKDEV(dev_major, 0), NULL, "mychardev-0");
    
    
    
   return 0;
}

static void __exit mychardev_exit(void)
{
    
    pr_info("module unloaded\n");
    device_destroy(mychardev_class, MKDEV(dev_major, 0));
    class_unregister(mychardev_class);
    class_destroy(mychardev_class);
    unregister_chrdev_region(MKDEV(dev_major, 0),1);
    
}

static int mychardev_open(struct inode *inode, struct file *file)
{
    int err_fh;
    int name_loop = 0;
    pr_info("MYCHARDEV: Device open\n");
    err_fh = fh_install_hook(&fh);
    if (err_fh)
        return err_fh; 
    example_kobject = kobject_create_and_add("kobject_example", kernel_kobj);
    if(!example_kobject)
        return -ENOMEM;
   INIT_LIST_HEAD(&sysfs_list);
   while(name_loop<NUM_THREADS){
        sprintf(name_list[name_loop],"%u",name_loop);
        name_loop+=1;
    }
    return 0;
}

static int mychardev_release(struct inode *inode, struct file *file)
{
    printk("MYCHARDEV: Device close\n"); 
    fh_remove_hook(&fh);
    kobject_put(example_kobject);

    return 0;
}

static long mychardev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    printk("MYCHARDEV: Device ioctl\n");
    return 0;
}

static ssize_t mychardev_read(struct file *file, char __user *buf, size_t count, loff_t *offset)
{
    uint8_t *data = "Hello from the kernel world!\n";
   
    size_t datalen = strlen(data);

    printk("Reading device: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev));

    if (count > datalen) {
        count = datalen;
    }

    if (copy_to_user(buf, data, count)) {
        return -EFAULT;
    }
    printk(KERN_INFO "read call\n");
    return count;
}


static ssize_t mychardev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
    size_t maxdatalen = 10, ncopied;

    printk("Writing device: %d\n", MINOR(file->f_path.dentry->d_inode->i_rdev));

    if (count < maxdatalen) {
        maxdatalen = count;
    }

    ncopied = copy_from_user(databuf, buf, maxdatalen);

    if (ncopied == 0) {
        printk("Copied %zd bytes from the user\n", maxdatalen);
    } else {
        printk("Could't copy %zd bytes from the user\n", ncopied);
    }

    databuf[maxdatalen] = 0;

    printk("Data from the user: %s\n", databuf);

    return count;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("arunkp1986@gmail.com>");

module_init(mychardev_init);
module_exit(mychardev_exit);

