/********************************************************************
* Header:
*    Implement your cipher here. You can view 'example.h' to see the
*    completed Caesar Cipher example.
********************************************************************/
#ifndef CIPHER04_H
#define CIPHER04_H

#include "cipher.h"

#include <sstream>
#include <string>

#define DEBUG false
#define DEBUG_LOG(...) if (DEBUG) { printf(__VA_ARGS__); }

// Note: We need to support 94 characters (ASCII 32..126)

static int kDeckSize = 54;
static int kSuitSize = 13;
static char kValidSuits[] = { 'C', 'D', 'H', 'S', 'J' };

#define kGroupSize 5
#define kPaddingCharacter 'X'

static int IntValueOfLetter(const char &c) {
   // Must be a valid English letter
   assert(((int)c >= 65 && (int)c <= 90) ||
          ((int)c >= 97 && (int)c <= 122));
   char u = toupper(c);
   return (int)u - 64;
}

static char CharacterValueOfInt(int v) {
   // Must become a valud English letter
   int cv = v + 64;
   assert((cv >= 65 && cv <= 90) ||
          (cv >= 97 && cv <= 122));
   char c = (char)cv;
   return toupper(c);
}

struct Card
{
   int value;
   char suit;

   Card()
      : value(1), suit('C')
   {}

   Card(int value, char suit)
   {
      this->value = value;
      this->suit = suit;
   }

   Card(Card &other)
   {
      this->value = other.value;
      this->suit = other.suit;
   }

   int intValue()
   {
      switch(suit) {
         case 'C':
            return value + 0;
         case 'D':
            return value + kSuitSize;
         case 'H':
            return value + (kSuitSize * 2);
         case 'S':
            return value + (kSuitSize * 3);
         case 'J':
            return kDeckSize - 1;
         default:
            assert(false);
            return -1;
      }
   }

   std::string stringValue()
   {
      assert(value <= kSuitSize);
      if (suit == 'J') {
         assert(value <= 2);
      }

      std::ostringstream os;
      os << suit;

      // Number cards / Jokers
      if (value < 11) {
         os << value;
         return os.str();
      }

      // Face cards
      switch(value) {
         case 11:
            os << 'J';
            break;
         case 12:
            os << 'Q';
            break;
         case 13:
            os << 'K';
            break;
         default:
            assert(false);
      }

      return os.str();
   }

   char letterValue()
   {
      assert(suit != 'J');
      if (suit == 'C' || suit == 'H')
      {
         return CharacterValueOfInt(value);
      }

      return CharacterValueOfInt(value + 13);
   }

   bool operator ==(Card other)
   {
      return (suit == other.suit && value == other.value);
   }

   static Card cardFromIntValue(int intValue)
   {
      assert(intValue > 0 && intValue <= kDeckSize);

      int v;
      char s;

      // Handle Jokers
      if (intValue >= kDeckSize - 1) {
         v = intValue % (kDeckSize - 1);
         s = kValidSuits[4];
         return Card(v, s);
      }

      v = ((intValue - 1) % kSuitSize) + 1;
      int sOffset = (intValue - 1) / kSuitSize;
      s = kValidSuits[sOffset];

      return Card(v, s);
   }

   static Card * newCardFromIntValue(int intValue)
   {
      return new Card(cardFromIntValue(intValue));
   }
};

/********************************************************************
 * CLASS
 *******************************************************************/
class Deck
{
public:
   Deck()
   {
      // Init with a sorted deck
      _deck.reserve(kDeckSize);
      for (int i = 0; i < kDeckSize; i++) {
         Card *c = Card::newCardFromIntValue(i + 1);
         _deck[i] = c;
      }
   }

   ~Deck()
   {
      for (int i=0; i<kDeckSize; i++) {
         delete(_deck[i]);
      }
   }

   void shuffle() {}

