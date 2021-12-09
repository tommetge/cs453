#include <algorithm>
#include <random>

#include "include/catch.hpp"

#include "cipher04.h"

static char GetRandomCharacter(bool extended)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(
        extended ? ' ' : 'A', extended ? '~' : 'Z');

    return (char)dist(gen);
}

static std::string GetRandomString(int length, bool extended)
{
    std::ostringstream os;
    for (int i=0; i<length; i++) {
        os << GetRandomCharacter(extended);
    }

    return os.str();
}

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
        REQUIRE(c.intValue() == DeckSize(false) - 1);
        c.value = 2;
        REQUIRE(c.intValue() == DeckSize(false) - 1);
    }

    SECTION("Int Value (Extended Deck)")
    {
        Card c = Card(true);

        c.suit = 'C'; // Clubs
        for (int i=1; i<kSuitSizeExtended; i++) {
            c.value = i;
            REQUIRE(c.intValue() == i);
        }

        c.suit = 'D'; // Diamonds
        for (int i=1; i<kSuitSizeExtended; i++) {
            c.value = i;
            REQUIRE(c.intValue() == i + kSuitSizeExtended);
        }

        c.suit = 'H'; // Hearts
        for (int i=1; i<kSuitSizeExtended; i++) {
            c.value = i;
            REQUIRE(c.intValue() == i + (kSuitSizeExtended * 2));
        }

        c.suit = 'S'; // Spades
        for (int i=1; i<kSuitSizeExtended; i++) {
            c.value = i;
            REQUIRE(c.intValue() == i + (kSuitSizeExtended * 3));
        }

        c.suit = 'J'; // Jokers
        c.value = 1;
        REQUIRE(c.intValue() == DeckSize(true) - 1);
        c.value = 2;
        REQUIRE(c.intValue() == DeckSize(true) - 1);
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

    SECTION("String Value (Extended Deck)")
    {
        Card c = Card(true);

        for (int s=0; s<4; s++) {
            c.suit = kValidSuits[s];
            for (int i=1; i<kSuitSizeExtended; i++) {
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
                    } else  if (i == 14) {
                        os << 'a';
                    } else  if (i == 15) {
                        os << 'b';
                    } else  if (i == 16) {
                        os << 'c';
                    } else  if (i == 17) {
                        os << 'd';
                    } else  if (i == 18) {
                        os << 'e';
                    } else  if (i == 19) {
                        os << 'f';
                    } else  if (i == 20) {
                        os << 'g';
                    } else  if (i == 21) {
                        os << 'h';
                    } else  if (i == 22) {
                        os << 'i';
                    } else  if (i == 23) {
                        os << 'j';
                    } else  if (i == 24) {
                        os << 'k';
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

    SECTION("New Card from Int Value (Extended Deck)")
    {
        for (int s=0; s<4; s++) {
            for (int i=1; i<=kSuitSizeExtended; i++) {
                Card *c = Card::newCardFromIntValue(i + (kSuitSizeExtended * s), true);
                INFO("Suite is " << kValidSuits[s]);
                INFO("Value is " << i + (kSuitSizeExtended * s));
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
        for (int i=0; i<DeckSize(false) - 1; i++) {
            Card *c = deck.getCard(i);
            REQUIRE(c->intValue() == i + 1);
        }
        // Last joker
        Card *c = deck.getCard(DeckSize(false) - 1);
        REQUIRE(c->intValue() == DeckSize(false) - 1);
    }

    SECTION("Get Position of Card")
    {
        Deck deck;
        for (int i=1; i<=DeckSize(false) - 2; i++) {
            Card *c = Card::newCardFromIntValue(i);
            REQUIRE(deck.getPosition(c->intValue()) == i - 1);
            delete(c);
        }
    }

    SECTION("Swap Card")
    {
        Deck deck;
        Card *card = deck.getCard(10); // C10
        Card *card2 = deck.getCard(26); // H1
        deck.swap(10, 26);
        REQUIRE(deck.getCard(26) == card);
        REQUIRE(deck.getCard(10) == card2);
    }

    SECTION("Swap Card (Extended Deck)")
    {
        Deck deck(true);
        Card *card = deck.getCard(10); // C10
        Card *card2 = deck.getCard(65); // H1
        deck.swap(10, 65);
        REQUIRE(deck.getCard(65) == card);
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
        deck.move(DeckSize(false) - 2 /* Joker A */, 0);
        // REQUIRE(deck.getCard())
    }

    SECTION("Move Card (Extended Deck)")
    {
        Deck deck;
        deck.move(9 /* C10 */, 12 /* CK */);
        REQUIRE(deck.getCard(9)->intValue() == 11);
        REQUIRE(deck.getCard(10)->intValue() == 12);
        REQUIRE(deck.getCard(11)->intValue() == 13);
        REQUIRE(deck.getCard(12)->intValue() == 10);

        deck = Deck();
        deck.move(DeckSize(false) - 2 /* Joker A */, 0);
        // REQUIRE(deck.getCard())
    }

    SECTION("Cut Deck")
    {
        for (int cut=1; cut<DeckSize(false); cut++) {
            Deck deck;
            INFO("Deck (initialized): " << deck.toString());
            deck.cut(cut);
            for (int i=0; i<DeckSize(false); i++) {
                INFO("Iterator is " << i << ", cut is " << cut);
                INFO("Deck: " << deck.toString());
                Card *c = deck.getCard(i);
                int expectedValue = i + cut;
                expectedValue = expectedValue % DeckSize(false);
                expectedValue++;
                expectedValue = std::min(expectedValue, DeckSize(false) - 1);
                REQUIRE(c->intValue() == expectedValue /* Handle Jokers */);
            }
        }
    }

    SECTION("Cut Deck (Extended Deck)")
    {
        for (int cut=1; cut<DeckSize(true); cut++) {
            Deck deck(true);
            INFO("Deck (initialized): " << deck.toString());
            deck.cut(cut);
            for (int i=0; i<DeckSize(true); i++) {
                INFO("Iterator is " << i << ", cut is " << cut);
                INFO("Deck: " << deck.toString());
                Card *c = deck.getCard(i);
                int expectedValue = i + cut;
                expectedValue = expectedValue % DeckSize(true);
                expectedValue++;
                expectedValue = std::min(expectedValue, DeckSize(true) - 1);
                REQUIRE(c->intValue() == expectedValue /* Handle Jokers */);
            }
        }
    }

    SECTION("Cut Deck (Keep Bottom Card)")
    {
        for (int cut=1; cut<DeckSize(false); cut++) {
            Deck deck;
            INFO("Deck (initialized): " << deck.toString());
            Card *lastCard = deck.getCard(DeckSize(false) - 1);
            deck.cut(cut, true);
            REQUIRE(lastCard->intValue() == lastCard->intValue());

            for (int i=0; i<DeckSize(false) - 1; i++) {
                INFO("Iterator is " << i);
                INFO("Deck: " << deck.toString());
                Card *c = deck.getCard(i);
                int expectedValue = ((i + cut) % (DeckSize(false) - 1)) + 1;
                REQUIRE(c->intValue() == std::min(expectedValue, DeckSize(false) - 1) /* Handle Jokers */);
            }
        }
    }

    SECTION("Cut Deck (Keep Bottom Card, Extended Deck)")
    {
        for (int cut=1; cut<DeckSize(true); cut++) {
            Deck deck(true);
            INFO("Deck (initialized): " << deck.toString());
            Card *lastCard = deck.getCard(DeckSize(true) - 1);
            deck.cut(cut, true);
            REQUIRE(lastCard->intValue() == lastCard->intValue());

            for (int i=0; i<DeckSize(true) - 1; i++) {
                INFO("Iterator is " << i);
                INFO("Deck: " << deck.toString());
                Card *c = deck.getCard(i);
                int expectedValue = ((i + cut) % (DeckSize(true) - 1)) + 1;
                REQUIRE(c->intValue() == std::min(expectedValue, DeckSize(true) - 1) /* Handle Jokers */);
            }
        }
    }

    SECTION("Triple Cut Deck") {
        Deck deck;
        INFO("Deck (initialized): " << deck.toString());
        int offset1 = 10;
        int offset2 = 30;

        deck.tripleCut(offset1, offset2);
        for (int i = 0; i < DeckSize(false) - offset2 - 1; i++) {
            INFO("Deck is " << deck.toString());
            int expectedValue = offset2 + i + 2;
            Card *c = deck.getCard(i);
            REQUIRE(c->intValue() == std::min(expectedValue, DeckSize(false) - 1) /* Handle Jokers */);
        }
        for (int i = DeckSize(false) - offset2 - 1; i < DeckSize(false) - offset1; i++) {
            INFO("Deck is " << deck.toString());
            int expectedValue = offset1 + (i - (DeckSize(false) - offset2 - 2));
            Card *c = deck.getCard(i);
            REQUIRE(c->intValue() == std::min(expectedValue, DeckSize(false) - 1) /* Handle Jokers */);
        }
        for (int i = DeckSize(false) - offset1; i < DeckSize(false); i++) {
            INFO("Deck is " << deck.toString());
            int expectedValue = i - (DeckSize(false) - offset1) + 1;
            Card *c = deck.getCard(i);
            REQUIRE(c->intValue() == std::min(expectedValue, DeckSize(false) - 1) /* Handle Jokers */);
        }
    }

    SECTION("Triple Cut Deck (Extended Deck)") {
        Deck deck(true);
        INFO("Deck (initialized): " << deck.toString());
        int offset1 = 10;
        int offset2 = 60;

        deck.tripleCut(offset1, offset2);
        for (int i = 0; i < DeckSize(true) - offset2 - 1; i++) {
            INFO("Deck is " << deck.toString());
            int expectedValue = offset2 + i + 2;
            Card *c = deck.getCard(i);
            REQUIRE(c->intValue() == std::min(expectedValue, DeckSize(true) - 1) /* Handle Jokers */);
        }
        for (int i = DeckSize(true) - offset2 - 1; i < DeckSize(true) - offset1; i++) {
            INFO("Deck is " << deck.toString());
            int expectedValue = offset1 + (i - (DeckSize(true) - offset2 - 2));
            Card *c = deck.getCard(i);
            REQUIRE(c->intValue() == std::min(expectedValue, DeckSize(true) - 1) /* Handle Jokers */);
        }
        for (int i = DeckSize(true) - offset1; i < DeckSize(true); i++) {
            INFO("Deck is " << deck.toString());
            int expectedValue = i - (DeckSize(true) - offset1) + 1;
            Card *c = deck.getCard(i);
            REQUIRE(c->intValue() == std::min(expectedValue, DeckSize(true) - 1) /* Handle Jokers */);
        }
    }

    SECTION("Solitaire")
    {
        Deck deck;
        deck.solitaire();
        REQUIRE(deck.getCard(0)->intValue() == 2);
        REQUIRE(deck.getCard(1)->intValue() == 3);
        REQUIRE(deck.getCard(DeckSize(false) - 1)->intValue() == 1);
        REQUIRE(deck.getCard(DeckSize(false) - 2)->intValue() == 53);
        REQUIRE(deck.getCard(DeckSize(false) - 3)->intValue() == 53);
        REQUIRE(deck.getCard(DeckSize(false) - 4)->intValue() == 52);
    }

    SECTION("Solitaire (Extended Deck)")
    {
        Deck deck(true);
        deck.solitaire();
        REQUIRE(deck.getCard(0)->intValue() == 2);
        REQUIRE(deck.getCard(1)->intValue() == 3);
        REQUIRE(deck.getCard(DeckSize(true) - 1)->intValue() == 1);
        REQUIRE(deck.getCard(DeckSize(true) - 2)->intValue() == DeckSize(true) - 1);
        REQUIRE(deck.getCard(DeckSize(true) - 3)->intValue() == DeckSize(true) - 1);
        REQUIRE(deck.getCard(DeckSize(true) - 4)->intValue() == DeckSize(true) - 2);
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
    char letters[]     = { 'a', 'b', 'c', 'f', 'm', 'o', 'w', 'z' };
    char uLetters[]    = { 'A', 'B', 'C', 'F', 'M', 'O', 'W', 'Z' };
    int intValues[]    = {  1,   2,   3,   6,   13,  15,  23,  26 };
    char symbols[]     = { ' ', '&', '5', '?', '~' };
    int symIntValues[] = {  1,   7,   22,  32,  95 };

    SECTION("Letter to Number")
    {
        for (int i=0; i<8; i++) {
            REQUIRE(IntValueOfCharacter(letters[i]) == intValues[i]);
            REQUIRE(IntValueOfCharacter(uLetters[i]) == intValues[i]);
        }
    }

    SECTION("Letter to Number (Extended)")
    {
        for (int i=0; i<5; i++) {
            REQUIRE(IntValueOfCharacter(symbols[i], true) == symIntValues[i]);
        }
        REQUIRE(IntValueOfCharacter('~', true) == 95);
        REQUIRE(CharacterValueOfInt(95, true) == '~');
    }

    SECTION("Number to Letter")
    {
        for (int i=0; i<8; i++) {
            REQUIRE(CharacterValueOfInt(intValues[i]) == uLetters[i]);
        }
    }

    SECTION("Number to Letter (Extended)")
    {
        for (int i=0; i<5; i++) {
            REQUIRE(CharacterValueOfInt(symIntValues[i], true) == symbols[i]);
        }
    }

    SECTION("Defines")
    {
        REQUIRE(kCharacterSetSize == 26);
        REQUIRE(kCharacterSetSizeExtended == 95);
    }
}

TEST_CASE("Cipher")
{
    SECTION("Encryption (Unkeyed Deck)")
    {
        Cipher04 cipher;
        std::string cipherText = cipher.encrypt("YOURCIPHERISWORKING", "", false);
        REQUIRE(cipherText == "CLEPKHHNIYCFPWHFDFEH");
        std::string plainText = cipher.decrypt(cipherText, "", false);
        REQUIRE(plainText == "YOURCIPHERISWORKINGX");
    }

    SECTION("Encryption (Keyed Deck)")
    {
        Cipher04 cipher;
        std::string cipherText = cipher.encrypt("SOLITAIRE", "CRYPTONOMICON", false);
        REQUIRE(cipherText == "KIRAKSFJAN");
    }

    SECTION("Decryption (Keyed Deck)")
    {
        Cipher04 cipher;
        std::string plainText = cipher.decrypt("KIRAKSFJAN", "CRYPTONOMICON", false);
        REQUIRE(plainText == "SOLITAIREX");
    }

    SECTION("Round Trip (Keyed, Extended Deck)")
    {
        Cipher04 cipher;
        std::string passphrase = "P@55w0rd!~";
        std::string plainText = "Love everyone~Treat them kindly, too.";
        std::string cipherText = cipher.encrypt(plainText, passphrase);
        INFO("CipherText: " << cipherText);
        REQUIRE(cipherText.size() == 40);
        std::string decrypted = cipher.decrypt(cipherText, passphrase);
        INFO("PlainText: " << plainText);
        INFO("Decrypted: " << decrypted);
        for (int i=0; i<decrypted.size(); i++) {
            REQUIRE(decrypted[i] >= (int)' ');
            REQUIRE(decrypted[i] <= (int)'~');
        }
        for (int i=0; i<cipherText.size(); i++) {
            REQUIRE(cipherText[i] >= (int)' ');
            REQUIRE(cipherText[i] <= (int)'~');
        }
        REQUIRE(decrypted.size() == cipherText.size());
        REQUIRE(decrypted == PadText(plainText, kPaddingCharacter, kGroupSize));
    }

    SECTION("Round Trip (Unkeyed, Extended Deck)")
    {
        Cipher04 cipher;
        std::string plainText = "Love everyone~Treat them kindly, too.";
        std::string cipherText = cipher.encrypt(plainText, "");
        REQUIRE(cipherText.size() == 40);
        std::string decrypted = cipher.decrypt(cipherText, "");
        REQUIRE(decrypted.size() == 40);
        REQUIRE(decrypted == PadText(plainText, kPaddingCharacter, kGroupSize));
    }

    SECTION("Round Trip (Keyed, Extended, Random)")
    {
        Cipher04 cipher;
        for (int i=0; i<10; i++) {
            std::string passphrase = GetRandomString(i, true);
            std::string plainText = GetRandomString(i * 10, true);
            std::string cipherText = cipher.encrypt(plainText, passphrase);
            std::string decryptedText = cipher.decrypt(cipherText, passphrase);
            REQUIRE(decryptedText.substr(0, plainText.size()) == plainText);
        }
    }
}
