#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// 所有 Token 類型列舉
typedef enum {
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

// Token 結構定義
typedef struct {
    TokenType type;
    char value[64];
} Token;

// 原始碼指標
const char* src;
int pos = 0;

// 跳過空白
void skip_whitespace() {
    while (isspace(src[pos])) {
        pos++;
    }
}

// 識別字或關鍵字
Token make_identifier_or_keyword() {
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

// 整數
Token make_number() {
    Token token;
    token.type = TOKEN_NUMBER;
    int start = pos;
    while (isdigit(src[pos])) pos++;
    int len = pos - start;
    strncpy(token.value, src + start, len);
    token.value[len] = '\0';
    return token;
}

// 詞法分析主體
Token get_next_token() {
    skip_whitespace();
    Token token;
    char c = src[pos];

    if (c == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.value, "EOF");
        return token;
    }

    if (isalpha(c)) {
        return make_identifier_or_keyword();
    }

    if (isdigit(c)) {
        return make_number();
    }

    switch (c) {
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

// 主測試程式
int main() {
    src = "int x = 42 + 1 # this is comment";

    Token token;
    do {
        token = get_next_token();
        printf("Token: Type=%d, Value=%s\n", token.type, token.value);
    } while (token.type != TOKEN_EOF);

    return 0;
}
