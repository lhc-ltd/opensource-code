#include <stdio.h>
#include <string.h>
#include "hashmap.h"

typedef struct {
    char *name;
    int age;
} User;

int user_cmp(const void *a, const void *b, void *udata) {
    const User *u1 = a;
    const User *u2 = b;
    return strcmp(u1->name, u2->name);
}

bool user_itr(const void *item, void *udata) {
    const User *user = item;
    printf("%s (age=%d)\n", user->name, user->age);
    return true;
}

uint64_t user_hash(const void *item, uint64_t seed0, uint64_t seed1) {
    const User *usr = item;
    return hashmap_sip(usr->name, strlen(usr->name), seed0, seed1);
}

int main() {
    // 创建一个新的哈希映射，其中每个元素都是一个`User`结构体。第二个参数是初始容量，
    // 第三个和第四个参数是可选的种子，用于指定后续的散列函数。
    struct hashmap *map = hashmap_new(sizeof(User), 0, 0, 0,
                                     user_hash, user_cmp, NULL, NULL);

    // 将一些用户加载到哈希映射中。每次设置操作都会复制第二参数指向的数据。
    hashmap_set(map, &(User){ .name="Dale", .age=44 });
    hashmap_set(map, &(User){ .name="Roger", .age=68 });
    hashmap_set(map, &(User){ .name="Jane", .age=47 });

    User *user;

    printf("\n-- 获取一些用户 --\n");
    user = hashmap_get(map, &(User){ .name="Jane" });
    printf("%s age=%d\n", user->name, user->age);

    user = hashmap_get(map, &(User){ .name="Roger" });
    printf("%s age=%d\n", user->name, user->age);

    user = hashmap_get(map, &(User){ .name="Dale" });
    printf("%s age=%d\n", user->name, user->age);

    user = hashmap_get(map, &(User){ .name="Tom" });
    printf("%s\n", user ? "exists" : "not exists");

    printf("\n-- 遍历所有用户（hashmap_scan）--\n");
    hashmap_scan(map, user_itr, NULL);

    printf("\n-- 遍历所有用户（hashmap_iter）--\n");
    size_t iter = 0;
    void *data;
    while (hashmap_iter(map, &iter, &data)) {
        const User *usr = data;
        printf("%s (age=%d)\n", usr->name, usr->age);
    }

    hashmap_free(map);
}

// 输出：
// -- 获取一些用户 --
// Jane age=47
// Roger age=68
// Dale age=44
// not exists
// 
// -- 遍历所有用户（hashmap_scan）--
// Dale (age=44)
// Roger (age=68)
// Jane (age=47)
//
// -- 遍历所有用户（hashmap_iter）--
// Dale (age=44)
// Roger (age=68)
// Jane (age=47)