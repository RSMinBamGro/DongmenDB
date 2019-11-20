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

    /** 匹配保留字 update */
    if (!this->matchToken(TOKEN_RESERVED_WORD, "update"))
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


    /** 匹配保留字 set */
    token = this->parseEatAndNextToken();
    if (!this->matchToken(TOKEN_RESERVED_WORD, "set"))
        return nullptr;

    /** 解析更新表达式 field=expr */
    token = this->parseNextToken();

    vector<char*> fields; // 被更新的字段列表
    vector<Expression*> fieldsExpr;  // 新值(表达式)列表
    if (token->type == TOKEN_WORD) {
        while (token->type == TOKEN_WORD) {
            token = this->parseNextToken();
            char *fieldName = new_id_name();
            strcpy(fieldName, token->text);

            fields.push_back(fieldName);

            token = this->parseEatAndNextToken();
            if(token->type == TOKEN_EQ)
                token = this->parseEatAndNextToken();


            Expression *fieldExpr = this->parseExpressionRD();
            if (this->parserStateType == PARSER_WRONG) {
                return NULL;
            }

            fieldsExpr.push_back(fieldExpr);

            token = this->parseNextToken();
            if (token != NULL && token->type == TOKEN_COMMA) {
                token = this->parseEatAndNextToken();
            } else break;

        }
    } else {
        strcpy(this->parserMessage, "invalid sql: missing field name.");
        return NULL;
    }


    /** 匹配分号 */
    token = this->parseNextToken();
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
        sql_stmt_update *sqlStmtUpdate = (sql_stmt_update *)calloc(sizeof(sql_stmt_update), 1);
        sqlStmtUpdate->tableName = tableName;
        sqlStmtUpdate->fields = fields;
        sqlStmtUpdate->fieldsExpr = fieldsExpr;
        sqlStmtUpdate->where = table;


        return sqlStmtUpdate;
    }


    /** 匹配保留字 where */
    if(!this->matchToken(TOKEN_RESERVED_WORD, "where"))
        return nullptr;

    /** 解析 where 子句中的条件表达式 */
    token = this->parseNextToken();
    Expression *whereExpr = this->parseExpressionRD();
    if (this->parserStateType == PARSER_WRONG) {
        return NULL;
    }


    SRA_t *select = SRASelect(table, whereExpr);

    sql_stmt_update *sqlStmtUpdate = (sql_stmt_update *)calloc(sizeof(sql_stmt_update),1);
    sqlStmtUpdate->tableName = tableName;
    sqlStmtUpdate->fields = fields;
    sqlStmtUpdate->fieldsExpr = fieldsExpr;
    sqlStmtUpdate->where = select;


    token = this->parseNextToken();
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
        // fprintf(stdout, "succeeded");
        return sqlStmtUpdate;
    } else {
        return NULL;
    }
};
