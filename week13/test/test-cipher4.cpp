#include <algorithm>

#include "include/catch.hpp"

#include "cipher04.h"

TEST_CASE("Card")
{
    SECTION("Int Value")
    {
        Card c = Card();

        c.suit = 'C'; // Clubs
        for (int i=1; i<kSuitSize; i++) {
            c.value = i;
            REQUIRE(c.intValue() == i);
        }

        c.suit = 'D'; // Diamonds
        for (int i=1; i<kSuitSize; i++) {
            c.value = i;
            REQUIRE(c.intValue() == i + kSuitSize);
        }

        c.suit = 'H'; // Hearts
        for (int i=1; i<kSuitSize; i++) {
            c.value = i;
            REQUIRE(c.intValue() == i + (kSuitSize * 2));
        }

        c.suit = 'S'; // Spades
        for (int i=1; i<kSuitSize; i++) {
            c.value = i;
            REQUIRE(c.intValue() == i + (kSuitSize * 3));
        }

        c.suit = 'J'; // Jokers
        c.value = 1;
        REQUIRE(c.intValue() == kDeckSize - 1);
        c.value = 2;
        REQUIRE(c.intValue() == kDeckSize - 1);
    }

    SECTION("String Value")
    {
        Card c = Card();

        for (int s=0; s<4; s++) {
            c.suit = kValidSuits[s];
            for (int i=1; i<kSuitSize; i++) {
                c.value = i;
                std::ostringstream os;
                os << kValidSuits[s];
                if (i <= 10) {
                    os << i;
                } else {
                    if (i == 11) {
                        os << 'J';
                    } else if (i == 12) {
                        os << 'Q';
                    } else if (i == 13) {
                        os << 'K';
                    }
                }
                REQUIRE(c.stringValue() == os.str());
            }
        }

        c.suit = 'J'; // Joker
        c.value = 1;
        REQUIRE(c.stringValue() == "J1");
        c.value = 2;
        REQUIRE(c.stringValue() == "J2");
    }

    SECTION("New Card from Int Value")
    {
        for (int s=0; s<4; s++) {
            for (int i=1; i<=kSuitSize; i++) {
                Card *c = Card::newCardFromIntValue(i + (kSuitSize * s));
                INFO("Suite is " << kValidSuits[s]);
                INFO("Value is " << i + (kSuitSize * s));
                REQUIRE(c->suit == kValidSuits[s]);
                REQUIRE(c->value == i);
                delete(c);
            }
        }
    }
}

