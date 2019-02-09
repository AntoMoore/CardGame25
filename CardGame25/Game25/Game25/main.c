#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <process.h>

//constants
#define DECK_SIZE 52
#define HAND_SIZE 5
#define MAX_SCORE 25
#define MAX_PLAYERS 6
#define MIN_PLAYERS 2

//struct
typedef struct
{
	int playerNumber;
	int playerScore;
	int playerHand[5];

}Player_p;

//declare functions
void printCards(int card[], int num);
void shuffleDeck(int card[], int num);
char findSuit(int card);
char findValue(int card);
void displayTrumps(char suit);
void displayStartSuit(char suit);
int getTrumpHeart(char value);
int getTrumpDiamond(char value);
int getTrumpClubs(char value);
int getTrumpSpades(char value);
int getHeart(char value);
int getDiamond(char value);
int getClub(char value);
int getSpades(char value);

//main method
void main()
{
	//files
	FILE* outputFile;
	outputFile = fopen("output.txt", "r+"); // NB - "r+" does not automatically generate a file

	//initalise players struct
	Player_p players[MAX_PLAYERS];

	// Array from 0 to 51 
	int cardArr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
		26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };

	//variables
	char menuInput;
	char trumps;
	char startingSuit;
	char key;
	int playersAmount;
	int dealer;
	int topCard;
	int cardSelect;
	int cardRenege;
	int cardInPlay;
	int cardsOnTable[MAX_PLAYERS];
	char cardSuit;
	char cardValue;
	int cardScore;
	int cardsRemaining;
	int cardsPlayed;
	int turn;
	int playerHigh;
	int readNum;
	int i, j, k, l;

	//inatialised variables
	int cardsDelt = 1;
	int round = 0;
	int isTrumpPlayed = 0; // 0 = false, 1 = true
	int mustPlayTrump = 0; // 0 = false, 1 = true
	int isValid = 0; // 0 = false, 1 = true
	int isAllowedRob = 0; // 0 = false , 1 = true
	int dealerCanSteal = 0; // 0 = false , 1 = true
	int cardHigh = 0;
	int counterHigh = 0;
	int scoreHigh = 0;
	int scoreCounter = 0;
	int isGameLoaded = 0; // 0 = false, 1 = true

	//===== start menu =======
	do
	{
		//prompt user for inputs
		printf("Press 1 - New Game \nPress 2 - Load Game \nPress X - Exit\n");
		scanf(" %c", &menuInput);

	} while (menuInput != '1' && menuInput != '2' && menuInput != 'X' && menuInput != 'x');

	//process user inputs
	if (menuInput == '1')
	{
		//start new game
		do
		{
			//promt to enter valid number of players
			printf("\nPlease Enter number of players (2 - 6): ");
			scanf("%d", &playersAmount);
			printf("\nCreating New Game.....");

		} while (playersAmount < MIN_PLAYERS || playersAmount > MAX_PLAYERS);		
				
		//show players in game and initalise values
		for (i = 0; i < playersAmount; i++)
		{
			players[i].playerNumber = (i + 1);
			printf("\nCreated Player: %d", players[i].playerNumber);
			players[i].playerScore = 0;
		}
							
	}
	else if (menuInput == '2')
	{
		//load game
		printf("\nGame Loading.....\n");
		isGameLoaded = 1;

		//read input file
		if (outputFile == NULL)
		{
			printf("ERROR - File not found");
		}
		else
		{
			
			//read game data from file 
			fscanf(outputFile, "%d %d %d %d %d %d %c", &playersAmount, &round, &scoreHigh, &cardsPlayed, &cardsRemaining, &dealer, &trumps);

			//load player data
			for (i = 0; i < playersAmount; i++)
			{
				//player number and score
				fscanf(outputFile, "%d %d", &players[i].playerNumber, &players[i].playerScore);

				//players cards
				for (j = 0; j < HAND_SIZE; j++)
				{
					fscanf(outputFile, "%d", &players[i].playerHand[j]);
				}

			}//for

			//reset EOF pointer
			rewind(outputFile);

			//display game status
			printf("\nGame Status....\n");
			printf("\nPlayers: %d  Round: %d  Highest Score: %d  Cards Remaining: %d  Dealer: %d  Trumps: %c\n", playersAmount, round, scoreHigh, cardsRemaining, (dealer + 1), trumps);

			//display player data
			for (i = 0; i < playersAmount; i++)
			{
				//player number and score
				printf("\nPlayer: %d  Score: %d ", players[i].playerNumber, players[i].playerScore);

			}//for
			printf("\n"); //newline

		}
	}
	else
	{
		//Exit game
		printf("\nGame Exit......\n");
		exit(0);
	}

	//keep playing new rounds until somebody reaches 25
	while (scoreHigh < MAX_SCORE)
	{
		//NB- THIS CODE SHOULD NOT RUN ON LOADED GAME
		if (isGameLoaded == 0)
		{
			//shuffle deck
			puts("\n\nShuffling Deck....");
			shuffleDeck(cardArr, DECK_SIZE);

			//reset cards remaining in each players hand
			cardsRemaining = HAND_SIZE;
			cardsPlayed = 0;
			cardsDelt = 1;
		}

		//select dealer at random (first round only and not a loaded game)
		if (round == 0 && isGameLoaded == 0)
		{
			puts("\nPicking dealer at Random....");
			srand(time(NULL));
			dealer = rand() % playersAmount;
			printf("\nThe Dealer is Player: %d \n", (dealer + 1));
			round++;
		}
		else
		{
			//dealer is winner from last hand
			printf("\nThe Dealer is Player: %d \n", (dealer + 1));
		}

		//deal cards to players (skip if loaded game)
		if (isGameLoaded == 0)
		{
			//deal 5 cards to the player next to Dealer/Winner until reaching the end of table
			for (i = (dealer + 1); i < playersAmount; i++)
			{
				for (j = 0; j < HAND_SIZE; j++)
				{
					players[i].playerHand[j] = cardArr[DECK_SIZE - cardsDelt];
					cardsDelt++;
				}

			}//for

			//deal 5 cards to start of table, until the remaining players all have cards
			for (i = 0; i < (dealer + 1); i++)
			{
				for (j = 0; j < HAND_SIZE; j++)
				{
					players[i].playerHand[j] = cardArr[DECK_SIZE - cardsDelt];
					cardsDelt++;
				}

			}//for

			// turn over the top card from remaining deck to start the round
			topCard = cardArr[DECK_SIZE - cardsDelt];

			//determine trumps for this round
			trumps = findSuit(topCard);
			cardValue = findValue(topCard);
			printf("\nTop Card is: %c %c \n", cardValue, trumps);

			//if top card is ACE of Trumps
			if (cardValue == 'A')
			{
				dealerCanSteal = 1; //true
			}

		}// if game not loaded
		
		//keep Round open until all cards are used (unless a player reaches 25 first)
		while (cardsRemaining > 0 && scoreHigh < MAX_SCORE)
		{
			//reset counters and boolean
			isTrumpPlayed = 0; //false
			isGameLoaded = 0; //false
			counterHigh = 0;
			scoreCounter = 0;
			turn = 0;

			// dealer/winner goes first until end of table
			for (i = dealer; i < playersAmount; i++)
			{
				//reset card steal
				isAllowedRob = 0; //false

				//display current players cards;
				printf("\n==== Player %d ====\n", (i + 1));

				for (j = 0; j < cardsRemaining; j++)
				{
					// find suit and value
					cardSuit = findSuit(players[i].playerHand[j]);
					cardValue = findValue(players[i].playerHand[j]);

					//if they have the Ace of trump they are allowed to steal
					if (cardSuit == trumps && cardValue == 'A')
					{
						isAllowedRob = 1; //true
					}

					//output hand
					printf("\nCard %d: %c %c \n", (j + 1), cardValue, cardSuit);
				}

				//show player what is currently trumps
				displayTrumps(trumps);

				//show player the starting suit (not on dealer/winners turn)
				if (turn != 0)
				{
					displayStartSuit(startingSuit);
				}

				//if top card is ace of trumps, the dealer is allowed to rob it
				if (turn == 0 && cardsRemaining == HAND_SIZE && dealerCanSteal == 1)
				{
					//prompt dealer to steal the ace
					do
					{
						//prompt player if they want to rob the ACE
						printf("\nDo you want to Rob the Ace of Trumps(Y / N): ");
						scanf(" %c", &key);

					} while (key != 'y' && key != 'Y' && key != 'n' && key != 'N');
					if (key == 'y' || key == 'Y')
					{
						do
						{
							//prompt player to pick a card to rob with
							printf("\nSelect a Card to Swap from Hand(1 - %d): ", cardsRemaining);
							scanf("%d", &cardSelect);

						} while (cardSelect < 1 || cardSelect >(HAND_SIZE - cardsPlayed));

						//swap the selected card with the topCard
						players[i].playerHand[cardSelect - 1] = topCard;

						//display players cards again (will have the swapped card)
						printf("\n==== Player %d ====\n", (i + 1));

						for (j = 0; j < cardsRemaining; j++)
						{
							// find suit and value
							cardSuit = findSuit(players[i].playerHand[j]);
							cardValue = findValue(players[i].playerHand[j]);

							//output hand
							printf("\nCard %d: %c %c \n", (j + 1), cardValue, cardSuit);
						}

						//show player what is currently trumps
						displayTrumps(trumps);

						//show player the starting suit (not on dealer/winners turn)
						if (turn != 0)
						{
							displayStartSuit(startingSuit);
						}

					}//if
				}

				// if player has Ace of trump and its their first go, they have the option to "ROB" the top card
				if ((cardsRemaining == HAND_SIZE) && (isAllowedRob == 1))
				{
					printf("\n**You have the Ace of Trumps**");
					cardSuit = findSuit(topCard);
					cardValue = findValue(topCard);
					printf("\nTop Card is: %c %c \n", cardValue, cardSuit);

					do
					{
						//prompt player if they want to rob the top card
						printf("\nDo you want to Rob the top card(Y / N): ");
						scanf(" %c", &key);

					} while (key != 'y' && key != 'Y' && key != 'n' && key != 'N');

					if (key == 'y' || key == 'Y')
					{
						do
						{
							//prompt player to pick a card to rob with
							printf("\nSelect a Card to Swap from Hand(1 - %d): ", cardsRemaining);
							scanf("%d", &cardSelect);

						} while (cardSelect < 1 || cardSelect > (HAND_SIZE - cardsPlayed));

						//swap the selected card with the topCard
						players[i].playerHand[cardSelect - 1] = topCard;

						//display players cards again (will have the swapped card)
						printf("\n==== Player %d ====\n", (i + 1));

						for (j = 0; j < cardsRemaining; j++)
						{
							// find suit and value
							cardSuit = findSuit(players[i].playerHand[j]);
							cardValue = findValue(players[i].playerHand[j]);

							//output hand
							printf("\nCard %d: %c %c \n", (j + 1), cardValue, cardSuit);
						}

						//show player what is currently trumps
						displayTrumps(trumps);

						//show player the starting suit (not on dealer/winners turn)
						if (turn != 0)
						{
							displayStartSuit(startingSuit);
						}

					}//if					
				}//if
				
				do
				{
					//prompt player to pick a card
					printf("\nSelect a Card to Play(1 - %d): ", cardsRemaining);
					scanf("%d", &cardSelect);

				} while (cardSelect < 1 || cardSelect > (HAND_SIZE - cardsPlayed));

				//if trump has been played, check hand to see if they also have a trump
				if (isTrumpPlayed == 1)
				{
					for (l = 0; l < cardsRemaining; l++)
					{
						cardRenege = players[i].playerHand[l];
						cardSuit = findSuit(cardRenege);
						cardValue = findValue(cardRenege);

						//calculate value for the cards in hand
						if ((cardSuit == trumps && cardSuit == 'H') || (cardSuit == 'H' && cardValue == 'A')) // or Ace of Heart Played
						{
							//get score for card 
							cardScore = getTrumpHeart(cardValue);
						}
						else if (cardSuit == trumps && cardSuit == 'D')
						{
							//get score for card 
							cardScore = getTrumpDiamond(cardValue);
						}
						else if (cardSuit == trumps && cardSuit == 'C')
						{
							//get score for card 
							cardScore = getTrumpClubs(cardValue);
						}
						else if (cardSuit == trumps && cardSuit == 'S')
						{
							//get score for card
							cardScore = getTrumpSpades(cardValue);
						}
						else
						{
							//dud card played
							cardScore = 0;
						}

						//if they have a trump they cant Renege it (unless 5 J or AH) - prompt to pick a different card *LOOP*
						if ((cardSuit == trumps) && (cardValue == '5' || cardValue == 'J'))
						{
							// if the current high card beats their trump
							if (cardScore < cardHigh)
							{
								mustPlayTrump = 1; //true
							}
						}
						else if (cardSuit == 'H' && cardValue == 'A') //Ace of Heart
						{
							// if the current high card beats their trump
							if (cardScore < cardHigh)
							{
								mustPlayTrump = 1; //true
							}
						}
						else if (cardSuit == trumps)
						{
							mustPlayTrump = 1; //true
						}

					}//for

					if (mustPlayTrump == 1)
					{
						//keep asking player to play until they choose a valid trump
						do
						{
							//check the suit of the card they selected
							cardInPlay = players[i].playerHand[cardSelect - 1];
							cardSuit = findSuit(cardInPlay);
							cardValue = findValue(cardInPlay);

							//if card played was a valid trump or the Ace of hearts
							if (cardSuit == trumps)
							{
								isValid = 1; //played correct card
							}
							else if (cardSuit == 'H' && cardValue == 'A')
							{
								isValid = 1; //played correct card
							}
							else
							{
								isValid = 0; //played wrong card
								printf("\nERROR - Cant Renege Trump Card!\n");

								//make player pick a new different card
								do
								{
									//prompt player to pick a card
									printf("\nSelect a Card to Play(1 - %d): ", cardsRemaining);
									scanf("%d", &cardSelect);

								} while (cardSelect < 1 || cardSelect > (HAND_SIZE - cardsPlayed));

							}//else

						} while (isValid == 0); //keep asking until valid is true
											
					}//if

				}//if isTrumpPlayed
								
				//find card in players hand and place on table
				cardInPlay = players[i].playerHand[cardSelect - 1];
				cardsOnTable[i] = cardInPlay;

				// find suit and value
				cardSuit = findSuit(cardInPlay);
				cardValue = findValue(cardInPlay);

				//NB - the first card played by dealer/winner is the starting suit for this hand (non trump)
				if (turn == 0)
				{
					startingSuit = cardSuit;

				}//first card played

				//calculate value for the card played
				if ((cardSuit == trumps && cardSuit == 'H') || (cardSuit == 'H' && cardValue == 'A')) // or Ace of Heart Played
				{
					//trump heart card played
					isTrumpPlayed = 1; //true

					//get score for card played
					cardScore = getTrumpHeart(cardValue);
				}
				else if (cardSuit == trumps && cardSuit == 'D')
				{
					//trump diamond card played
					isTrumpPlayed = 1; //true

					//get score for card played
					cardScore = getTrumpDiamond(cardValue);
				}
				else if (cardSuit == trumps && cardSuit == 'C')
				{
					//trump clubs card played
					isTrumpPlayed = 1; //true

					//get score for card played
					cardScore = getTrumpClubs(cardValue);
				}
				else if (cardSuit == trumps && cardSuit == 'S')
				{
					//trump spades card played
					isTrumpPlayed = 1; //true

					//get score for card played
					cardScore = getTrumpSpades(cardValue);
				}
				else if (cardSuit == startingSuit && cardSuit == 'H')
				{
					//normal heart played
					cardScore = getHeart(cardValue);
				}
				else if (cardSuit == startingSuit && cardSuit == 'D')
				{
					//normal diamond played
					cardScore = getDiamond(cardValue);
				}
				else if (cardSuit == startingSuit && cardSuit == 'C')
				{
					//normal club played
					cardScore = getClub(cardValue);
				}
				else if (cardSuit == startingSuit && cardSuit == 'S')
				{
					//normal spade played
					cardScore = getSpades(cardValue);
				}
				else
				{
					//dud card played
					cardScore = 0;
				}

				//remove selected card from players hand (array splice)
				for (k = (cardSelect - 1); k < (cardsRemaining - 1); k++)
				{
					players[i].playerHand[k] = players[i].playerHand[k + 1];
				}

				//find highest value card played
				if ((cardHigh >= 0) && (counterHigh == 0))
				{
					cardHigh = cardScore; //only runs on first pass
					playerHigh = i;
					counterHigh++;
				}
				else if (cardHigh < cardScore)
				{
					cardHigh = cardScore;
					playerHigh = i;
					counterHigh++;
				}

				//end players turn
				turn++;
				mustPlayTrump = 0; //reset for next player

			}//for

			//player 1 turn until remaining players (back to start of table)
			for (i = 0; i < dealer; i++)
			{
				//reset card steal
				isAllowedRob = 0; //false

				//display current players cards
				printf("\n==== Player %d ====\n", (i + 1));

				for (j = 0; j < cardsRemaining; j++)
				{
					// find suit and value
					cardSuit = findSuit(players[i].playerHand[j]);
					cardValue = findValue(players[i].playerHand[j]);

					//if they have the Ace of trump they are allowed to steal
					if (cardSuit == trumps && cardValue == 'A')
					{
						isAllowedRob = 1; //true
					}

					//output hand
					printf("\nCard %d: %c %c \n", (j + 1), cardValue, cardSuit);
				}

				//show player what is currently trumps
				displayTrumps(trumps);

				//show player the starting suit (not on dealer/winners turn)
				if (turn != 0)
				{
					displayStartSuit(startingSuit);
				}

				// if player has Ace of trump and its their first go, they have the option to "ROB" the top card
				if ((cardsRemaining == HAND_SIZE) && (isAllowedRob == 1))
				{
					printf("\n**You have the Ace of Trumps**");
					cardSuit = findSuit(topCard);
					cardValue = findValue(topCard);
					printf("\nTop Card is: %c %c \n", cardValue, cardSuit);

					do
					{
						//prompt player if they want to rob the top card
						printf("\nDo you want to Rob the top card(Y / N): ");
						scanf(" %c", &key);

					} while (key != 'y' && key != 'Y' && key != 'n' && key != 'N');

					if (key == 'y' || key == 'Y')
					{
						do
						{
							//prompt player to pick a card to rob with
							printf("\nSelect a Card to Swap from Hand(1 - %d): ", cardsRemaining);
							scanf("%d", &cardSelect);

						} while (cardSelect < 1 || cardSelect > (HAND_SIZE - cardsPlayed));

						//swap the selected card with the topCard
						players[i].playerHand[cardSelect - 1] = topCard;

						//display players cards again (will have the swapped card)
						printf("\n==== Player %d ====\n", (i + 1));

						for (j = 0; j < cardsRemaining; j++)
						{
							// find suit and value
							cardSuit = findSuit(players[i].playerHand[j]);
							cardValue = findValue(players[i].playerHand[j]);

							//output hand
							printf("\nCard %d: %c %c \n", (j + 1), cardValue, cardSuit);
						}

						//show player what is currently trumps
						displayTrumps(trumps);

						//show player the starting suit (not on dealer/winners turn)
						if (turn != 0)
						{
							displayStartSuit(startingSuit);
						}

					}//if					
				}//if

				do
				{
					//prompt player to pick a card
					printf("\nSelect a Card to Play(1 - %d): ", cardsRemaining);
					scanf("%d", &cardSelect);

				} while (cardSelect < 1 || cardSelect > (HAND_SIZE - cardsPlayed));

				//if trump has been played, check hand to see if they also have a trump
				if (isTrumpPlayed == 1)
				{
					for (l = 0; l < cardsRemaining; l++)
					{
						cardRenege = players[i].playerHand[l];
						cardSuit = findSuit(cardRenege);
						cardValue = findValue(cardRenege);

						//calculate value for the cards in hand
						if ((cardSuit == trumps && cardSuit == 'H') || (cardSuit == 'H' && cardValue == 'A')) // or Ace of Heart Played
						{
							//get score for card 
							cardScore = getTrumpHeart(cardValue);
						}
						else if (cardSuit == trumps && cardSuit == 'D')
						{
							//get score for card 
							cardScore = getTrumpDiamond(cardValue);
						}
						else if (cardSuit == trumps && cardSuit == 'C')
						{
							//get score for card 
							cardScore = getTrumpClubs(cardValue);
						}
						else if (cardSuit == trumps && cardSuit == 'S')
						{
							//get score for card
							cardScore = getTrumpSpades(cardValue);
						}
						else
						{
							//dud card played
							cardScore = 0;
						}

						//if they have a trump they cant Renege it (unless 5 J or AH) - prompt to pick a different card *LOOP*
						if ((cardSuit == trumps) && (cardValue == '5' || cardValue == 'J'))
						{
							// if the current high card beats their trump
							if (cardScore < cardHigh)
							{
								mustPlayTrump = 1; //true
							}
						}
						else if (cardSuit == 'H' && cardValue == 'A') //Ace of Heart
						{
							// if the current high card beats their trump
							if (cardScore < cardHigh)
							{
								mustPlayTrump = 1; //true
							}
						}
						else if (cardSuit == trumps)
						{
							mustPlayTrump = 1; //true
						}

					}//for

					if (mustPlayTrump == 1)
					{
						//keep asking player to play until they choose a valid trump
						do
						{
							//check the suit of the card they selected
							cardInPlay = players[i].playerHand[cardSelect - 1];
							cardSuit = findSuit(cardInPlay);
							cardValue = findValue(cardInPlay);

							//if card played was a valid trump or the Ace of hearts
							if (cardSuit == trumps)
							{
								isValid = 1; //played correct card
							}
							else if (cardSuit == 'H' && cardValue == 'A')
							{
								isValid = 1; //played correct card
							}
							else
							{
								isValid = 0; //played wrong card
								printf("\nERROR - Cant Renege Trump Card!\n");

								//make player pick a new different card
								do
								{
									//prompt player to pick a card
									printf("\nSelect a Card to Play(1 - %d): ", cardsRemaining);
									scanf("%d", &cardSelect);

								} while (cardSelect < 1 || cardSelect > (HAND_SIZE - cardsPlayed));

							}//else

						} while (isValid == 0);

					}//if

				}//if isTrumpPlayed

				//find card in players hand and place on table
				cardInPlay = players[i].playerHand[cardSelect - 1];
				cardsOnTable[i] = cardInPlay;

				// find suit and value
				cardSuit = findSuit(cardInPlay);
				cardValue = findValue(cardInPlay);

				//calculate value for the card played
				if ((cardSuit == trumps && cardSuit == 'H') || (cardSuit == 'H' && cardValue == 'A')) // or Ace of Heart Played
				{
					//trump heart card played
					isTrumpPlayed = 1; //true

					//get score for card played
					cardScore = getTrumpHeart(cardValue);
				}
				else if (cardSuit == trumps && cardSuit == 'D')
				{
					//trump diamond card played
					isTrumpPlayed = 1; //true

					//get score for card played
					cardScore = getTrumpDiamond(cardValue);
				}
				else if (cardSuit == trumps && cardSuit == 'C')
				{
					//trump clubs card played
					isTrumpPlayed = 1; //true

					//get score for card played
					cardScore = getTrumpClubs(cardValue);
				}
				else if (cardSuit == trumps && cardSuit == 'S')
				{
					//trump spades card played
					isTrumpPlayed = 1; //true

					//get score for card played
					cardScore = getTrumpSpades(cardValue);
				}
				else if (cardSuit == startingSuit && cardSuit == 'H')
				{
					//normal heart played
					cardScore = getHeart(cardValue);
				}
				else if (cardSuit == startingSuit && cardSuit == 'D')
				{
					//normal diamond played
					cardScore = getDiamond(cardValue);
				}
				else if (cardSuit == startingSuit && cardSuit == 'C')
				{
					//normal club played
					cardScore = getClub(cardValue);
				}
				else if (cardSuit == startingSuit && cardSuit == 'S')
				{
					//normal spade played
					cardScore = getSpades(cardValue);
				}
				else
				{
					//dud card played
					cardScore = 0;
				}

				//remove selected card from players hand (array splice)
				for (k = (cardSelect - 1); k < (cardsRemaining - 1); k++)
				{
					players[i].playerHand[k] = players[i].playerHand[k + 1];
				}

				//find highest value card played
				if ((cardHigh >= 0) && (counterHigh == 0))
				{
					cardHigh = cardScore; //only runs on first pass
					playerHigh = i;
					counterHigh++;
				}
				else if (cardHigh < cardScore)
				{
					cardHigh = cardScore;
					playerHigh = i;
					counterHigh++;
				}

				//end players turn
				turn++;
				mustPlayTrump = 0; //reset for next player

			}//for

			//display all cards on table
			puts("\nCards played this Hand: \n");

			//header (P:1 P:2 P:3 etc..)
			for (i = 0; i < playersAmount; i++)
			{
				printf("P:%d  ", (i + 1));
			}

			puts(""); //newline

			//output cards played by each player
			for (i = 0; i < playersAmount; i++)
			{
				// find suit and value
				cardSuit = findSuit(cardsOnTable[i]);
				cardValue = findValue(cardsOnTable[i]);

				//output cards played on table
				printf("%c %c  ", cardValue, cardSuit);
			}

			puts(""); //newline

			//counter for the cards remaining and played
			cardsRemaining--;
			cardsPlayed++;

			//display winner for the currect hand
			printf("\nWinning hand was player %d\n", (playerHigh + 1));

			//give 5pts to winner of the hand
			players[playerHigh].playerScore += 5;
			printf("\nPlayer %d has %d points\n", (playerHigh + 1), players[playerHigh].playerScore);

			//winner of hand is the "Dealer" in next hand
			dealer = playerHigh;

			//determine who is currently in the lead
			for (i = 0; i < playersAmount; i++)
			{
				//find player with best score
				if ((scoreHigh >= 0) && (scoreCounter == 0))
				{
					scoreHigh = players[i].playerScore; //only runs on first pass
					scoreCounter++;
				}
				else if (scoreHigh < players[i].playerScore)
				{
					scoreHigh = players[i].playerScore;
					scoreCounter++;
				}
			}

			//prompt player at end of hand
			do
			{
				//prompt user for inputs
				printf("\nPress 1 - Continue Game \nPress 2 - Save Game \nPress 3 - Show Game Status \nPress X - Exit\n");
				scanf(" %c", &menuInput);

			} while (menuInput != '1' && menuInput != '2' && menuInput != '3' && menuInput != 'X' && menuInput != 'x');

			//process user inputs
			if (menuInput == '1')
			{
				//skip logical operator and keep playing
			}
			else if (menuInput == '2')
			{
				//save game data
				printf("\nSaving Game....\n");
				fprintf(outputFile, "%d %d %d %d %d %d %c\n", playersAmount, round, scoreHigh, cardsPlayed, cardsRemaining, dealer, trumps);

				//save player data
				for (i = 0; i < playersAmount; i++)
				{
					//player number and score
					fprintf(outputFile, "%d %d ", players[i].playerNumber, players[i].playerScore);

					//players cards
					for (j = 0; j < HAND_SIZE; j++)
					{
						fprintf(outputFile, "%d ", players[i].playerHand[j]);
					}
					fprintf(outputFile, "\n"); //newline

				}//for

				//close files
				fclose(outputFile);
			}
			else if (menuInput == '3')
			{
				//display game status
				printf("\nGame Status....\n");
				printf("\nPlayers: %d  Round: %d  Highest Score: %d  Cards Remaining: %d  Dealer: %d  Trumps: %c\n", playersAmount, round, scoreHigh, cardsRemaining, (dealer + 1), trumps);

				//display player data
				for (i = 0; i < playersAmount; i++)
				{
					//player number and score
					printf("\nPlayer: %d  Score: %d ", players[i].playerNumber, players[i].playerScore);

				}//for
				printf("\n"); //newline
			}
			else if (menuInput == 'X' || menuInput == 'x')
			{	
				//prompt player at end of hand
				do
				{
					//prompt user for inputs
					printf("\nPress 1 - Start New Game \nPress 2 - Load Previous Game \nPress X - Exit\n");
					scanf(" %c", &menuInput);

				} while (menuInput != '1' && menuInput != '2' && menuInput != 'X' && menuInput != 'x');

				//start new game
				if (menuInput == '1')
				{
					//reset round
					round = -1; // will go back to 0 after "break;" (round++)

					//start new game menu
					do
					{
						//promt to enter valid number of players
						printf("\nPlease Enter number of players (2 - 6): ");
						scanf("%d", &playersAmount);
						printf("\nCreating New Game.....");

					} while (playersAmount < MIN_PLAYERS || playersAmount > MAX_PLAYERS);

					//show players in game and initalise values
					for (i = 0; i < playersAmount; i++)
					{
						players[i].playerNumber = (i + 1);
						printf("\nCreated Player: %d", players[i].playerNumber);
						players[i].playerScore = 0;
					}

					break; //back to start of round (deal new cards)
				}
				else if (menuInput == '2')
				{
					//load game
					printf("\nGame Loading.....\n");
					isGameLoaded = 1;

					//read input file
					if (outputFile == NULL)
					{
						printf("ERROR - File not found");
					}
					else
					{

						//read game data from file 
						fscanf(outputFile, "%d %d %d %d %d %d %c", &playersAmount, &round, &scoreHigh, &cardsPlayed, &cardsRemaining, &dealer, &trumps);

						//load player data
						for (i = 0; i < playersAmount; i++)
						{
							//player number and score
							fscanf(outputFile, "%d %d", &players[i].playerNumber, &players[i].playerScore);

							//players cards
							for (j = 0; j < HAND_SIZE; j++)
							{
								fscanf(outputFile, "%d", &players[i].playerHand[j]);
							}

						}//for

						//reset end of file (EOF) pointer
						rewind(outputFile);
					}
				}
				else
				{
					//exit program
					printf("\nExiting the program....\n");
					exit(0);
				}

			}//if

		}//while (play 5 hands or until 25)

		//round counter
		round++;

	}//while (make new rounds until 25)

	printf("\n** The Winner is Player: %d **\n", (dealer + 1));
	
}//main

