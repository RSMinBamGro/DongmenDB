//
// Created by Sam on 2018/2/13.
//
#include <dongmensql/sqlstatement.h>
#include <parser/parser.h>
#include <utils/utils.h>
#include <parser/StatementParser.h>

/**
 * 在现有实现的基础上，修改parser以支持insert values多条的特性。
 */

/**
 * 支持的 insert 语法：
 *
 * insert into <tableName>(f1,f2,...,fn) values(v1,v2,...,vn);
 *
 */

sql_stmt_insert *InsertParser::parse_sql_stmt_insert() {
    char *tableName = NULL;
    vector<char*> fields ;
    vector<variant*> values ;

    /** 匹配保留字 insert */
    if (!this->matchToken( TOKEN_RESERVED_WORD, "insert")) {
        return NULL;
    }

    /** 匹配保留字 into */
    if (!this->matchToken( TOKEN_RESERVED_WORD, "into")) {
        strcpy(this->parserMessage, "invalid sql: should be into.");
        return NULL;
    }

    /** 解析表名 */
    Token *token = this->parseNextToken();
    if (token->type == TOKEN_WORD) {
        tableName = new_id_name();
        strcpy(tableName, token->text);
    } else {
        strcpy(this->parserMessage, "invalid sql: missing table name.");
        return NULL;
    }

    /** 匹配括号 ( */
    token = this->parseEatAndNextToken();
    if (!this->matchToken( TOKEN_OPEN_PAREN, "(")) {
        strcpy(this->parserMessage, "invalid sql: missing (.");
        return NULL;
    }

    /** 解析字段名列表 */
    token = this->parseNextToken();
    if (token->type == TOKEN_WORD) {
        while (token->type == TOKEN_WORD) {
            char *fieldName = new_id_name();
            strcpy(fieldName, token->text);
            fields.push_back(fieldName);
            token = this->parseEatAndNextToken();

            /** 匹配逗号 , */
            if (token->type==TOKEN_COMMA){
                token = this->parseEatAndNextToken();
            }else{
                break;
            }
        }
    } else {
        strcpy(this->parserMessage, "invalid sql: missing field name.");
        return NULL;
    }

    /** 匹配括号 ) */
    if (!this->matchToken( TOKEN_CLOSE_PAREN, ")")) {
        strcpy(this->parserMessage, "invalid sql: missing ).");
        return NULL;
    }


    /** 匹配保留字 values */
    if (!this->matchToken( TOKEN_RESERVED_WORD, "values")) {
        strcpy(this->parserMessage, "invalid sql: missing values.");
        return NULL;
    }

    /** 匹配括号 ( */
    if (!this->matchToken( TOKEN_OPEN_PAREN, "(")) {
        strcpy(this->parserMessage, "invalid sql: missing (.");
        return NULL;
    }

    /** 解析要插入的数据列表 */
    token = this->parseNextToken();
    if (token->type == TOKEN_STRING || token->type == TOKEN_DECIMAL) {
        while (token->type == TOKEN_STRING || token->type == TOKEN_DECIMAL) { // 当提取到 “）” 时退出循环
            if (token->type == TOKEN_STRING){
                variant *v = (variant*)malloc(sizeof(variant*));
                v->type == DATA_TYPE_CHAR;
                v->strValue=token->text;
                values.push_back(v);
            } else{
                variant *v = (variant*)malloc(sizeof(variant*));
                v->type == DATA_TYPE_INT;
                v->intValue = atoi(token->text);
                values.push_back(v);
            }

            token = this->parseEatAndNextToken();
            if (token->type==TOKEN_COMMA){
                token = this->parseEatAndNextToken();
            }else{
                break;
            }
        }
    } else {
        strcpy(this->parserMessage, "invalid sql: missing a value.");
        return NULL;
    }

    /** 匹配括号 ) */
    if (!this->matchToken( TOKEN_CLOSE_PAREN, ")")) {
        strcpy(this->parserMessage, "invalid sql: missing ).");
        return NULL;
    }

    sql_stmt_insert *sqlStmtInsert = (sql_stmt_insert *)calloc(sizeof(sql_stmt_insert),1);
    sqlStmtInsert->tableName = tableName;
    sqlStmtInsert->fields = fields;
    sqlStmtInsert->values = values;

    return sqlStmtInsert;
};

