#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "9cc.h"

Node *code[100];

void program();
Node *assign();
Node *equality();
Node *expr();
Node *mul();
Node *term();

Node *new_node(int op, Node *lhs, Node *rhs) {
    Node *node = malloc(sizeof(Node));
    node->ty = op;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = malloc(sizeof(Node));
    node->ty = ND_NUM;
    node->val = val;
    return node;
}

Node *new_node_ident(char name) {
    Node *node = malloc(sizeof(Node));
    node->ty = ND_IDENT;
    node->name = name;
    return node;
}

void program() {
    int i = 0;
    for (;;) {
        Node *stmt = assign();
        if (tokens[pos].ty != ';') {
            error("コードの末尾に;がありません。");
        }
        code[i] = stmt;

        pos++;
        if (tokens[pos].ty == TK_EOF) {
            break;
        }
        i++;
    }
}

Node *assign() {
    Node *lhs = equality();
    if (tokens[pos].ty == '=') {
        pos++;
        return new_node('=', lhs, equality());
    }
    return lhs;
}

Node *equality() {
    Node *lhs = expr();
    if (tokens[pos].ty == TK_EQ) {
        pos++;
        return new_node(ND_EQ, lhs, expr());
    }
    if (tokens[pos].ty == TK_NE) {
        pos++;
        return new_node(ND_NE, lhs, expr());
    }
    return lhs;
}

Node *expr() {
    Node *lhs = mul();
    if (tokens[pos].ty == '+') {
        pos++;
        return new_node('+', lhs, expr());
    }
    if (tokens[pos].ty == '-') {
        pos++;
        return new_node('-', lhs, expr());
    }
    return lhs;
}

Node *mul() {
    Node *lhs = term();
    if (tokens[pos].ty == '*') {
        pos++;
        return new_node('*', lhs, mul());
    }
    if (tokens[pos].ty == '/') {
        pos++;
        return new_node('/', lhs, mul());
    }
    return lhs;
}

Node *term() {
    if (tokens[pos].ty == TK_NUM) {
        return new_node_num(tokens[pos++].val);
    }
    if (tokens[pos].ty == TK_IDENT) {
        return new_node_ident(tokens[pos++].name);
    }
    if (tokens[pos].ty != '(') {
        error("数値でも開きカッコでもないトークンです: %s",
              tokens[pos].input);
    }

    pos++;
    Node *node = expr();
    if (tokens[pos].ty != ')') {
        error("開きカッコに対応する閉じカッコがありません: %s",
              tokens[pos].input);
    }
    pos++;
    return node;
}

void parse() {
    program();
}