// === My functions ===

void printCards(int cards[], int num)
{
	for (int i = 0; i < num; i++)
	{
		//print card to screen
		printf("%d ", cards[i]);
	}
	printf(" \n"); //new line

}//printDeck

void shuffleDeck(int cards[], int num)
{
	//local variables
	int tempValue;

	// Initialize seed randomly 
	srand(time(0));

	for (int i = 0; i < num; i++)
	{
		// Random for remaining positions. 
		int r = i + (rand() % (num - i));

		// Value of first card is assigned to tempValue
		tempValue = cards[i];

		// Value of second card is assigned to first card
		cards[i] = cards[r];

		// Value of tempValue (which contains the initial value of first) is assigned to second
		cards[r] = tempValue;		
	}

}//shuffleDeck

char findSuit(int card)
{
	//local variable
	char suit;

	if (card >= 0 && card <= 12)
	{
		//first 13 cards are Hearts
		suit = 'H';
	}
	else if (card >= 13 && card <= 25)
	{
		//next 13 are Clubs
		suit = 'C';
	}
	else if (card >= 26 && card <= 38)
	{
		//next 13 are Diamonds
		suit = 'D';
	}
	else if (card >= 39 && card <= 51)
	{
		//next 13 are Spades
		suit = 'S';
	}

	return suit;

}//findSuit

