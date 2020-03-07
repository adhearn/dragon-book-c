// Be lazy and incldue all of my commonly used headers
#include "allheads.h"

char *curChar;
GHashTable *words;

// Have to use TAG_TRUE and TAG_FALSE to avoid collisions with glib TRUE and FALSE
enum tags {NUM = 256, ID, TAG_TRUE, TAG_FALSE};

typedef struct NumAttributes {
    int value;
} NumAttributes;

typedef struct WordAttributes {
    char *lexeme;
} WordAttributes;

typedef struct Token {
    int tag;
    union {
        struct NumAttributes num;
        struct WordAttributes word;
    };
} Token;

/* NumAttributes *makeNumAttributes(int value) { */
/*     NumAttributes *attrs = malloc(sizeof(NumAttributes)); */
/*     attrs->value = value; */
/*     return attrs; */
/* } */

/* WordAttributes *makeWordAttributes(char *lexeme) { */
/*     WordAttributes *attrs = malloc(sizeof(WordAttributes)); */
/*     attrs->lexeme = lexeme; */
/*     return attrs; */
/* } */

Token *makeToken(int tag) {
    Token *token = malloc(sizeof(Token));
    token->tag = tag;
    return token;
}

Token *makeNum(int value) {
    NumAttributes attrs = { .value =value };
    Token *token = makeToken(NUM);
    token->num = attrs;
    return token;
}

Token *makeWord(char *lexeme) {
    Token *token = g_hash_table_lookup(words, lexeme);
    if (!token) {
        WordAttributes attrs = { .lexeme = lexeme };
        token = makeToken(ID);
        token->word = attrs;
    }

    return token;
}

void freeToken(Token *token) {
    if (token->tag == ID) {
        free(token->word.lexeme);
    }
    free(token);
}

void printToken(Token *token) {
    printf("<%d", token->tag);
    switch (token->tag) {
    case (NUM):
        printf(" %d", token->num.value);
        break;
    case (ID):
        printf(" %s", token->word.lexeme);
        break;
    case ('+'):
    case ('*'):
    case ('-'):
        printf(" %c", token->tag);
    }

    printf(">\n");
}

void advance() {
    curChar++;
}

Token *tokenizeNum() {
    int value = 0;
    do {
        value = (value * 10) + (*curChar - '0');
        advance();
    } while (isdigit(*curChar));

    return makeNum(value);
}

Token *tokenizeWord() {
    char *start = curChar;
    int length = 0;
    do {
        length++;
        advance();
    } while (isalnum(*curChar)); // isalnum is a bit restrictive, but works for this simple lexer

    char *lexeme = g_strndup(start, length);
    return makeWord(lexeme);
}

Token *nextToken() {
    if (*curChar) {
        if (isspace(*curChar)) {
            advance();
            return nextToken();
        } else if (isdigit(*curChar)) {
            return tokenizeNum();
        } else if (isalpha(*curChar)) {
            return tokenizeWord();
        } else if (*curChar == '/') {
            if (*(curChar + 1) == '/') {
                // Read to the end of the line
                do {
                    advance();
                } while ((*curChar != '\n') && (*curChar != 0));
            } else if (*(curChar + 1) == '*') {
                do {
                    advance();
                } while ((*(curChar + 1) != 0) && !((*curChar == '*') && (*(curChar + 1) == '/')));
                advance();
                advance();
            }
            return nextToken();
        } else  {
            Token *t = makeToken(*curChar);
            advance();
            return t;
        }
    } else {
         return 0;
    }
}

void reserveWord(Token *token) {
    g_hash_table_insert(words, token->word.lexeme, token);
}

void initializeKeywords() {
    char *trueStr, *falseStr;
    asprintf(&trueStr, "true");
    asprintf(&falseStr, "false");
    Token *trueToken = makeWord(trueStr);
    Token *falseToken = makeWord(falseStr);
    trueToken->tag = TAG_TRUE;
    falseToken->tag = TAG_FALSE;
    reserveWord(trueToken);
    reserveWord(falseToken);
}

int main(int argc, char *argv[]) {
    curChar = "4 - 3 //+ 2\n /*+ 5\n-2*/ *\n true";

    words = g_hash_table_new(g_str_hash, g_str_equal);
    initializeKeywords();

    Token *t;
    while ((t = nextToken())) {
        printToken(t);
    }
    printf("Done!\n");
}
