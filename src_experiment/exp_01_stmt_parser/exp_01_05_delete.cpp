//
// Created by Sam on 2018/2/13.
//

#include <dongmensql/sqlstatement.h>
#include <parser/StatementParser.h>

/**
 * 在现有实现基础上，实现delete from子句
 *
 *  支持的delete语法：
 *
 *  DELETE <table_name> WHERE <logical_expr>
 *
 * 解析获得 sql_stmt_delete 结构
 */

sql_stmt_delete *DeleteParser::parse_sql_stmt_delete () {
    /** 匹配保留字 delete */
    if(!this->matchToken(TOKEN_RESERVED_WORD, "delete"))
        return nullptr;

    /** 解析表名 */
    Token *token = this->parseNextToken();

    char *tableName;
    SRA_t *table;
    if (token->type == TOKEN_WORD) {
        tableName = new_id_name();
        strcpy(tableName, token->text);

        TableReference_t *ref = TableReference_make(tableName, NULL);
        table =  SRATable(ref);
    } else {
        strcpy(this->parserMessage, "invalid sql: missing table name.");
        return NULL;
    }

    /** 匹配分号，SQL 中的 delete 语句可能不含 where 子句 */
    token = this->parseEatAndNextToken();
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
        sql_stmt_delete *sqlStmtDelete = (sql_stmt_delete *)calloc(sizeof(sql_stmt_delete), 1);
        sqlStmtDelete->tableName = tableName;
        sqlStmtDelete->where = table;

        return sqlStmtDelete;
    }

    /** 匹配保留字 where */
    token = this->parseNextToken();
    if(!this->matchToken(TOKEN_RESERVED_WORD, "where"))
        return nullptr;

    /** 解析 where 子句中的条件表达式 */
    token = this->parseNextToken();
    Expression *whereExpr = this->parseExpressionRD();
    if (this->parserStateType == PARSER_WRONG) {
        return NULL;
    }

    SRA_t *select = SRASelect(table, whereExpr);

    /** 打包解析结果 */
    sql_stmt_delete *sqlStmtDelete = (sql_stmt_delete *)calloc(sizeof(sql_stmt_delete), 1);
    sqlStmtDelete->tableName = tableName;
    sqlStmtDelete->where = select;

    return sqlStmtDelete;
};