char findValue(int card)
{
	//local variable
	char value;

	if (card % 13 == 0)
	{
		value = 'A';
	}
	else if (card % 13 == 1)
	{
		value = '2';
	}
	else if (card % 13 == 2)
	{
		value = '3';
	}
	else if (card % 13 == 3)
	{
		value = '4';
	}
	else if (card % 13 == 4)
	{
		value = '5';
	}
	else if (card % 13 == 5)
	{
		value = '6';
	}
	else if (card % 13 == 6)
	{
		value = '7';
	}
	else if (card % 13 == 7)
	{
		value = '8';
	}
	else if (card % 13 == 8)
	{
		value = '9';
	}
	else if (card % 13 == 9)
	{
		value = 'T';
	}
	else if (card % 13 == 10)
	{
		value = 'J';
	}
	else if (card % 13 == 11)
	{
		value = 'Q';
	}
	else if (card % 13 == 12)
	{
		value = 'K';
	}
	else
	{
		value = '?';
	}

	return value;

}//findValue

void displayTrumps(char suit)
{
	switch (suit)
	{
		case 'H':
			puts("\n**Trumps are Hearts**");
			break;
		case 'C':
			puts("\n**Trumps are Clubs**");
			break;
		case 'D':
			puts("\n**Trumps are Diamonds**");
			break;
		case 'S':
			puts("\n**Trumps are Spades**");
			break;
		default:
			puts("\nERROR - Trumps are ????");
	}
}//displayTrumps

