#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xded39672, "module_layout" },
	{ 0x11a81e5a, "kobject_put" },
	{ 0x540f5484, "kmalloc_caches" },
	{ 0xf5bf96c2, "cdev_init" },
	{ 0x366c9548, "up_read" },
	{ 0x9b7fe4d4, "__dynamic_pr_debug" },
	{ 0x24791534, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x91715312, "sprintf" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x8463b4d6, "down_read" },
	{ 0xed42eb83, "kobject_create_and_add" },
	{ 0xb44ad4b3, "_copy_to_user" },
	{ 0xe472f1d, "current_task" },
	{ 0x7c32d0f0, "printk" },
	{ 0x983acb5c, "class_unregister" },
	{ 0x4d4bddcd, "device_create" },
	{ 0x801de1f9, "unregister_ftrace_function" },
	{ 0x373db350, "kstrtoint" },
	{ 0xe007de41, "kallsyms_lookup_name" },
	{ 0xe6ccec47, "cdev_add" },
	{ 0xafeda40e, "ftrace_set_filter_ip" },
	{ 0xdb7305a1, "__stack_chk_fail" },
	{ 0x2ea2c95c, "__x86_indirect_thunk_rax" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0xe8f45b1c, "kmem_cache_alloc_trace" },
	{ 0x1b9bcfb2, "kernel_kobj" },
	{ 0x91a5c02c, "register_ftrace_function" },
	{ 0x6457f81c, "class_destroy" },
	{ 0x110f6e16, "sysfs_create_file_ns" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0x7e3dceb7, "__class_create" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x6c07d933, "add_uevent_var" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "F27BBFC5B17468E1EAA491F");
