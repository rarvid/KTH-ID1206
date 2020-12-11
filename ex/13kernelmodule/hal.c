#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rudolfs Arvids Kalnins");
MODULE_DESCRIPTION("Heuristically programmed Algorithimc computer");

static int __init hal_init(void) {
here:
  printk(KERN_INFO "I'm here %p\n", &&here);
  return 0;
}

static void __exit hal_cleanup(void) {
  printk(KERN_INFO "I HAVE NO IDEA XDR\n;");
}

module_init(hal_init);
module_exit(hal_cleanup);
