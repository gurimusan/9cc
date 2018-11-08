#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// トークンの型を表す値
enum {
    TK_NUM = 256,   // 整数トークン
    TK_IDENT,       // 識別子
    TK_EOF,         // 入力の終わりを表すトークン
};

typedef struct {
    int ty;         // トークンの型
    int val;        // tyがTK_NUMの場合、その数値
    char *input;    // トークン文字列（エラーメッセージ用
    char name;     // tyがTK_IDENTの場合のみ使う
} Token;

// トークナイズした結果のトークン列はこの配列に保存する
// 100個以上のトークンは来ないものとする
extern Token tokens[100];

extern int pos;

enum {
    ND_NUM = 256,   // 整数のノード型
    ND_IDENT,       // 識別子のノードの型
};

typedef struct Node {
    int ty;             // 演算子かND_NUM
    struct Node *lhs;   // 左辺
    struct Node *rhs;   // 右辺
    int val;            // tyがND_NUMの場合のみ使う
    char name;          // tyがND_IDENTの場合のみ使う
} Node;

void parse();

extern Node *code[100];

void gen(Node *node);

void error(char *fmt, ...);

typedef struct {
    void **data;
    int capacity;
    int len;
} Vector;

Vector *new_vector();

void vec_push(Vector *vec, void *elem);

typedef struct {
    Vector *keys;
    Vector *vals;
} Map;

Map *new_map();
void map_put(Map *map, char *key, void *val);
void *map_get(Map *map, char *key);

void runtest();