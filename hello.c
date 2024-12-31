#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <asm-generic/bug.h>

MODULE_AUTHOR("Markelov Ivan");
MODULE_DESCRIPTION("Lab6");
MODULE_LICENSE("Dual BSD/GPL");

#define BUG_ON_MSG(condition, msg) do { \
    if (unlikely(condition)) { \
        printk(KERN_WARNING msg); \
        BUG(); \
    } \
} while (0)

static unsigned int N = 1;
static unsigned int allocations = 0;

struct event {
    struct list_head list;
    ktime_t event_time;
};

module_param(N, uint, S_IRUGO);
MODULE_PARM_DESC(N, "N times Hello, World! print");

static LIST_HEAD(event_list_head);

void new_event(void);

static int __init hello_init(void) {
    int i;

    if (N == 0 || (N >= 5 && N <= 10)) {
        printk(KERN_WARNING "N is 0 or in the range [5,10]\n");
    }

    BUG_ON(N > 10);

    for (i = 0; i < N; i++) {
        printk(KERN_EMERG "Hello, World!\n");
        new_event();
    }

    return 0;
}

static void __exit hello_exit(void) {
    struct event *md, *tmp;

    list_for_each_entry_safe(md, tmp, &event_list_head, list) {
        printk(KERN_EMERG "Time: %lld\n", md->event_time);
        list_del(&md->list);
        kfree(md);
    }
}

void new_event(void) {
    struct event *element = NULL;

    element = kmalloc(sizeof(struct event), GFP_KERNEL);
    allocations++;

    BUG_ON_MSG(!element, "kmalloc returned NULL\n");
    element->event_time = ktime_get();
    list_add_tail(&element->list, &event_list_head);
}

module_init(hello_init);
module_exit(hello_exit);
