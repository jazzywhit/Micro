#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x58334a4a, "module_layout" },
	{ 0x90f73b9b, "cdev_alloc" },
	{ 0x281c1af5, "cdev_del" },
	{ 0x45a30ebd, "kmalloc_caches" },
	{ 0x9ad71be8, "cdev_init" },
	{ 0x4aabc7c4, "__tracepoint_kmalloc" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x5bcdd1db, "parport_unregister_driver" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xe174aa7, "__init_waitqueue_head" },
	{ 0x25957f3c, "kmem_cache_alloc_notrace" },
	{ 0x70d1f8f3, "strncat" },
	{ 0xb72397d5, "printk" },
	{ 0x42224298, "sscanf" },
	{ 0x8248dc87, "parport_unregister_device" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0xb6ed1e53, "strncpy" },
	{ 0xb4390f9a, "mcount" },
	{ 0xf1cbf485, "parport_claim" },
	{ 0x241595a1, "parport_release" },
	{ 0x658e0cf7, "cdev_add" },
	{ 0xf433ea92, "parport_register_driver" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xda0fcf5, "parport_register_device" },
	{ 0xf09c7f68, "__wake_up" },
	{ 0x37a0cba, "kfree" },
	{ 0x701d0ebd, "snprintf" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=parport";


MODULE_INFO(srcversion, "B0E27E5F6CDC7DAAB592342");
