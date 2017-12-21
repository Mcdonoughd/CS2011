/************************************************************************
 * Dr. Evil's Insidious Bomb, Version 1.4
 * Copyright 2017, Dr. Evil Incorporated. All rights reserved.
 *
 * LICENSE:
 *
 * Dr. Evil Incorporated (the PERPETRATOR) hereby grants you (the
 * VICTIM) explicit permission to use this bomb (the BOMB).  This is a
 * time limited license, which expires on the death of the VICTIM.
 * The PERPETRATOR takes no responsibility for damage, frustration,
 * insanity, bug-eyes, carpal-tunnel syndrome, loss of sleep, or other
 * harm to the VICTIM.  Unless the PERPETRATOR wants to take credit,
 * that is.  The VICTIM may not distribute this bomb source code to
 * any enemies of the PERPETRATOR.  No VICTIM may debug,
 * reverse-engineer, statically auto-solve, decompile, decrypt, or use
 * any other technique to gain knowledge of and defuse the BOMB.  BOMB
 * proof clothing may not be worn when handling this program.  The
 * PERPETRATOR will not apologize for the PERPETRATOR's poor sense of
 * humor.  This license is null and void where the BOMB is prohibited
 * by law.
 *************************************************************************/


#include <stdio.h>
#include <stdlib.h>

FILE *infile;
char *read_line();
void phase_defused();

int main(int argc, char **argv) {
    /* If you can see this, it means I forgot to turn off debugging symbols. */
    char *input;

    if (argc == 1)
        infile = stdin;
    else if (argc == 2) {
        if (!(infile = fopen(argv[1], "r"))) {
            fprintf(stderr, "%s: error: couldn't open %s\n", argv[0], argv[1]);
            return 8;
        }
    }
    else {
        /* You can't call the bomb with more than one parameter. */
        fprintf(stderr, "Usage: %s [input file]\n", argv[0]);
        return 8;
    }

    initialize_bomb(argc, argv);

    puts("Welcome to my nasty little bomb. If you want to defuse it, you");
    printf("must enter the right password for each of the %d phases. Otherwise,\n", NUMPHASES);
    puts("the bomb will blow up! Enjoy!");


printf("%d: ", 1); fflush(stdout);
input = read_line(); /* Get some input */
phase1(input); /* Blow up the bomb */
phase_defused(); /* He'll never get here! */

printf("%d: ", 2); fflush(stdout);
input = read_line();
phase2(input);
phase_defused();

printf("%d: ", 3); fflush(stdout);
input = read_line();
phase3(input);
phase_defused();

printf("%d: ", 4); fflush(stdout);
input = read_line();
phase4(input);
phase_defused();

printf("%d: ", 5); fflush(stdout);
input = read_line();
phase5(input);
phase_defused();

printf("%d: ", 6); fflush(stdout);
input = read_line();
phase6(input);
phase_defused();


    return 0;
}

/* I'm an evil doctor and I don't need no standard library! */
int string_length(char *aString) {
    int length;
    char *ptr;

    ptr = aString;
    length = 0;

    while (*ptr != 0) {
        ptr++;
        length = length + 1;
    }
    return length;
}

int strings_not_equal(char *string1, char *string2) {
    char *p, *q;

    if (string_length(string1) != string_length(string2))
    return 1;

    p = string1;
    q = string2;

    while (*p != 0) {
        if (*p != *q)
            return 1;
        p++;
        q++;
    }
    return 0;
}