TEST_CASE("Deck")
{
    SECTION("Deck Init")
    {
        Deck deck;
        for (int i=0; i<kDeckSize - 1; i++) {
            Card *c = deck.getCard(i);
            REQUIRE(c->intValue() == i + 1);
        }
        // Last joker
        Card *c = deck.getCard(kDeckSize - 1);
        REQUIRE(c->intValue() == kDeckSize - 1);
    }

    SECTION("Get Position of Card")
    {
        Deck deck;
        for (int i=1; i<=kDeckSize; i++) {
            Card *c = Card::newCardFromIntValue(i);
            REQUIRE(deck.getPosition(c) == i - 1);
            delete(c);
        }
    }

    SECTION("Swap Card")
    {
        Deck deck;
        Card *card = deck.getCard(10); // C10
        Card *card2 = deck.getCard(26); // H1
        deck.swap(card, 26);
        REQUIRE(deck.getCard(26) == card);
        REQUIRE(deck.getCard(10) == card2);
    }

    SECTION("Move Card")
    {
        Deck deck;
        deck.move(9 /* C10 */, 12 /* CK */);
        REQUIRE(deck.getCard(9)->intValue() == 11);
        REQUIRE(deck.getCard(10)->intValue() == 12);
        REQUIRE(deck.getCard(11)->intValue() == 13);
        REQUIRE(deck.getCard(12)->intValue() == 10);

        deck = Deck();
        deck.move(kDeckSize - 2 /* Joker A */, 0);
        // REQUIRE(deck.getCard())
    }

    SECTION("Cut Deck")
    {
        for (int cut=1; cut<kDeckSize; cut++) {
            Deck deck;
            INFO("Deck (initialized): " << deck.toString());
            deck.cut(cut);
            for (int i=0; i<kDeckSize; i++) {
                INFO("Iterator is " << i << ", cut is " << cut);
                INFO("Deck: " << deck.toString());
                Card *c = deck.getCard(i);
                int expectedValue = i + cut;
                expectedValue = expectedValue % kDeckSize;
                expectedValue++;
                expectedValue = std::min(expectedValue, kDeckSize - 1);
                REQUIRE(c->intValue() == expectedValue /* Handle Jokers */);
            }
        }
    }

    SECTION("Cut Deck (Keep Bottom Card)")
    {
        for (int cut=1; cut<kDeckSize; cut++) {
            Deck deck;
            INFO("Deck (initialized): " << deck.toString());
            Card *lastCard = deck.getCard(kDeckSize - 1);
            deck.cut(cut, true);
            REQUIRE(lastCard->intValue() == lastCard->intValue());

            for (int i=0; i<kDeckSize - 1; i++) {
                INFO("Iterator is " << i);
                INFO("Deck: " << deck.toString());
                Card *c = deck.getCard(i);
                int expectedValue = ((i + cut) % (kDeckSize - 1)) + 1;
                REQUIRE(c->intValue() == std::min(expectedValue, kDeckSize - 1) /* Handle Jokers */);
            }
        }
    }

    SECTION("Triple Cut Deck") {
        Deck deck;
        INFO("Deck (initialized): " << deck.toString());
        int offset1 = 10;
        int offset2 = 30;

        deck.tripleCut(offset1, offset2);
        for (int i = 0; i < kDeckSize - offset2 - 1; i++) {
            INFO("Deck is " << deck.toString());
            int expectedValue = offset2 + i + 2;
            Card *c = deck.getCard(i);
            REQUIRE(c->intValue() == std::min(expectedValue, kDeckSize - 1) /* Handle Jokers */);
        }
        for (int i = kDeckSize - offset2 - 1; i < kDeckSize - offset1; i++) {
            INFO("Deck is " << deck.toString());
            int expectedValue = offset1 + (i - (kDeckSize - offset2 - 2));
            Card *c = deck.getCard(i);
            REQUIRE(c->intValue() == std::min(expectedValue, kDeckSize - 1) /* Handle Jokers */);
        }
        for (int i = kDeckSize - offset1; i < kDeckSize; i++) {
            INFO("Deck is " << deck.toString());
            int expectedValue = i - (kDeckSize - offset1) + 1;
            Card *c = deck.getCard(i);
            REQUIRE(c->intValue() == std::min(expectedValue, kDeckSize - 1) /* Handle Jokers */);
        }
    }

    SECTION("Solitaire")
    {
        Deck deck;
        deck.solitaire();
        REQUIRE(deck.getCard(0)->intValue() == 2);
        REQUIRE(deck.getCard(1)->intValue() == 3);
        REQUIRE(deck.getCard(kDeckSize - 1)->intValue() == 1);
        REQUIRE(deck.getCard(kDeckSize - 2)->intValue() == 53);
        REQUIRE(deck.getCard(kDeckSize - 3)->intValue() == 53);
        REQUIRE(deck.getCard(kDeckSize - 4)->intValue() == 52);
    }

    SECTION("Key Stream (Unkeyed)")
    {
        Deck deck;
        std::vector<int> keyStream = deck.keyStream(11);
        REQUIRE(keyStream[0] == 4);
        REQUIRE(keyStream[1] == 49 - 26);
        REQUIRE(keyStream[2] == 10);
        REQUIRE(keyStream[3] == 24);
        REQUIRE(keyStream[4] == 8);
        REQUIRE(keyStream[5] == 51 - 26);
        REQUIRE(keyStream[6] == 44 - 26);
        REQUIRE(keyStream[7] == 6);
        REQUIRE(keyStream[8] == 4);
        REQUIRE(keyStream[9] == 33 - 26);
    }
}

TEST_CASE("Letter Conversion")
{
    char letters[]  = { 'a', 'b', 'c', 'f', 'm', 'o', 'w', 'z' };
    char uLetters[] = { 'A', 'B', 'C', 'F', 'M', 'O', 'W', 'Z' };
    int intValues[] = {  1,   2,   3,   6,   13,  15,  23,  26 };

    SECTION("Letter to Number")
    {
        for (int i=0; i<8; i++) {
            REQUIRE(IntValueOfLetter(letters[i]) == intValues[i]);
            REQUIRE(IntValueOfLetter(uLetters[i]) == intValues[i]);
        }
    }

    SECTION("Number to Letter")
    {
        for (int i=0; i<8; i++) {
            REQUIRE(CharacterValueOfInt(intValues[i]) == uLetters[i]);
        }
    }
}

TEST_CASE("Cipher")
{
    SECTION("Encryption (Unkeyed Deck)")
    {
        Cipher04 cipher;
        std::string cipherText = cipher.encrypt("YOURCIPHERISWORKING", "");
        REQUIRE(cipherText == "CLEPKHHNIYCFPWHFDFEH");
    }

    SECTION("Encryption (Keyed Deck)")
    {
        Cipher04 cipher;
        std::string cipherText = cipher.encrypt("SOLITAIRE", "CRYPTONOMICON");
        REQUIRE(cipherText == "KIRAKSFJAN");
    }

    SECTION("Decryption (Keyed Deck)")
    {
        Cipher04 cipher;
        std::string cipherText = cipher.decrypt("KIRAKSFJAN", "CRYPTONOMICON");
        REQUIRE(cipherText == "SOLITAIREX");
    }
}