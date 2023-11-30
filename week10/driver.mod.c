#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
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
__used __section("__versions") = {
	{ 0xe45e768d, "module_layout" },
	{ 0xf7ce4723, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xa1493f32, "class_destroy" },
	{ 0x7183440, "device_destroy" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xfe990052, "gpio_free" },
	{ 0x9da64e56, "gpiod_direction_output_raw" },
	{ 0xd68b6952, "cdev_add" },
	{ 0xb772ecb4, "cdev_init" },
	{ 0xc17de8a, "device_create" },
	{ 0x8342f492, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0xdcb764ad, "memset" },
	{ 0xae064e15, "gpiod_set_raw_value" },
	{ 0xb83bb696, "gpio_to_desc" },
	{ 0x12a4e128, "__arch_copy_from_user" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x92997ed8, "_printk" },
	{ 0x1fdc7df2, "_mcount" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "7235276F725EFC1D814642D");
