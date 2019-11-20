//
// Created by Sam on 2018/2/13.
//



#include <relationalalgebra/sra.h>
#include <dongmensql/sqlstatement.h>
#include <parser/StatementParser.h>

/**
 * @brief 解析select语句，对select语句进行语法分析,并将select语句转换为关系代数表达式
 * @param parser 词法解析器
 * @return select语句
 */

/**
 * 支持的 select 语法：
 *
 * select * from <tableName>
 * select f1, f2, ... , fn from <tableName>
 *
 */

SRA_t * SelectParser::parse_sql_stmt_select() {
    Token *token = this->parseNextToken();
    /** 匹配保留字 select */
    if(!this->matchToken(TOKEN_RESERVED_WORD, "select")){
        strcpy(this->parserMessage, "语法错误.");
        return NULL;
    }

    /** 解析 select 子句中的表达式列表 */
    vector<Expression*> fieldsExpr = parseFieldsExpr();
    if (this->parserStateType == PARSER_WRONG) {
        return NULL;
    }

    /** 匹配保留字 from */
    token = this->parseNextToken();
    if(!this->matchToken(TOKEN_RESERVED_WORD, "from")){
        strcpy(this->parserMessage, "语法错误.");
        return NULL;
    }

    /** 解析 from 子句中的数据表列表 */
    token = this->parseNextToken();
    SRA_t *tablesExpr = parseTablesExpr();
    if (this->parserStateType == PARSER_WRONG) {
        return NULL;
    }

    /** 匹配分号 “;” ，若成功，表示 select 语句结束，则将 select 语句转化为关系代数表达式并返回 */
    token = this->parseNextToken();
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
        SRA_t *project = SRAProject(tablesExpr, fieldsExpr);
        return project;
    }

    /** 若未结束，则继续匹配 where 子句 */
    token = this->parseNextToken();
    if(!this->matchToken(TOKEN_RESERVED_WORD, "where")){
        strcpy(this->parserMessage, "语法错误.");
        return NULL;
    }

    /** 解析 where 子句中的条件表达式 */
    token = this->parseNextToken();
    Expression *whereExpr = this->parseExpressionRD();
    if (this->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    SRA_t *select = SRASelect(tablesExpr, whereExpr);
    SRA_t *project = SRAProject(select, fieldsExpr);
    token = this->parseNextToken();

    /** 匹配分号 ; ，若成功，表示 select 语句结束，则构造关系代数表达式并返回*/
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
        return project;
    }
    token = this->parseNextToken();

    /** 如果未结束，则尝试匹配 group 子句 */
    if(!this->matchToken(TOKEN_RESERVED_WORD, "group")){
        strcpy(this->parserMessage, "语法错误.");
        return NULL;
    }
    token = this->parseNextToken();
    if(!this->matchToken(TOKEN_RESERVED_WORD, "by")){
        strcpy(this->parserMessage, "语法错误.");
        return NULL;
    }
    token = this->parseNextToken();
    vector<Expression*> groupExpr = parseFieldsExpr();
    project->project.group_by.assign( groupExpr.begin(), groupExpr.end());
    project->project.group_by.assign( groupExpr.begin(), groupExpr.end());
    if (this->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    token = this->parseNextToken();
    if (token == NULL) {
        return project;
    }
    token = this->parseNextToken();

    /** 如果未结束，则尝试匹配 order 子句 */
    if(!this->matchToken(TOKEN_RESERVED_WORD, "order")){
        strcpy(this->parserMessage, "语法错误.");
        return NULL;
    }
    token = this->parseNextToken();
    if(!this->matchToken(TOKEN_RESERVED_WORD, "by")){
        strcpy(this->parserMessage, "语法错误.");
        return NULL;
    }
    token = this->parseNextToken();
    vector<Expression*> orderExpr = parseOrderExpr(this);
    project->project.order_by.assign(orderExpr.begin(), orderExpr.end());
    if (this->parserStateType == PARSER_WRONG) {
        return NULL;
    }
    token = this->parseNextToken();

    /** select 语句结束,返回构造的关系代数表达式 */
    if (token == NULL || token->type == TOKEN_SEMICOLON) {
        return project;
    } else {
        return NULL;
    }

};

/**
 * 解析select后面的表达式列表，以arraylist返回
 * @param parser 语法分析器
 * @return 表达式列表
 */
vector<Expression*> SelectParser::parseFieldsExpr() {
    vector<Expression*> exprs;

    /** 解析 from 子句中第一个表达式 */
    Expression *expr0 = this->parseExpressionRD();
    exprs.push_back(expr0);

    Token *token = this->parseNextToken();

    /** 匹配逗号 , ，若成功，表示还有其他表达式则进入循环，否则返回 */
    while (token != NULL && token->type == TOKEN_COMMA) {
        this->parseEatAndNextToken();
        Expression *expr1 = this->parseExpressionRD();
        exprs.push_back(expr1);
        token = this->parseNextToken();
    }

    return exprs;
};

/**
 * 解析from子句的数据表，并转换为笛卡尔积构成的关系代数表达式。
 * 比如from student 转换为 SRATable(student)
 * from student, sc 转换为 SRAJoin(SRATable(sc), SRATable(student))
 * from studnet, sc, course 转换为 SRAJoin(SRATable(course), SRAJoin(SRATable(student), SRATable(sc)))*
 *
 */
SRA_t *SelectParser::parseTablesExpr() {
    Token *token = this->parseNextToken();
    if (token->type == TOKEN_WORD) {

        /** 解析 from 子句第一个数据表 */
        char *tableName = strdup(token->text);
        TableReference_t *ref = TableReference_make(tableName, NULL);
        SRA_t *table =  SRATable(ref);

        Token *token = this->parseEatAndNextToken();
        /** 匹配逗号，若成功，表示还有其他数据表，则进入循环继续解析，否则返回 */
        while (token != NULL && token->type == TOKEN_COMMA) {
            token = this->parseEatAndNextToken();/*跳过comma*/
            char *tableName = strdup(token->text);
            TableReference_t *ref1 =   TableReference_make(tableName, NULL);
            SRA_t *table1 =  SRATable(ref1);

            /** 将当前解析的数据表与上一个关系代数表达式通过笛卡尔积操作结合 */
            table = SRAJoin(table, table1, NULL);
            token = this->parseEatAndNextToken();
        }

        return table;
    }

    return NULL;
};
