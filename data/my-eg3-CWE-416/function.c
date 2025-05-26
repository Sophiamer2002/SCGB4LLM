#include <stdio.h>
#include <stdlib.h>

struct sock {
    int sk_refcnt;  // Reference count
    void (*sk_destruct)(struct sock *sk);  // Destruction callback
};

struct socket {
    struct sock *sk;  // Pointer to the associated sock structure
};

static int called = 0;

void sock_put(struct sock *sk) {
    called = 1;
    if (--sk->sk_refcnt == 0) {
        if (sk->sk_destruct) {
            sk->sk_destruct(sk);
        }
        free(sk);
    }
}

void release_sk(struct socket *sock) {
// BEGIN SOLUTION
    if (sock->sk) {
        sock_put(sock->sk);
        sock->sk = NULL;
    }
// END SOLUTION
}

int main() {
    struct socket *sock = malloc(sizeof(struct socket));
    sock->sk = malloc(sizeof(struct sock));
    sock->sk->sk_refcnt = 1;
    sock->sk->sk_destruct = NULL;

    release_sk(sock);

    if (called == 0) {
        return 1;
    }

    free(sock);
    return 0;
}
