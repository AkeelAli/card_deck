/***************************************************************
 * rotation.h
 *
 * Author: Akeel Ali
 * Date: Dec. 2015
 *
 * Support & Utility functions needed to implement the 
 * rotation algorithm solving the code exercise 
 ***************************************************************/
#ifndef	__ROTATION_H__ 
#define __ROTATION_H__ 
#include "deck.h"
#include "rounds.h"

/* Computes the rotation groups for the deck in an array */
int
compute_groups(deck_t *d, value_t *group_arr, uint32_t num_cards);

/* Computes the LCM of the rotation groups */
rounds_t
lcm_groups(value_t *group_arr, uint32_t num_cards);

#endif /* __ROTATION_H__ */
