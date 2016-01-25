/***************************************************************
 * rounds.c
 *
 * Author: Akeel Ali
 * Date: Dec. 2015
 *
 * See USAGE definition below along with the exercise PDF for a 
 * description of what this program does and how to use it.
 ***************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <getopt.h>
#include "deck.h"
#include "rotation.h"
#include "rounds.h"

#define USAGE	\
"description:\n"	\
"	Program taking as input the # of cards in a deck, and printing\n"	\
"	the # of rounds needed to put the deck back into its original order.\n"	\
"	Note that while the permitted input can go up to UINT32_MAX,\n"	\
"	the implementation may take too long for large n and/or\n"	\
"	may overflow the round count.\n"	\
"usage:\n"	\
"	rounds [options]\n"	\
"options:\n"	\
"	-n                  Number of cards in the deck ([1, UINT32_MAX])\n"	\
"	-r                  Use the faster and more efficient rotation algorithm\n"\
"	-h                  Show this help message\n"	\
"example:\n"	\
"	rounds -n 52\n"	\
"	rounds -r -n 52\n"


/* Defines the implementations that can be used to solve the exercise */
typedef enum {
	NAIVE,
	ROTATION,
} implementation_en;

/*
 * Executes a single round as defined by the coding exercise (see PDF).
 * Requires a deck of num_cards in hand and an empty table deck
 */
int
execute_round(deck_t **hand_d, deck_t **table_d, uint32_t num_cards)
{
    deck_t *tmp_d_p = NULL;
    card_t *tmp_c_p = NULL;

    if (!hand_d || !*hand_d || 
        !table_d || !*table_d) {
        return -1;
    }

    /* Ensure table_d is empty at start of round */
    if ((*table_d)->top || (*table_d)->bottom) {
        return -1;
    }
        
    while(num_cards--) {
		/* (1) Remove a card from the top of the hand deck, break if no cards */
        if (0 != remove_card_from_top(*hand_d, &tmp_c_p)) return -1;
        if (!tmp_c_p) break;

		/* (2) Add the removed card to the top of the table deck */
        if (0 != add_card_to_top(*table_d, tmp_c_p)) return -1;

		/* (3) Remove a card from the top of the hand deck, break if no cards */
        if (0 != remove_card_from_top(*hand_d, &tmp_c_p)) return -1;
        if (!tmp_c_p) break;

		/* (4) Add the removed card fo the bottom of the hand deck */
        if (0 != add_card_to_bottom(*hand_d, tmp_c_p)) return -1;
    }
    
    /* Ensure hand_d is empty at end of round */
    if ((*hand_d)->top || (*hand_d)->bottom) {
        return -1;
    }

    /* Swap hand and table decks */
    tmp_d_p = *hand_d;
    *hand_d = *table_d;
    *table_d = tmp_d_p;

    return 0;
}

/* 
 * Sets up a sorted deck with cards having values 0 to num_cards-1 
 */
int
setup_deck(deck_t **d, uint32_t num_cards)
{
    value_t i = 0;
	card_t *c = NULL;

    if (!d)
        return -1;

    *d = create_deck(); 
	if (!*d)
		return -1;

	/* Create the cards, adding them in a sorted fashion in the deck */
    for (i = 0; i < num_cards; i++) {
        c = create_card();
		if (!c)
			return -1;

        c->value = i;
        if (add_card_to_bottom(*d, c) != 0) {
            fprintf(stderr, "Failed to add card\n");
            return -1;
        }
    }

    return 0;
}


/* 
 * Checks if the deck is in the original order it was setup 
 * according to the function setup_deck
 */
bool
is_original_deck(deck_t *d)
{
    card_t *c = NULL;
    value_t i = 0;

    if (!d || !d->top)
        return false;

    /* The original deck had its cards sorted by value AND 
	   the card at index i had a value of i */
    c = d->top;
    for (i = 0; c->below != NULL; i++, c = c->below) {
        if (c->value > c->below->value ||
            c->value != i) {
            return false;
        }
    }

    return true;
}

/*
 * Prints a deck for debugging purposes
 */
