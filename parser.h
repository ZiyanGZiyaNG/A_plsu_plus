typedef enum // 節點
{
    AST_INT, // 整數
    AST_IDENT, // 名稱
    AST_DINARY_EXPR,  // 二元運算
    AST_VAR_DECL // 宣告
} ASTNodeType;
