#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum   // 所有類型 
{
    TOKEN_INT_DECL,
    TOKEN_IDENT,
    TOKEN_ASSIGN,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_EOF
} TokenType;

typedef struct // 結構定義
{
    TokenType type;
    char value[64];
} Token;

const char* src; // 指標
int pos = 0;

void skip_whitespace() // 跳過空白
{
    while (isspace(src[pos])) 
    {
        pos++;
    }
}

Token make_identifier_or_keyword() // 識別字或關鍵字
{
    Token token;
    int start = pos;
    while (isalnum(src[pos]) || src[pos] == '_') pos++;
    int len = pos - start;
    strncpy(token.value, src + start, len);
    token.value[len] = '\0';

    if (strcmp(token.value, "int") == 0) {
        token.type = TOKEN_INT_DECL;
    } else {
        token.type = TOKEN_IDENT;
    }
    return token;
}

Token make_number() // 整數
{
    Token token;
    token.type = TOKEN_NUMBER;
    int start = pos;
    while (isdigit(src[pos])) pos++;
    int len = pos - start;
    strncpy(token.value, src + start, len);
    token.value[len] = '\0';
    return token;
}

Token get_next_token() // 分析
{
    skip_whitespace();
    Token token;
    char c = src[pos];

    if (c == '\0') 
    {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }

    if (isalpha(c)) 
    {
        return make_identifier_or_keyword();
    }

    if (isdigit(c)) 
    {
        return make_number();
    }

    switch (c) 
    {
        case '=':
            token.type = TOKEN_ASSIGN;
            strcpy(token.value, "=");
            break;
        case '+':
            token.type = TOKEN_PLUS;
            strcpy(token.value, "+");
            break;
        case '-':
            token.type = TOKEN_MINUS;
            strcpy(token.value, "-");
            break;
        case '(':
            token.type = TOKEN_LPAREN;
            strcpy(token.value, "(");
            break;
        case ')':
            token.type = TOKEN_RPAREN;
            strcpy(token.value, ")");
            break;
        case '{':
            token.type = TOKEN_LBRACE;
            strcpy(token.value, "{");
            break;
        case '}':
            token.type = TOKEN_RBRACE;
            strcpy(token.value, "}");
            break;
        case '#':
            while (src[pos] != '\n' && src[pos] != '\0') pos++;
            return get_next_token(); // 遞迴呼叫，略過註解
        default:
            printf("未知符號: %c\n", c);
            exit(1);
    }

    pos++;
    return token;
}
