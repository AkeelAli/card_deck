/***************************************************************
 * deck.h
 *
 * Author: Akeel Ali
 * Date: Dec. 2015
 *
 * Defines and operations necessary to create and manipulate
 * a deck of cards for the purposes of solving a specific coding
 * challenge.
 ***************************************************************/
#ifndef __DECK_H__
#define __DECK_H__

#include <stdlib.h>
#include <stdint.h>

typedef uint32_t value_t;

typedef struct card_ {
    struct card_ *above;
    struct card_ *below;
    value_t value;
} card_t;

typedef struct deck_ {
   card_t *top;
   card_t *bottom;
} deck_t;

int
add_card_to_top(deck_t *d, card_t *c);

int
add_card_to_bottom(deck_t *d, card_t *c);

int
remove_card_from_top(deck_t *d, card_t **c);

card_t *
create_card();

deck_t *
create_deck();

void
delete_deck(deck_t **d);

#endif /* __DECK_H__ */
