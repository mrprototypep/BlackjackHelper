#include "Header.h"
#include <time.h>

using namespace std;

//Returns validated input
Cards GetInput(string s = "") {
	cout << s;
	string input;
	getline(cin, input);

	string value;
	for (char c : input)
		value += tolower(c);

	//please ignore my hideous code
	if (value == "a")
		return cA;
	else if (value == "2")
		return c2;
	else if (value == "3")
		return c3;
	else if (value == "4")
		return c4;
	else if (value == "5")
		return c5;
	else if (value == "6")
		return c6;
	else if (value == "7")
		return c7;
	else if (value == "8")
		return c8;
	else if (value == "9")
		return c9;
	else if (value == "10")
		return c10;
	else if (value == "j")
		return cJ;
	else if (value == "q")
		return cQ;
	else if (value == "k")
		return cK;
	else if (value == "")
		return null;
	else
		return GetInput("Invalid, try again: ");
}

int main()
{
	while (true) {
		for (Card &c : cards) //Reset deck
			c.num = 4;
		for (Card &c : player) //Player has no cards
			c.num = 0;
		for (Card &c : dealer) //Dealer has no cards
			c.num = 0;
		int pcNum = 0; //Number of player cards

		int d = GetInput("Dealer: ");
		if (d == Cards::null)
			continue;
		dealer[d].num++; //"Moves" card to dealer
		cards[d].num--;

		Cards c;
		while ((c = GetInput("Player: ")) != null) {
			pcNum++; //Increment number of player cards
			player[c].num++; //"Moves" card to player
			cards[c].num--;

			cout << endl << "=====================================" << endl;
			cout << "Avg. in deck: " << AverageInDeck() << endl;
			cout << "Player hand: " << GetHandVal(player) << endl;
			cout << "Chance bust: " << ChanceBust(player) * 100 << endl;
			cout << "Dealer chance: " << DealerChance(dealer) * 100 << endl;
			cout << "=====================================" << endl;
			double suggestion = Suggestion();
			cout << "Suggestion: " << (suggestion >= 0 ? "HIT" : "STAND") << endl;
			if (pcNum > 1) //Only outputs if player has more than one card, so it looks nicer
				cout << "Percent difference: " << suggestion * 100 << endl;
			cout << "=====================================" << endl << endl;
		}
	}
}