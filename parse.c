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
    Token *t;
    for (;;) {
        Node *stmt = assign();
        t = tokens->data[pos];
        if (t->ty != ';') {
            error("コードの末尾に;がありません。");
        }
        code[i] = stmt;

        pos++;
        t = tokens->data[pos];
        if (t->ty == TK_EOF) {
            break;
        }
        i++;
    }
}

Node *assign() {
    Node *lhs = equality();
    Token *t = tokens->data[pos];
    if (t->ty == '=') {
        pos++;
        return new_node('=', lhs, equality());
    }
    return lhs;
}

Node *equality() {
    Node *lhs = expr();
    Token *t;

    t = tokens->data[pos];
    if (t->ty == TK_EQ) {
        pos++;
        return new_node(ND_EQ, lhs, expr());
    }
    if (t->ty == TK_NE) {
        pos++;
        return new_node(ND_NE, lhs, expr());
    }
    return lhs;
}

Node *expr() {
    Node *lhs = mul();
    Token *t;

    t = tokens->data[pos];
    if (t->ty == '+') {
        pos++;
        return new_node('+', lhs, expr());
    }
    t = tokens->data[pos];
    if (t->ty == '-') {
        pos++;
        return new_node('-', lhs, expr());
    }
    return lhs;
}

Node *mul() {
    Node *lhs = term();
    Token *t;

    t = tokens->data[pos];
    if (t->ty == '*') {
        pos++;
        return new_node('*', lhs, mul());
    }
    if (t->ty == '/') {
        pos++;
        return new_node('/', lhs, mul());
    }
    return lhs;
}

Node *term() {
    Token *t;

    t = tokens->data[pos];
    if (t->ty == TK_NUM) {
        pos++;
        return new_node_num(t->val);
    }
    if (t->ty == TK_IDENT) {
        pos++;
        return new_node_ident(t->name);
    }
    if (t->ty != '(') {
        error("数値でも開きカッコでもないトークンです: %s",
              t->input);
    }

    pos++;
    Node *node = expr();
    t = tokens->data[pos];
    if (t->ty != ')') {
        error("開きカッコに対応する閉じカッコがありません: %s",
              t->input);
    }
    pos++;
    return node;
}

void parse() {
    program();
}
