/***************************************************************
 * deck.c
 *
 * Author: Akeel Ali
 * Date: Dec. 2015
 *
 * Operations necessary to create and manipulate
 * a deck of cards for the purposes of solving a 
 * specific coding challenge.
 ***************************************************************/
 #include "deck.h"

int
add_card_to_top(deck_t *d, card_t *c)
{
    if (!d || !c)
        return -1;

    /* Update card neighbours */
    c->below = d->top;
    c->above = NULL;

    /* Update deck */
    if (d->top) {
        d->top->above = c;
    } else {
        /* Adding first card */
        d->bottom = c;
    }
    d->top = c;
    
    return 0;
}

int
add_card_to_bottom(deck_t *d, card_t *c)
{
    if (!d || !c)
        return -1;

    /* Update card neighbours */
    c->below = NULL;
    c->above = d->bottom;

    /* Update deck */
    if (d->bottom) {
        d->bottom->below = c;
    } else {
        /* Adding first card */
        d->top = c;
    }
    d->bottom = c;

    return 0;
}

int
remove_card_from_top(deck_t *d, card_t **c)
{
   if (!d || !c)
       return -1;

    /* Get top */
    *c = d->top;

    /* Update deck */
    if (d->top) {
        /* Single card removed */
        if (d->top == d->bottom) {
            d->top = d->bottom = NULL;
        } else {
            d->top = d->top->below;
        }
    }
    
    return 0;
}

/* remove_card_from_bottom not necessary for this problem */

card_t *
create_card()
{
	card_t *c = NULL;

	c = malloc(sizeof(card_t));
	if (!c)
		return NULL;

	c->above = NULL;
	c->below = NULL;
	c->value = 0;

	return c;
}

deck_t *
create_deck()
{
	deck_t *d = NULL;

	d = malloc(sizeof(deck_t));
	if (!d)
		return NULL;

	d->top = NULL;
	d->bottom = NULL;

	return d;
}

void
delete_deck(deck_t **d)
{
	card_t	*c = NULL;
	card_t	*t = NULL;

	if (d && *d) {
		c = (*d)->top;
		
		while(c) {
			t = c;
			c = c->below;
			free(t);
		}

		free(*d);
		*d = NULL;
	}
}
