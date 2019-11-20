//
// Created by sam on 2018/9/18.
//

#include <physicalplan/ExecutionPlan.h>
#include <physicalplan/TableScan.h>
#include <physicalplan/Select.h>
#include <physicalplan/Project.h>
#include <physicalplan/Join.h>

/** 执行 update 语句的物理计划，返回修改的记录条数
 * 返回大于等于 0 的值，表示修改的记录条数；
 * 返回小于 0 的值，表示修改过程中出现错误。
 * */
/*TODO: plan_execute_update， update语句执行*/


int ExecutionPlan::executeUpdate(DongmenDB *db, sql_stmt_update *sqlStmtUpdate, Transaction *tx) {
    /** 更新语句以 select 的物理操作为基础实现。
     * 1. 使用 sql_stmt_update 的条件参数，调用 physical_scan_select_create 创建 select 的物理计划并初始化;
     * 2. 执行 select 的物理计划，完成update操作
     * */

    /** 创建物理计划，该计划相当于数据表的一个结果集 */
    Scan* scan = generateScan(db, sqlStmtUpdate->where, tx);
    //Select* select = new Select(scan);

    size_t count_updated = 0;

    /** 对计划进行遍历 */
    scan->beforeFirst(); ///遍历指针指向计划首部
    while (scan->next()) {

        /** 针对每个字段进行处理 */
        for (size_t i = 0; i < sqlStmtUpdate->fields.size(); i ++) {
            char* fieldName = sqlStmtUpdate->fields.at(i);

            variant *var = (variant*)calloc(sizeof(variant), 1); /// 用以保存新值表达式的值
            enum data_type field_type = scan->getField(sqlStmtUpdate->tableName, fieldName)->type;

            scan->evaluateExpression(sqlStmtUpdate->fieldsExpr.at(i), scan, var);/// 计算新值表达式的值并存入 var 中

            /** 判断数据类型是否匹配 */
            if(var->type != field_type) { /// 若不匹配则报错
                fprintf(stdout, "error");
                return DONGMENDB_EINVALIDSQL;
            }

            if(var->type == DATA_TYPE_INT) { /// 若值为整型
                scan->setInt(sqlStmtUpdate->tableName, fieldName, var->intValue);
            } else if (var->type == DATA_TYPE_CHAR) { /// 若值为字符串型
                scan->setString(sqlStmtUpdate->tableName, fieldName, var->strValue);

                /** 若字符串值的长度超过定义长度，需要进行截取 */
            }
        }


        count_updated ++;
    }

    // select->close();
    scan->close();

    // fprintf(stdout, "%d", count_updated);
    return count_updated;
};