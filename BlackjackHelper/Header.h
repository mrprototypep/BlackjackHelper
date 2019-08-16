#pragma once
#include <iostream>
#include <vector>
#include <string>

using namespace std;

enum Cards {
	//Note that null is used to reset the environment in main().
	cA, c2, c3, c4, c5, c6, c7, c8, c9, c10, cJ, cQ, cK, null
};

const vector<string> cardVals { "a", "2", "3", "4", "5", "6", "7", "8", "9", "10", "j", "q", "k" };

//A card
struct Card {
	Cards c;
	int num;
};

Card cards[]{ {cA}, {c2}, {c3}, {c4}, {c5}, {c6}, {c7}, {c8}, {c9}, {c10}, {cJ}, {cQ}, {cK} }; //Deck
Card player[]{ {cA}, {c2}, {c3}, {c4}, {c5}, {c6}, {c7}, {c8}, {c9}, {c10}, {cJ}, {cQ}, {cK} }; //Player cards
Card dealer[]{ {cA}, {c2}, {c3}, {c4}, {c5}, {c6}, {c7}, {c8}, {c9}, {c10}, {cJ}, {cQ}, {cK} }; //Dealer cards

//Gets the value of a hand
int GetHandVal(Card hand[13]) {
	int handVal = 0;
	int aces = 0;
	for (int i = 0; i < 13; i++) {
		if (hand[i].c == cA)
			aces+=hand[i].num; //Aces are calculated last
		else {
			switch (hand[i].c) {
			case cJ:
			case cQ:
			case cK:
				handVal += 10 * hand[i].num;
				break;
			default:
				handVal += (hand[i].c + 1) * hand[i].num;
				break;
			}
		}
	}

	handVal += aces; //Adds 10 for every ace (where it can fit)
	for (int i = 0; i < aces; i++) {
		if (handVal + 10 <= 21)
			handVal += 10;
	}
	return handVal;
}

//Gets value of card, based on the preexisting hand.
int GetCardVal(Card& c, Card hand[13]) {
	switch (c.c) {
	case cA:
		if (GetHandVal(hand) <= 10)
			return 11;
		else
			return 1;
		break;
	case cJ:
	case cQ:
	case cK:
		return 10;
		break;
	default:
		return c.c + 1;
		break;
	}
}

//Gets value of card.
int GetCardVal(Card& c) {
	switch (c.c) {
	case cA:
		return 1;
	case cJ:
	case cQ:
	case cK:
		return 10;
		break;
	default:
		return c.c + 1;
		break;
	}
}

//Calculates and returns the average value of a the cards in the deck
double AverageInDeck(Card hand[13] = cards) {
	int sum = 0;
	int num = 0;
	for (int i = 0; i < 13; i++) {
		sum += GetCardVal(hand[i]) * hand[i].num;
		num += hand[i].num;
	}

	return ((double)sum) / num;
}

//Calculates the chance that, on a draw, the hand would bust.
double ChanceBust(Card hand[13]) {
	int busts = 0;
	int num = 0;
	for (int i = 0; i < 13; i++) {
		busts += (GetCardVal(cards[i]) + GetHandVal(hand) > 21) ? cards[i].num : 0;
		num += cards[i].num;
	}
	return ((double)busts) / num;
}

//Recursively calculates the chance the dealer would win. Stops at 10 iterations, although it's unlikely it'd get there. Takes input of dealer hand, and the value of the player hand (defaults to calculated value)
long double DealerChance(Card dH[13], const double p = GetHandVal(player), int iterations = 0) {
	long double sum = 0;
	long num = 0;

	int dVal = GetHandVal(dH); //Hand value of dealer

	if (dVal > 21)
		return 0;
	else if (dVal > 16) //Dealer doesn't hit on 17 (which is dumb, vegas rules are better)
		return dVal > p;
	else if (iterations > 10) {
		int ndVal = dVal + AverageInDeck(dH); //Basically an approximation for if the dealer would win, if it gets this far.
		return (ndVal > p) && (ndVal <= 21);
	}
	else {
		for (int i = 0; i < 13; i++) {
			if (4 - player[i].num - dH[i].num <= 0) //If cards in deck is 0
				continue;

			Card dHmod[13]{ dH[0], dH[1], dH[2], dH[3], dH[4], dH[5], dH[6], dH[7], dH[8], dH[9], dH[10], dH[11], dH[12] };
			dHmod[dH[i].c].num++; //Ensures a copy of the array, because it's nicer. Uses a bit more memory but it doesn't exceed like 2MB so it's fine.

			sum += DealerChance(dHmod, p, iterations + 1) * dHmod[i].num;
			num += dHmod[i].num;
		}
	}

	return sum / num;
}

double Suggestion() {
	double chance = 1.0 - DealerChance(dealer); //Player chance as it stands
	double modChance = (1.0 - DealerChance(dealer, GetHandVal(player) + AverageInDeck())) * (1.0 - ChanceBust(player)); //Chance if hit is calculated as what the player's chance would be if their deck was AverageInDeck() larger, multiplied by their chance of not busting.
	return modChance - chance;
}