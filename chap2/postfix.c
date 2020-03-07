/**
 * A simple infix-to-postfix compiler. The lexer and parser are so simple that
 * we don't define them separately.
 *
 * The alphabet:
 * [0-9]+-
 *
 * The grammar and translation scheme:
 * expr --> expr1 + term  ## epxr1 term print('+')
 * expr --> expr1 - term  ## epxr1 term print('-')
 * expr --> term          ## term
 * term -> 0              ## print('0')
 * term -> 1              ## print('1')
 * ...
 * term -> 9              ## print('9')
 *
 * Unfortunately, this grammar is left-recursive, so a naive predictive parser
 * won't cut it. Instead, we'll use the transformation from the book,
 * where if you have a grammar like:
 * A -> Aa | b
 * you can convert it using a new intermediate nonterminal, R:
 * A -> bR
 * R -> aR | e
 * (where e should be epsilon, the empty string).
 * Using that transformation, we have:
 *
 * expr -> term R1         || term R1
 * R -> + term R           || term { print('+') } R
      | - term R           || term { print('-') } R
      | e                  || // do nothing
 * term -> ...             || print { '0' } // or '1', '2', etc.
 *
 * For a first pass, we'll just print the postfix string, which allows us to
 * use very simple semantic rules while parsing. In a later step, we'll
 * construct a full AST, then walk the tree to produce an output string.
 */

// Be lazy and incldue all of my commonly used headers
#include "allheads.h"

// Keep track of where we are in the inmput string
char *curChar;

// Because all of our terminals are single characters, we can cheat and use a
// single char for our lookahead.
char lookahead;

// Improve our error messages by tracking the current position in the input string
int curPos;

void advanceLookahead() {
    curChar++;
    lookahead = *curChar;
    curPos++;
}

void term() {
    if ('0' <= lookahead && '9' >= lookahead) {
        printf("%c", lookahead);
        advanceLookahead();
    } else {
        fprintf(stderr, "Illegal character encountered in input string at location %d: %c\n", curPos, lookahead);
        abort();
    }
}

void r() {
    // We could condense these into a single case by storing off the current
    // lookahead so we know what to print. This is nice and explicit, though.
    while (1) {
        char op = lookahead;
        if (op == '+' || op == '-') {
            advanceLookahead();
            term();
            printf("%c", op);
        }
        else {
            break;
        }
    }

    // Could be the empty string, return without signaling an error and let the
    // next step of the parser decide.
}

void expr() {
    term(); r();
}

/**
 * Simple wrapper for our parser. Initialies our variables, then enters
 * at the start symbol.
 */
void parse(char **input) {
    curChar = *input;
    lookahead = *curChar;
    expr();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input string>\n", argv[0]);
        return 1;
    }
    char *input = argv[1];
    parse(&input);
    printf("\n");
    if (*curChar) {
        // We haven't reached the end of the string, means our parse was unsuccessful
        fprintf(stderr, "Failed to parse entire string, stopped at location %d. Remaining string: %s\n", curPos, curChar);
        return 1;
    }
}
