#include <stdio.h>
#include <string.h>

typedef enum {
    START,
    AFTER_MINUS,
    AFTER_DOT,
    VALID_END_STATES,
    SECOND_DIGIT_ONWARDS,
    MANTISSA,
    LAST_STATE
} FSM_STATES;

typedef struct {
    FSM_STATES state;
    char *tokens;
    FSM_STATES next_state;
} FSM_STATE_INFO;

int run(FSM_STATES start_state, FSM_STATE_INFO *state_machine, char *cargo) {
    FSM_STATE_INFO *p = state_machine;
    FSM_STATES current_state = start_state;
    FSM_STATE_INFO *found;
    char *c = cargo;

    while (*c != 0) {
        found = NULL;
        p = state_machine;
        while (p->state != LAST_STATE) {
            if (p->state == current_state) {
                if (strchr(p->tokens, *c) != NULL) {
                    found = p;
                    break;
                }
            }
            p++;
        }
    
        if (found != NULL) {
            current_state = p->next_state;
            c++;
        }
        else {
            printf("%s is faulty. Failed at: %c\n", cargo, *c);
            return -1;
        }
    }

    if (current_state > VALID_END_STATES) {
        printf("%s is good.\n", cargo);
        return 1;
    }
    else {
        printf("%s is faulty. Incomplete input.\n", cargo);
        return -2;
    }
}

int main(int argc, char *argv[]) {
    static FSM_STATE_INFO state_machine[] = {
        {START, "1234567890", SECOND_DIGIT_ONWARDS},
        {START, "-", AFTER_MINUS},
        {AFTER_MINUS, "1234567890", SECOND_DIGIT_ONWARDS},
        {SECOND_DIGIT_ONWARDS, "1234567890", SECOND_DIGIT_ONWARDS},
        {SECOND_DIGIT_ONWARDS, ".", AFTER_DOT},
        {AFTER_DOT, "1234567890", MANTISSA},
        {MANTISSA, "1234567890", MANTISSA},
        {LAST_STATE, "", LAST_STATE}
    };

    char input;
    printf("Enter a number: ");
    scanf("%s", &input); 

    run(START, state_machine, &input);
}