void displayStartSuit(char suit)
{
	switch (suit)
	{
		case 'H':
			puts("\nStarting suit is: Hearts");
			break;
		case 'C':
			puts("\nStarting suit is: Clubs");
			break;
		case 'D':
			puts("\nStarting suit is: Diamonds");
			break;
		case 'S':
			puts("\nStarting suit is: Spades");
			break;
		default:
			puts("\nERROR - Start suit is ????");
	}
}//displayStartSuit

int getTrumpHeart(char value)
{
	//local variable
	int score;

	switch (value)
	{
		case '2':
			score = 21;
			break;
		case '3':
			score = 22;
			break;
		case '4':
			score = 23;
			break;
		case '6':
			score = 24;
			break;
		case '7':
			score = 25;
			break;
		case '8':
			score = 26;
			break;
		case '9':
			score = 27;
			break;
		case 'T':
			score = 28;
			break;
		case 'Q':
			score = 29;
			break;
		case 'K':
			score = 30;
			break;
		case 'A':
			score = 32;
			break;
		case 'J':
			score = 33;
			break;
		case '5':
			score = 34;
			break;
		default:
			score = 0;
		
	}//switch

	return score;

}//getTrumpHeart

int getTrumpDiamond(char value)
{
	//local variable
	int score;

	switch (value)
	{
		case '2':
			score = 21;
			break;
		case '3':
			score = 22;
			break;
		case '4':
			score = 23;
			break;
		case '6':
			score = 24;
			break;
		case '7':
			score = 25;
			break;
		case '8':
			score = 26;
			break;
		case '9':
			score = 27;
			break;
		case 'T':
			score = 28;
			break;
		case 'Q':
			score = 29;
			break;
		case 'K':
			score = 30;
			break;
		case 'A':
			score = 31;
			break;
		case 'J':
			score = 33;
			break;
		case '5':
			score = 34;
			break;
		default:
			score = 0;

	}//switch

	return score;

}//getTrumpDiamond

