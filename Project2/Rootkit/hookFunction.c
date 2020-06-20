#include <asm/unistd.h>
#include <asm/cacheflush.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <asm/pgtable_types.h>
#include <linux/highmem.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <linux/slab.h>
#include <asm/cacheflush.h>
#include <linux/fdtable.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cao Gia Hung");

void **system_call_table_addr;

asmlinkage int (*original_open) (const char*, int, int);
asmlinkage int (*original_write) (unsigned int, const char*, int);

asmlinkage int hook_open (const char* file, int flags, int mode) {
    char* buf = kmalloc(256, GFP_KERNEL);
    copy_from_user(buf, file, 256);
    printk(KERN_INFO "%s opened %s\n", current->comm, buf);
    kfree(buf);
    return original_open(file, flags, mode);
}

asmlinkage int hook_write (unsigned int fd, const char* buf, int count) {
    char* fileName = kmalloc(256, GFP_KERNEL);
    char* cwd = d_path(&files_fdtable(current->files)->fd[fd]->f_path, fileName, 256);
    printk(KERN_INFO "%s wrote %d byte(s) in %s", current->comm, count, cwd);
    kfree(fileName);
    return original_write(fd, buf, count);
}

int make_rw(unsigned long address){
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    if(pte->pte &~_PAGE_RW){
        pte->pte |=_PAGE_RW;
    }
    return 0;
}

int make_ro(unsigned long address){
    unsigned int level;
    pte_t *pte = lookup_address(address, &level);
    pte->pte = pte->pte &~_PAGE_RW;
    return 0;
}

static int __init entry_point(void){
    printk(KERN_INFO "Successfully Hook!...\n ");
    system_call_table_addr = (void*)0xffffffff81a001c0;

    make_rw((unsigned long)system_call_table_addr);

    original_open = system_call_table_addr[__NR_open];
    system_call_table_addr[__NR_open] = hook_open;
    
    original_write = system_call_table_addr[__NR_write];
    system_call_table_addr[__NR_write] = hook_write;

    return 0;
}

static int __exit exit_point(void){
    system_call_table_addr[__NR_open] = original_open;
    system_call_table_addr[__NR_write] = original_write;
    make_ro((unsigned long)system_call_table_addr);
    return 0;
}

module_init(entry_point);
module_exit(exit_point);
