#define BN_IMPLEMENTATION
#include "../bn.h"

int main(void) {
    String text = stringLit("test string");
    bnLogInfo(text.str);
    bnLogInfof("%s", text.str);
    bnLogWarnf("%.*s", stringFmt(text));

    bnAssert(true);

    BN_Allocator allocator = bnInitAllocator(
        (BN_AllocatorInitParams){.type = BN_AllocatorType_ArenaGrowing}
    );

    u32BNArray array = bnDaInit(&allocator);

    bnDaAppend(&array, 2);
    bnDaAppend(&array, 3);

    u32 items[] = {0, 1, 5};
    bnDaAppendMany(&array,items, arrayLength(items));

    for (u32 i = 0; i < array.count; i++) {
        bnLogInfof("%d", array.items[i]);
    }

    bnDaFree(&array);

    bnLogInfof("%d", array.count);

    BN_HashTable hash_table = bnHTInit(&allocator);

    bnHTAppend(&hash_table, "test", 1);
    bnHTAppend(&hash_table, "test2", 2);
    bnHTAppend(&hash_table, "test3", 3);
    bnHTAppend(&hash_table, "test4", 4);

    u32* hash_table_value = bnHTGet(&hash_table, "test3");

    bnLogInfof("%d", hash_table_value);

    bnAllocatorFreeAll(&allocator);

    return 0;
}
