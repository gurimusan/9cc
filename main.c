#include "9cc.h"

int pos = 0;
Vector *tokens;

// pが指している文字列をトークンに分割してtokensに保存する
void tokenize(char *p) {
    tokens = new_vector();

    while (*p) {
        // 空白文字をスキップ
        if (isspace(*p)) {
            p++;
            continue;
        }

        // ==の場合
        if (strncmp(p, "==", 2) == 0) {
            Token *t = calloc(1, sizeof(Token));
            t->ty = TK_EQ;
            t->input = p;
            vec_push(tokens, t);
            p += 2;
            continue;
        }

        // !=の場合
        if (strncmp(p, "!=", 2) == 0) {
            Token *t = calloc(1, sizeof(Token));
            t->ty = TK_NE;
            t->input = p;
            vec_push(tokens, t);
            p += 2;
            continue;
        }

        if (strchr("+-*/;=()", *p)) {
            Token *t = calloc(1, sizeof(Token));
            t->ty = *p;
            t->input = p;
            vec_push(tokens, t);
            p++;
            continue;
        }

        if ('a' <= *p && *p <= 'z') {
            Token *t = calloc(1, sizeof(Token));
            t->ty = TK_IDENT;
            t->input = p;
            t->name = *p;
            vec_push(tokens, t);
            p++;
            continue;
        }

        if (isdigit(*p)) {
            Token *t = calloc(1, sizeof(Token));
            t->ty = TK_NUM;
            t->input = p;
            t->val = strtol(p, &p, 10);
            vec_push(tokens, t);
            continue;
        }

        fprintf(stderr, "トークナイズできません: %s\n", p);
        exit(1);
    }

    Token *t = calloc(1, sizeof(Token));
    t->ty = TK_EOF;
    t->input = p;
    vec_push(tokens, t);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません。\n");
        return 1;
    }

    if (strcmp(argv[1], "-test") == 0) {
        runtest();
        return 0;
    }

    tokenize(argv[1]);
    parse();

    // アセンブリの前半部分を出力
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    // プロローグ
    // 変数26個分の領域を確保する
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, 208\n");

    // 先頭の式から順にコード生成
    for (int i=0; code[i]; i++) {
        gen(code[i]);
        printf("  pop rax\n");
    }

    // エピローグ
    // 最後の式の結果がRAXに残っているのでそれが返り値になる
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return 0;
}
