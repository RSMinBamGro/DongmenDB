//
// Created by Sam on 2018/2/13.
//

#include <dongmensql/sqlstatement.h>
#include <parser/StatementParser.h>

/**
 * 在现有实现基础上，实现update from子句
 *
 * 支持的update语法：
 *
 * UPDATE <table_name> SET <field1> = <expr1>[, <field2 = <expr2>, ..]
 * WHERE <logical_expr>
 *
 * 解析获得 sql_stmt_update 结构
 */

/*TODO: parse_sql_stmt_update， update语句解析*/
sql_stmt_update *UpdateParser::parse_sql_stmt_update() {
    // fprintf(stderr, "TODO: update is not implemented yet. in parse_sql_stmt_update \n");

    char *tableName = nullptr;
    vector<char*> fields; // 被更新的字段列表
    vector<Expression*> fieldsExpr;  // 新值(表达式)列表
    SRA_t *where;


    /** "update" */
    if (!this->matchToken(TOKEN_RESERVED_WORD, "update"))
        return nullptr;

    /** tableName */
    Token *token = this->parseNextToken();
    if (token->type == TOKEN_WORD) { // 判断是否为非保留字
        tableName = new_id_name();
        strcpy(tableName, token->text);
    } else { // 如果是保留字，则该表名不合法
        strcpy(this->parserMessage, "invalid sql: missing table name.");
        return NULL;
    }

    /** "set" */
    token = this->parseEatAndNextToken();
    if (!this->matchToken(TOKEN_RESERVED_WORD, "set"))
        return nullptr;

    /** columnName=expr */

    /** "where" */
    if(!this->matchToken(TOKEN_RESERVED_WORD, "where"))
        return nullptr;

    /** columnName=expr */
};