int getTrumpClubs(char value)
{
	//local variable
	int score;

	switch (value)
	{
		case 'T':
			score = 21;
			break;
		case '9':
			score = 22;
			break;
		case '8':
			score = 23;
			break;
		case '7':
			score = 24;
			break;
		case '6':
			score = 25;
			break;
		case '4':
			score = 26;
			break;
		case '3':
			score = 27;
			break;
		case '2':
			score = 28;
			break;
		case 'Q':
			score = 29;
			break;
		case 'K':
			score = 30;
			break;
		case 'A':
			score = 31;
			break;
		case 'J':
			score = 33;
			break;
		case '5':
			score = 34;
			break;
		default:
			score = 0;

	}//switch

	return score;

}//getTrumpClubs

int getTrumpSpades(char value)
{
	//local variable
	int score;

	switch (value)
	{
		case 'T':
			score = 21;
			break;
		case '9':
			score = 22;
			break;
		case '8':
			score = 23;
			break;
		case '7':
			score = 24;
			break;
		case '6':
			score = 25;
			break;
		case '4':
			score = 26;
			break;
		case '3':
			score = 27;
			break;
		case '2':
			score = 28;
			break;
		case 'Q':
			score = 29;
			break;
		case 'K':
			score = 30;
			break;
		case 'A':
			score = 31;
			break;
		case 'J':
			score = 33;
			break;
		case '5':
			score = 34;
			break;
		default:
			score = 0;

	}//switch

	return score;

}//getTrumpSpades


