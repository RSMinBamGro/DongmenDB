//
// Created by Sam on 2018/1/26.
//

#ifndef DONGMENDB_TRANSACTION_H
#define DONGMENDB_TRANSACTION_H

#include "DongmenDB.h"

#include <vector>
#include <map>

using namespace std;


#define  END_OF_FILE -1

class BufferManager;
class DongmenDB;
class RecoveryManager;
class ConcurrencyManager;

typedef void *void_ptr;

static int next_tx_num = 1;


class BufferList {
public:
    map<string, MemoryBuffer*> *buffers;
    vector<DiskBlock *> pins;
    BufferManager *bufferManager;

    int buffer_list_pin(DiskBlock *block);
    int buffer_list_unpin(DiskBlock *block);
    int buffer_list_unpin_all();
    MemoryBuffer *buffer_list_get_buffer(DiskBlock *block);
    int buffer_list_pin_new(char *fileName, void_ptr *block, TableInfo *tableInfo);

} ;

/** Transaction 即事务 */
class Transaction {
public:
    DongmenDB *db;
    int txNum;
    RecoveryManager *recoveryManager;
    ConcurrencyManager *concurrencyManager;
    BufferList *bufferList;

    Transaction(DongmenDB *db);
    int transaction_commit();
    int transaction_rollback();
    int transaction_recover();

    int transaction_pin(DiskBlock *block);
    int transaction_unpin(DiskBlock *block);

    int transaction_getint(DiskBlock *block, int offset);
    int transaction_setint(DiskBlock *block, int offset, int value);
    int transaction_getstring(DiskBlock *block, int offset, char *value);
    int transaction_setstring(DiskBlock *block, int offset, const char *value);

    int transaction_size(char *fileName);
    int transaction_append(char *fileName, TableInfo *tableInfo);

    int transaction_next_txnum();
} ;


#endif //DONGMENDB_TRANSACTION_H