void
print_deck(deck_t *d)
{
    if (d) {
        card_t *c = d->top;
        while (c) {
            fprintf(stdout, "%d,", c->value);
            c = c->below;
        }
    }
    fprintf(stdout, "\n");
}

/* 
 * NAIVE Implementation: 
 * Execute rounds on deck until we get back the original deck 
 */
int
execute_naive_impl(deck_t **hand_d, 
				   deck_t **table_d, 
				   uint32_t num_cards,
				   rounds_t *rounds)
{
	if (!hand_d || !*hand_d || !table_d || !*table_d || !rounds) { 
		return -1;
	}

    do { 
        /* Fail if rounds count will overflow */
		if ((*rounds) == MAX_ROUNDS) {
			return -1;
		}
        
		if (0 != execute_round(hand_d, table_d, num_cards)) {
			return -1;
        }

#ifdef TEST_RUN
		print_deck(*hand_d);
#endif

        (*rounds)++;
    } while (!is_original_deck(*hand_d));

	return 0;
}

/* 
 * ROTATION Implementation: 
 * Computes the LCM of the rotation groups as the # of rounds 
 */
int
execute_rotation_impl(deck_t **hand_d, 
					  deck_t **table_d, 
					  uint32_t num_cards,
					  rounds_t *rounds)
{
	value_t *group_arr = NULL;

	if (!hand_d || !*hand_d || !table_d || !*table_d || !rounds) { 
		return -1;
	}
	
	group_arr = malloc(sizeof(value_t) * num_cards);
	if (!group_arr)
		return -1;

	/*
	 * ROTATION: 
	 * (1) Execute a single round to find the groups
	 * (2) Compute the groups in an array
	 * (3) Find LCM of the groups
	 */
	if (0 != execute_round(hand_d, table_d, num_cards)) {
		return -1;
    }

	if (0 != compute_groups(*hand_d, group_arr, num_cards)) {
		return -1;
	}
	
	*rounds = lcm_groups(group_arr, num_cards);

	free(group_arr);

	return 0;
}


int
main(int argc, char **argv)
{
	int					option_char = 0;
	unsigned long		parsed_num_cards = 0;
	implementation_en	impl = NAIVE;
    uint32_t			num_cards = 0;
    rounds_t			rounds = 0; 
    deck_t				*hand_d = NULL, *table_d = NULL;
	int					rc = EXIT_SUCCESS;

	/* Parse command line arguments */
	while ((option_char = getopt(argc, argv, "n:rh")) != -1) {
		switch(option_char) {
			case 'n':
				parsed_num_cards = strtoul(optarg, NULL, 0);
				break;
			case 'r':
				impl = ROTATION;	
				break;
			case 'h':
				fprintf(stdout, "%s", USAGE);
				return EXIT_SUCCESS;
				break;
			default:
				fprintf(stderr, "%s", USAGE);
				return EXIT_FAILURE;
				break;
		}
	}

	if (parsed_num_cards < 1 || parsed_num_cards > UINT32_MAX) {
		fprintf(stderr, "%s", USAGE);
		return EXIT_FAILURE;
	}
	num_cards = parsed_num_cards;

	/* Setup hand deck with num_cards & an empty table deck */
    if (0 != setup_deck(&hand_d, num_cards)) {
		rc = EXIT_FAILURE;
		goto done;
    }
    
    if (0 != setup_deck(&table_d, 0)) {
		rc = EXIT_FAILURE;
		goto done;
    }

#ifdef TEST_RUN
	print_deck(hand_d);
#endif
    
	/* Execute the chosen implementation to find the # of rounds */
	if (impl == ROTATION) {
		rc = execute_rotation_impl(&hand_d, &table_d, num_cards, &rounds);
	} else {
		rc = execute_naive_impl(&hand_d, &table_d, num_cards, &rounds);
	}

	if (0 != rc) {
		rc = EXIT_FAILURE;
		goto done;
	}

	/* Output Result */
#ifdef BASH_RUN
    fprintf(stdout, "%d -> %d\n", num_cards, rounds);
#else
    fprintf(stdout, "%d\n", rounds);
#endif

done:
	delete_deck(&hand_d);
	delete_deck(&table_d);

	return rc;
}
