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
    // ����һ���µĹ�ϣӳ�䣬����ÿ��Ԫ�ض���һ��`User`�ṹ�塣�ڶ��������ǳ�ʼ������
    // �������͵��ĸ������ǿ�ѡ�����ӣ�����ָ��������ɢ�к�����
    struct hashmap *map = hashmap_new(sizeof(User), 0, 0, 0,
                                     user_hash, user_cmp, NULL, NULL);

    // ��һЩ�û����ص���ϣӳ���С�ÿ�����ò������Ḵ�Ƶڶ�����ָ������ݡ�
    hashmap_set(map, &(User){ .name="Dale", .age=44 });
    hashmap_set(map, &(User){ .name="Roger", .age=68 });
    hashmap_set(map, &(User){ .name="Jane", .age=47 });

    User *user;

    printf("\n-- ��ȡһЩ�û� --\n");
    user = hashmap_get(map, &(User){ .name="Jane" });
    printf("%s age=%d\n", user->name, user->age);

    user = hashmap_get(map, &(User){ .name="Roger" });
    printf("%s age=%d\n", user->name, user->age);

    user = hashmap_get(map, &(User){ .name="Dale" });
    printf("%s age=%d\n", user->name, user->age);

    user = hashmap_get(map, &(User){ .name="Tom" });
    printf("%s\n", user ? "exists" : "not exists");

    printf("\n-- ���������û���hashmap_scan��--\n");
    hashmap_scan(map, user_itr, NULL);

    printf("\n-- ���������û���hashmap_iter��--\n");
    size_t iter = 0;
    void *data;
    while (hashmap_iter(map, &iter, &data)) {
        const User *usr = data;
        printf("%s (age=%d)\n", usr->name, usr->age);
    }

    hashmap_free(map);
}

// �����
// -- ��ȡһЩ�û� --
// Jane age=47
// Roger age=68
// Dale age=44
// not exists
// 
// -- ���������û���hashmap_scan��--
// Dale (age=44)
// Roger (age=68)
// Jane (age=47)
//
// -- ���������û���hashmap_iter��--
// Dale (age=44)
// Roger (age=68)
// Jane (age=47)