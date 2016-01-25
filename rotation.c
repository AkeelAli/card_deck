/***************************************************************
 * rotation.c
 *
 * Author: Akeel Ali
 * Date: Dec. 2015
 *
 * Support & utility functions needed to implement the 
 * rotation algorithm solving the Card Deck code exercise 
 ***************************************************************/
#include <stdlib.h>
#include <stdint.h>
#include "rotation.h"

/* Computes the rotation groups for the deck & places them in an array */
int
compute_groups(deck_t *d, value_t *group_arr, uint32_t num_cards)
{
	value_t		*deck_arr = NULL;
	card_t		*c = NULL;
	uint32_t	i = 0;
	uint32_t	index = 0;

	if (!d || !group_arr)
		return -1;

	deck_arr = malloc(sizeof(value_t) * num_cards);
	if (!deck_arr)
		return -1;

	for (i = 0, c = d->top; i < num_cards; i++, c = c->below) {
		deck_arr[i] = c->value;
		group_arr[i] = 1;
	}

	for (i = 0, index = 0; i < num_cards; i++) {
		index = i;
		while (deck_arr[index] != i) {
			index = deck_arr[index];
			group_arr[i]++;
		}
	}
	
	free(deck_arr);

	return 0;
}

/* Computes the GCD of two card values */
value_t
gcd(value_t a, value_t b) 
{
	while (a != b) {
		if (a > b)
			a -= b;
		else
			b -= a;
	}

	return a;
}

/* 
 * After profiling the program with gprof, I realized
 * that my naive gcd implementation above was the bottleneck.
 * So I opted for a more efficient implemenation that I found
 * on wikipedia called binary GCD. Code borrowed from:
 * https://en.wikipedia.org/wiki/Binary_GCD_algorithm
 */
value_t binary_gcd(value_t u, value_t v)
{
  int shift;

  /* GCD(0,v) == v; GCD(u,0) == u, GCD(0,0) == 0 */
  if (u == 0) return v;
  if (v == 0) return u;
 
  /* Let shift := lg K, where K is the greatest power of 2
        dividing both u and v. */
  for (shift = 0; ((u | v) & 1) == 0; ++shift) {
         u >>= 1;
         v >>= 1;
  }
 
  while ((u & 1) == 0)
    u >>= 1;
 
  /* From here on, u is always odd. */
  do {
       /* remove all factors of 2 in v -- they are not common */
       /*   note: v is not zero, so while will terminate */
       while ((v & 1) == 0)  /* Loop X */
           v >>= 1;

       /* Now u and v are both odd. Swap if necessary so u <= v,
          then set v = v - u (which is even). For bignums, the
          swapping is just pointer movement, and the subtraction
          can be done in-place. */
       if (u > v) {
         value_t t = v; v = u; u = t;}  // Swap u and v.
       v = v - u;                       // Here v >= u.
     } while (v != 0);

  /* restore common factors of 2 */
  return u << shift;
}


/* Computes the LCM of the rotation groups */
#define LCM(a,b) ((a*b)/binary_gcd(a,b))
rounds_t
lcm_groups(value_t *group_arr, uint32_t num_cards)
{
	rounds_t rounds = 1;
	uint32_t i = 0;

	for (i = 0; i < num_cards; i++) {
		rounds = LCM(rounds, group_arr[i]);
	}

	return rounds;
}