   void solitaire()
   {
      // Move Joker A down 1
      int jokerAOffset = getPosition(kDeckSize - 1);
      move(jokerAOffset, (jokerAOffset + 1) % kDeckSize);
      DEBUG_LOG("Solitaire (move 1): %s\n", toString().c_str());

      // Move Joker B down 2
      int jokerBOffset = getPosition(kDeckSize);
      DEBUG_LOG("Solitaire (move 2): Move %d to %d\n", jokerBOffset, (jokerBOffset + 2) % kDeckSize);
      move(jokerBOffset, (jokerBOffset + 2) % kDeckSize);
      DEBUG_LOG("Solitaire (move 2): %s\n", toString().c_str());

      // Triple cut deck based on Joker positions
      jokerAOffset = getPosition(kDeckSize - 1);
      jokerBOffset = getPosition(kDeckSize);
      tripleCut(std::min(jokerAOffset, jokerBOffset), std::max(jokerAOffset, jokerBOffset));
      DEBUG_LOG("Solitaire (triple cut): %s\n", toString().c_str());

      // Cut at the value specified by the bottom card,
      // leaving the bottom card at the bottom
      Card *bottomCard = getCard(kDeckSize - 1);
      cut(bottomCard->intValue(), true);
      DEBUG_LOG("Solitaire (cut @ %d): %s\n", bottomCard->intValue(), toString().c_str());
   }

   std::vector<int> keyStream(int length)
   {
      std::vector<int> ks;

      for (int i = 0; i < length; i++) {
         solitaire();
         Card *top = getCard(0);
         Card *letterCard = getCard(top->intValue());
         while(letterCard->intValue() == 53 /* Joker */)
         {
            solitaire();
            top = getCard(0);
            letterCard = getCard(top->intValue());
         }

         DEBUG_LOG("Key stream (top card): %c%d\n", top->suit, top->value);
         DEBUG_LOG("Key stream (letter card): %c%d\n", letterCard->suit, letterCard->value);
         DEBUG_LOG("Key stream: %d (%d)\n", letterCard->letterValue(), letterCard->intValue());
         ks.push_back(IntValueOfLetter(letterCard->letterValue()));
      }

      return ks;
   }

   void key(const std::string &passphrase)
   {
      for (const char &c : passphrase) {
         solitaire();
         int v = IntValueOfLetter(c);
         cut(v, true);
      }
   }

   void swap(int position1, int position2)
   {
      assert(position1 >= 0 && position1 <= kDeckSize);
      assert(position2 >= 0 && position2 <= kDeckSize);
      if (position1 == position2) {
         return; // Nothing to do
      }

      Card *orig = _deck[position1];
      _deck[position1] = _deck[position2];
      _deck[position2] = orig;
   }

   void swap(Card *card, int offset)
   {
      int current = getPosition(card);
      assert(current >= 0);
      swap(current, offset);
   }

   void move(int position1, int position2)
   {
      assert(position1 >= 0 && position1 <= kDeckSize);
      assert(position2 >= 0 && position2 <= kDeckSize);
      if (position1 == position2) {
         return; // Nothing to do
      }

      if (position1 < position2) {
         for (int i = position1 + 1; i <= position2; i++) {
            swap(i - 1, i);
         }
      } else {
         for (int i = position1 - 1; i > position2; i--) {
            swap(i + 1, i);
         }
      }
   }

   void move(Card *card, int offset)
   {
      int current = getPosition(card);
      assert(current >= 0);
      move(current, offset);
   }

   void cut(int offset, bool keepBottomCard = false)
   {
      int deckSize = kDeckSize;

      if (keepBottomCard) {
         deckSize = kDeckSize - 1;
      }

      std::vector<Card *> cutDeck;
      cutDeck.reserve(kDeckSize);

      for (int i = offset; i < deckSize + offset; i++)
      {
         cutDeck.push_back(_deck[i % deckSize]);
      }

      _deck = cutDeck;
   }