int getHeart(char value)
{
	//local variable
	int score;

	switch (value)
	{
		case '2':
			score = 1;
			break;
		case '3':
			score = 2;
			break;
		case '4':
			score = 3;
			break;
		case '5':
			score = 4;
			break;
		case '6':
			score = 5;
			break;
		case '7':
			score = 6;
			break;
		case '8':
			score = 7;
			break;
		case '9':
			score = 8;
			break;
		case 'T':
			score = 9;
			break;
		case 'J':
			score = 10;
			break;
		case 'Q':
			score = 11;
			break;
		case 'K':
			score = 12;
			break;
		default:
			score = 0;

	}//switch

	return score;

}//getHeart

int getDiamond(char value)
{
	//local variable
	int score;

	switch (value)
	{
		case 'A':
			score = 1;
			break;
		case '2':
			score = 2;
			break;
		case '3':
			score = 3;
			break;
		case '4':
			score = 4;
			break;
		case '5':
			score = 5;
			break;
		case '6':
			score = 6;
			break;
		case '7':
			score = 7;
			break;
		case '8':
			score = 8;
			break;
		case '9':
			score = 9;
			break;
		case 'T':
			score = 10;
			break;
		case 'J':
			score = 11;
			break;
		case 'Q':
			score = 12;
			break;
		case 'K':
			score = 13;
			break;
		default:
			score = 0;

	}//switch

	return score;

}//getDiamond

