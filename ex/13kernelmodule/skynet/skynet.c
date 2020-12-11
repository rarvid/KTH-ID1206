#include <linux/module.h>   // included for all kernel modules
#include <linux/kernel.h>   // included for KER_INFO
#include <linux/init.h>     // included for __init and __exit macros

#include <linux/proc_fs.h>  // file operations
#include <linux/seq_file.h> // seq_read, etc.

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rudolfs Arvids Kalnis");
MODULE_DESCRIPTION("Cringe");

static int skynet_show(struct seq_file *m, void *v);

static int skynet_open(struct inode *inode, struct file *file);

static const struct proc_ops skynet_fops = {
  .proc_open = skynet_open,
  .proc_read = seq_read,
  .proc_lseek = seq_lseek,
  .proc_release = single_release,
};

static int skynet_show(struct seq_file *m, void *v) {
here:
  seq_printf(m, "Skynet location: 0x%lx\n", (unsigned long)&&here);
  return 0;
}
static int skynet_open(struct inode *inode, struct file *file) {
  return single_open(file, skynet_show, NULL);
}

static int __init skynet_init(void) {
  proc_create("skynet", 0, NULL, &skynet_fops);
  printk(KERN_INFO "Skyner in control\n");

  return 0;
}

static void __exit skynet_cleanup(void) {
  remove_proc_entry("skynet", NULL);
  printk(KERN_INFO "I'll be back!\n");
}

module_init(skynet_init);
module_exit(skynet_cleanup);
