//
// Created by sam on 2018/9/18.
//
#include <physicalplan/ExecutionPlan.h>
#include <physicalplan/TableScan.h>
#include <physicalplan/Select.h>
#include <physicalplan/Project.h>
#include <physicalplan/Join.h>

/*执行delete语句的物理计划，返回删除的记录条数
 * 返回大于等于0的值，表示删除的记录条数；
 * 返回小于0的值，表示删除过程中出现错误。
 * */

int ExecutionPlan::executeDelete(DongmenDB *db, sql_stmt_delete *sqlStmtDelete, Transaction *tx){
    /** 删除语句以select的物理操作为基础实现。
     * 1. 使用 sql_stmt_delete 的条件参数，调用 physical_scan_select_create 创建select的物理计划并初始化;
     * 2. 执行 select 的物理计划，完成 delete 操作
     * */
    /** 创建物理计划，该计划相当于数据表的一个结果集 */
    Scan* scan = generateScan(db, sqlStmtDelete->where, tx);
    //Select* select = new Select(scan);

    size_t count_deleted = 0;

    /** 对计划进行遍历 */
    scan->beforeFirst(); ///遍历指针指向计划首部
    while (scan->next()) {

        scan->deleteRecord();

        count_deleted ++;
    }

    // select->close();
    scan->close();

    // fprintf(stdout, "%d", count_updated);
    return count_deleted;
};