   void tripleCut(int offset1, int offset2)
   {
      assert(offset1 < offset2);
      assert(offset1 > 0);
      assert(offset2 < kDeckSize);

      std::vector<Card *> cutDeck;
      cutDeck.reserve(kDeckSize);

      // Move the end block to the front
      for (int i = offset2 + 1; i < kDeckSize; i++) {
         cutDeck.push_back(_deck[i]);
      }
      // Keep the center block the same
      for (int i = offset1; i <= offset2; i++) {
         cutDeck.push_back(_deck[i]);
      }
      // Move the beginning block to the end
      for (int i = 0; i < offset1; i++) {
         cutDeck.push_back(_deck[i]);
      }

      _deck = cutDeck;
   }

   int getPosition(Card card)
   {
      for (int i = 0; i < kDeckSize; i++) {
         if (*(_deck[i]) == card) {
            return i;
         }
      }
      return -1;
   }

   int getPosition(Card *card)
   {
      return getPosition(*card);
   }

   int getPosition(int intValue)
   {
      Card c = Card::cardFromIntValue(intValue);
      return getPosition(c);
   }

   Card * getCard(int offset) { return _deck[offset]; }

   std::string toString()
   {
      std::ostringstream os;
      for (int i = 0; i < kDeckSize; i++) {
         os << i << ": " << _deck[i]->stringValue() << std::endl;
      }

      return os.str();
   }

private:
   std::vector<Card *> _deck;
};

/********************************************************************
 * CLASS
 *******************************************************************/
class Cipher04 : public Cipher
{
public:
   virtual std::string getPseudoAuth()  { return "Tom Metge"; }
   virtual std::string getCipherName()  { return "Solitaire"; }
   virtual std::string getEncryptAuth() { return "Tom Metge"; }
   virtual std::string getDecryptAuth() { return "Tom Metge"; }

   /***********************************************************
    * GET CIPHER CITATION
    * Returns the citation from which we learned about the cipher
    ***********************************************************/
   virtual std::string getCipherCitation()
   {
      return std::string("citation");
   }
   
   /**********************************************************
    * GET PSEUDOCODE
    * Returns the pseudocode as a string to be used by the caller.
    **********************************************************/
   virtual std::string getPseudocode()
   {
      std::string str;

      // TODO: please format your pseudocode
      // The encrypt pseudocode
      str =  "insert the encryption pseudocode\n";

      // The decrypt pseudocode
      str += "insert the decryption pseudocode\n";

      return str;
   }

   /**********************************************************
    * ENCRYPT
    * TODO: ADD description
    **********************************************************/
   virtual std::string encrypt(const std::string & plainText,
                               const std::string & password)
   {
      Deck deck;
      deck.key(password);

      // Ensure plain text length is a multiple of 5 (our group size),
      // padding with 'X' as needed.
      std::string plainTextCp(plainText);
      int remainder = plainText.size() % kGroupSize;
      for (int i = 0; i < kGroupSize - remainder; i++) {
         plainTextCp.push_back(kPaddingCharacter);
      }

      std::vector<int> keyStream = deck.keyStream(plainTextCp.size());
      std::ostringstream cipherText;
      for (int i = 0; i < plainTextCp.size(); i++) {
         int v = IntValueOfLetter(plainTextCp[i]);
         int c = v + keyStream[i];
         if (c > 26) {
            c -= 26;
         }
         cipherText << CharacterValueOfInt(c);
      }

      return cipherText.str();
   }

   /**********************************************************
    * DECRYPT
    * TODO: ADD description
    **********************************************************/
   virtual std::string decrypt(const std::string & cipherText,
                               const std::string & password)
   {
      Deck deck;
      deck.key(password);

      std::vector<int> keyStream = deck.keyStream(cipherText.size());
      std::ostringstream plainText;
      for (int i = 0; i < cipherText.size(); i++) {
         int v = IntValueOfLetter(cipherText[i]);
         int c = (v - keyStream[i]);
         if (c < 0) {
            c += 26;
         } else if (c > 26) {
            c -= 26;
         }
         plainText << CharacterValueOfInt(c);
      }

      return plainText.str();
   }
};

#endif // CIPHER04_H