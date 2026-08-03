#define BN_IMPLEMENTATION
#include "../bn.h"

int main(int argc, char* argv[]) {
    bn_initContext(BN_DEFAULT_CONTEXT_PARAMS);

    BN_arrayi32 test_da_array = bn_daInit(&bn_context->allocator);

    bn_daAppend(&test_da_array, 1);
    bn_daAppend(&test_da_array, 2);
    bn_daAppend(&test_da_array, 3);

    for (i32 i = 0; i < test_da_array.length; i++) {
        bn_logInfof("%d", test_da_array.items[i]);
    }

    bn_daFree(&test_da_array);

    BN_HashTable test_hash_table = bn_htInit(&bn_context->allocator);

    bn_htAppend(&test_hash_table, "test1", 123);
    bn_htAppend(&test_hash_table, "test2", 456);
    bn_htAppend(&test_hash_table, "test3", 789);

    bn_logInfof("%d", bn_htGet(&test_hash_table, "test2"));

    bn_hashTableFree(&test_hash_table);

    bn_destroyContext();
    return 0;
}