int getClub(char value)
{
	//local variable
	int score;

	switch (value)
	{
		case 'T':
			score = 1;
			break;
		case '9':
			score = 2;
			break;
		case '8':
			score = 3;
			break;
		case '7':
			score = 4;
			break;
		case '6':
			score = 5;
			break;
		case '5':
			score = 6;
			break;
		case '4':
			score = 7;
			break;
		case '3':
			score = 8;
			break;
		case '2':
			score = 9;
			break;
		case 'A':
			score = 10;
			break;
		case 'J':
			score = 11;
			break;
		case 'Q':
			score = 12;
			break;
		case 'K':
			score = 13;
			break;
		default:
			score = 0;

	}//switch

	return score;

}//getClub

int getSpades(char value)
{
	//local variable
	int score;

	switch (value)
	{
		case 'T':
			score = 1;
			break;
		case '9':
			score = 2;
			break;
		case '8':
			score = 3;
			break;
		case '7':
			score = 4;
			break;
		case '6':
			score = 5;
			break;
		case '5':
			score = 6;
			break;
		case '4':
			score = 7;
			break;
		case '3':
			score = 8;
			break;
		case '2':
			score = 9;
			break;
		case 'A':
			score = 10;
			break;
		case 'J':
			score = 11;
			break;
		case 'Q':
			score = 12;
			break;
		case 'K':
			score = 13;
			break;
		default:
			score = 0;

	}//switch

	return score;

}//getSpades
