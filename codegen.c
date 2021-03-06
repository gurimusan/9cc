#include "9cc.h"

void gen_lval(Node *node) {
    if (node->ty == ND_IDENT) {
        printf("  mov rax, rbp\n");
        printf("  sub rax, %d\n",
               ('z' - node->name + 1) * 8);
        printf("  push rax\n");
        return;
    }
    error("代入の左辺値が変数ではありません");
}

void gen(Node *node) {
    if (node->ty == ND_NUM) {
        printf("  push %d\n", node->val);
        return;
    }

    if (node->ty == ND_IDENT) {
        gen_lval(node);
        printf("  pop rax\n");
        printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        return;
    }

    if (node->ty == ND_EQ) {
        gen(node->lhs);
        gen(node->rhs);
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cmp rdi, rax\n");
        printf("  sete al\n");
        printf("  movzb rax, al\n");
        printf("  push rax\n");
        return;
    }

    if (node->ty == ND_NE) {
        gen(node->lhs);
        gen(node->rhs);
        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  cmp rdi, rax\n");
        printf("  setne al\n");
        printf("  movzb rax, al\n");
        printf("  push rax\n");
        return;
    }

    if (node->ty == '=') {
        gen_lval(node->lhs);
        gen(node->rhs);

        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  mov [rax], rdi\n");
        printf("  push rdi\n");
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->ty) {
        case '+':
            printf("  add rax, rdi\n");
            break;
        case '-':
            printf("  sub rax, rdi\n");
            break;
        case '*':
            // RDXに一旦値が入るので0クリアする
            printf("  mov rdx, 0\n");
            printf("  mul rdi\n");
            break;
        case '/':
            // RDXに一旦値が入るので0クリアする
            printf("  mov rdx, 0\n");
            printf("  div rdi\n");
            break;
    }

    printf("  push rax\n");
}
