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

// Enables logging useful in debugging
#define DEBUG false
#define DEBUG_LOG(...) if (DEBUG) { printf(__VA_ARGS__); }

// Background: The Solitaire encryption algorithm [1] was designed by
// Bruce Schneier in 1999 in collaboration with the author Neal
// Stephensen, who was writing "Cryptonomicon" at the time. Bruce was
// challenged with coming up with a truly secure system of encryption
// that could be done by hand while still standing up to the best
// code breakers and most advanced computers. He used a deck of cards
// as the source of entropy (by shuffling the deck or keying the deck
// with a specific sequence), the method of operation for the cipher,
// and to chain each cipher. It is surprisingly strong and has stood
// up well to rigorous review. There is a small reduction in entropy
// due to a small bias toward consecutive character repetition that
// could allow for attacks if the message size is sufficiently large.
// Cryptanalysis shows that the cipher leaks information at a rate of
// 0.0005 bits per character [2]. Given that the original cipher is
// intended to be implemented manually (by hand), it follows that
// message size will be somewhat limited, significantly reducing the
// impact of this bias.
//
// Like all encryption, the Solitaire cipher is only as good as the
// key. In Solitaire, the deck itself is the key. A shuffled, fully
// randomized deck is ideal but the Solitaire agorithm itself can be
// used to "key" a deck using a passphrase. That process is included
// in the implementation below.
//
// References:
//
// [1] B. Schneier, “The Solitaire Encryption Algorithm,”
//     Schneier on Security, 26-May-1999.
//     https://www.schneier.com/academic/solitaire/.
// [2] W. Tounsi, B. Justus, N. C. Boulahia, F. Cuppen and J. G. Alfaro,
//     "Probabilistic Cycle Detection for Schneier's Solitaire Keystream
//      Algorithm," 2014 IEEE Eighth International Conference on Software
//     Security and Reliability-Companion, 2014, pp. 113-120,
//     doi: 10.1109/SERE-C.2014.29.

// Note: The original Solitaire algorithm supports only 26 upper-case
// English letters (26 characters). In order to support the larger
// character set specified in the lab, we increase the size of each of
// the 4 suits in the deck from 13 to 24, giving us enough cards to
// represent all 95 required characters. So far as I can tell, the
// nature of the Solitaire algorithm itself is not impacted. You will
// see the 'extended' variable passed throughout the implmentation;
// this is to allow testing of both the original Solitaire algorithm
// (for a more formal black-box validation) using a standard deck and
// the "extended" version, using the larger deck size.

// Another note: The following code is a strict duplication of the
// "by hand" Solitaire algorithm - meaning that the implementation
// creates a virtual deck with virtual cards and performs all
// operations strictly as prescribed for a physical deck. It is
// entirely possible to implement this algorithm without resorting to
// a virtual deck. Doing so is much, much simpler. It is, however,
// far less interesting and more challenging to validate against the
// original description.

// Deck-related constants
static int  kSuitSize = 13;
static int  kSuitSizeExtended = 24; // To support 96 characters
static int  kSuitNum = 4;
static char kValidSuits[] = { 'C', 'D', 'H', 'S', 'J' };
static char kJokerSuit = 'J';
static int  kJokerNum = 2;

// Cipher-specific constants
// Messages are grouped and padded to the following group size
#define kGroupSize 5
// The following is used to pad messages up to the group size
#define kPaddingCharacter 'X'
// The number of allowed plaintext characters
#define kCharacterSetSize (int)'Z' - (int)'A' + 1
#define kCharacterSetSizeExtended (int)'~' - (int)' ' + 1

// Return the correct suit size
static int SuitSize(bool extended) {
   return extended ? kSuitSizeExtended : kSuitSize;
}

// Return the calculated deck size
static int DeckSize(bool extended) {
   return SuitSize(extended) * kSuitNum + kJokerNum;
}

// Utility method to convert a character to its integer value,
// according to the Solitaire algorithm.
static int IntValueOfCharacter(const char &c, bool extended = false) {
   if (extended) {
      // Anything from SPACE (32) to ~ (126)
      return (int)c - ((int)' ' - 1);
   }

   // Must be a valid English letter - and only upper case!
   assert(((int)c >= (int)'A' && (int)c <= (int)'Z') ||
          ((int)c >= (int)'a' && (int)c <= (int)'z'));
   char u = toupper(c);
   return (int)u - ((int)'A' - 1);
}

// Utility method to convert an integer to the corresponding
// character, according to Solitaire.
static char CharacterValueOfInt(int v, bool extended = false) {
   if (extended) {
      return (char)v + ((int)' ' - 1);
   }

   // Must become a valid English letter
   int cv = v + ((int)'a' - 1);
   assert((cv >= (int)'A' && cv <= int('Z')) ||
          (cv >= (int)'a' && cv <= int('z')));
   char c = (char)cv;
   return toupper(c);
}

// Mixes the provided key with the specified character to
// generate the encrypted output.
static char EncryptCharacter(int key, const char pt, bool extended = false)
{
   int characterSetSize = extended ? kCharacterSetSizeExtended : kCharacterSetSize;
   int v = IntValueOfCharacter(pt, extended);
   int c = v + key;
   if (c > characterSetSize) {
      c -= characterSetSize;
   }

   return CharacterValueOfInt(c, extended);
}

// Unmixes the provided key from the specified character to
// generate the original plaintext.
static char DecryptCharacter(int key, const char ct, bool extended = false)
{
   int characterSetSize = extended ? kCharacterSetSizeExtended : kCharacterSetSize;
   int v = IntValueOfCharacter(ct, extended);
   int c = (v - key);
   if (c < 1) {
      c += characterSetSize;
   } else if (c > characterSetSize) {
      c -= characterSetSize;
   }

   return CharacterValueOfInt(c, extended);
}

// Pads the provided plaintext to the appropriate size, based
// on the configured group size, using the specified padding
// character.
static std::string PadText(const std::string &plainText,
                           const char paddingCharacter,
                           int groupSize)
{
   std::string plainTextCp(plainText);
   int remainder = plainText.size() % groupSize;
   for (int i = 0; i < groupSize - remainder; i++) {
      plainTextCp.push_back(paddingCharacter);
   }

   return plainTextCp;
}

/********************************************************************
 * CLASS Card
 * Represents a card in the deck.
 *******************************************************************/
struct Card
{
   // The card's integer value (Ace = 1, 10 = 10, Jack = 11, etc.)
   int value;
   // The card's suit (Clubs, Spades, etc.)
   char suit;

   Card(bool extended = false)
      : value(1),
        suit('C'),
        extended(extended),
        suitSize(SuitSize(extended)),
        deckSize(DeckSize(extended))
   {}

   Card(int value, char suit, bool extended = false)
      : value(value),
        suit(suit),
        extended(extended),
        suitSize(SuitSize(extended)),
        deckSize(DeckSize(extended))
   {}

   Card(Card &other)
   {
      this->value = other.value;
      this->suit = other.suit;
      this->extended = other.extended;
      this->suitSize = other.suitSize;
      this->deckSize = other.deckSize;
   }

   // The absolute integer representation of the card, according to
   // Solitaire. Example:
   //   'C1' (Ace of Clubs) == 1
   //   'D3' (3 of Diamonds) == ...
   int intValue()
   {
      for (int i = 0; i < kSuitNum; i++) {
         if (suit == kValidSuits[i]) {
            return value + (suitSize * i);
         }
      }

      if (suit == 'J') {
         return deckSize - 1;
      }

      assert(false);
      return -1;
   }

   // Human-readable representation of the card. Example:
   //   'C1' (Ace of Clubs)
   std::string stringValue()
   {
      assert(value <= suitSize);
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
            assert(extended);
            assert(value <= deckSize);
            // For the extended deck, just use a lowercase
            // letter to represent anthing > 13 (K).
            os << (char)(value - (kSuitSize + 1) + (int)'a');
      }

      return os.str();
   }

   // The letter representation of the card, according to
   // Solitaire.
   char letterValue()
   {
      assert(suit != 'J');
      if (suit == 'C' || suit == 'H')
      {
         return CharacterValueOfInt(value, extended);
      }

      return CharacterValueOfInt(value + suitSize, extended);
   }

   // Comparison operator
   bool operator ==(Card other)
   {
      return (suit == other.suit && value == other.value);
   }

   // Factory method to create a Card using the provided
   // integer value (see IntValueOfCharacter and CharacterValueOfInt).
   static Card cardFromIntValue(int intValue, bool extended = false)
   {
      int deckSize = DeckSize(extended);
      int suitSize = extended ? kSuitSizeExtended : kSuitSize;

      assert(intValue > 0 && intValue <= deckSize);

      int v;
      char s;

      // Handle Jokers
      if (intValue >= deckSize - 1) {
         v = intValue % (deckSize - 1);
         s = kJokerSuit;
         return Card(v, s, extended);
      }

      v = ((intValue - 1) % suitSize) + 1;
      int sOffset = (intValue - 1) / suitSize;
      s = kValidSuits[sOffset];

      return Card(v, s, extended);
   }

   static Card * newCardFromIntValue(int intValue, bool extended = false)
   {
      return new Card(cardFromIntValue(intValue, extended));
   }

private:
   bool extended;
   int suitSize;
   int deckSize;
};

/********************************************************************
 * CLASS Deck
 * Represents a deck of cards
 *******************************************************************/
class Deck
{
public:
   Deck(bool extended = false)
      : extended(extended),
        deckSize(DeckSize(extended))
   {
      // Init with a sorted deck (bridge order)
      _deck.reserve(deckSize);
      for (int i = 0; i < deckSize; i++) {
         Card *c = Card::newCardFromIntValue(i + 1, extended);
         _deck[i] = c;
      }
   }

   ~Deck()
   {
      for (int i=0; i<deckSize; i++) {
         delete(_deck[i]);
      }
   }

   // The core of the Solitaire algorithm. This sequence of
   // operations on the deck are used to derive the key stream
   // and encrypted characters.
   void solitaire()
   {
      // Move Joker A down 1
      int jokerAOffset = getPosition(deckSize - 1);
      move(jokerAOffset, (jokerAOffset + 1) % deckSize);
      DEBUG_LOG("Solitaire (move 1): %s\n", toString().c_str());

      // Move Joker B down 2
      int jokerBOffset = getPosition(deckSize);
      DEBUG_LOG("Solitaire (move 2): Move %d to %d\n", jokerBOffset, (jokerBOffset + 2) % deckSize);
      move(jokerBOffset, (jokerBOffset + 2) % deckSize);
      DEBUG_LOG("Solitaire (move 2): %s\n", toString().c_str());

      // Triple cut deck based on Joker positions
      jokerAOffset = getPosition(deckSize - 1);
      jokerBOffset = getPosition(deckSize);
      tripleCut(std::min(jokerAOffset, jokerBOffset), std::max(jokerAOffset, jokerBOffset));
      DEBUG_LOG("Solitaire (triple cut): %s\n", toString().c_str());

      // Cut at the value specified by the bottom card,
      // leaving the bottom card at the bottom
      Card *bottomCard = getCard(deckSize - 1);
      cut(bottomCard->intValue(), true);
      DEBUG_LOG("Solitaire (cut @ %d): %s\n", bottomCard->intValue(), toString().c_str());
   }

   // Generate a key stream of the specified length using the
   // Solitaire algorithm. This key stream is the Solitaire
   // equivalent of the encryption key as well as the values
   // from which the ciphertext is produced.
   std::vector<int> keyStream(int length)
   {
      std::vector<int> ks;

      for (int i = 0; i < length; i++) {
         solitaire();
         Card *top = getCard(0);
         Card *letterCard = getCard(top->intValue());
         while(letterCard->intValue() == deckSize - 1 /* Joker */)
         {
            solitaire();
            top = getCard(0);
            letterCard = getCard(top->intValue());
         }

         DEBUG_LOG("Key stream (top card): %c%d\n", top->suit, top->value);
         DEBUG_LOG("Key stream (letter card): %c%d\n", letterCard->suit, letterCard->value);
         DEBUG_LOG("Key stream: %d (%d)\n", letterCard->letterValue(), letterCard->intValue());
         ks.push_back(IntValueOfCharacter(letterCard->letterValue(), extended));
      }

      return ks;
   }

   // Key (arrange / sort) the deck using the specified
   // passphrase. This is simpler than memorizing the order
   // of the entire deck, though it is better to use a fully
   // randomized deck.
   void key(const std::string &passphrase)
   {
      for (const char &c : passphrase) {
         solitaire();
         int v = IntValueOfCharacter(c, extended);
         cut(v, true);
      }
   }

   // Swap two cards in the deck
   void swap(int position1, int position2)
   {
      assert(position1 >= 0 && position1 <= deckSize);
      assert(position2 >= 0 && position2 <= deckSize);
      if (position1 == position2) {
         return; // Nothing to do
      }

      Card *orig = _deck[position1];
      _deck[position1] = _deck[position2];
      _deck[position2] = orig;
   }

   // Move a card from position1 to position2. Note that this
   // is not the same as a swap; when moving a card in a
   // physical deck, all adjacent cards move as well. That is
   // essential and included here.
   void move(int position1, int position2)
   {
      assert(position1 >= 0 && position1 <= deckSize);
      assert(position2 >= 0 && position2 <= deckSize);
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

   // Cut the deck, optionally keeping the last card in the
   // same position. While that is an unusual cut, it is what
   // the Solitaire algorithm requires.
   void cut(int offset, bool keepBottomCard = false)
   {
      int dSize = deckSize;

      if (keepBottomCard) {
         dSize = deckSize - 1;
      }

      std::vector<Card *> cutDeck;
      cutDeck.reserve(deckSize);

      for (int i = offset; i < dSize + offset; i++)
      {
         cutDeck.push_back(_deck[i % dSize]);
      }

      _deck = cutDeck;
   }

   // Perform a 'triple' cut: swap all cards above offset1
   // with those below offset2.
   void tripleCut(int offset1, int offset2)
   {
      assert(offset1 < offset2);
      assert(offset1 > 0);
      assert(offset2 < deckSize);

      std::vector<Card *> cutDeck;
      cutDeck.reserve(deckSize);

      // Move the end block to the front
      for (int i = offset2 + 1; i < deckSize; i++) {
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

   // Find the card matching the specified card in the deck.
   int getPosition(Card card)
   {
      for (int i = 0; i < deckSize; i++) {
         if (*(_deck[i]) == card) {
            return i;
         }
      }
      return -1;
   }

   // Convenience call to find a card by intValue in the deck.
   int getPosition(int intValue)
   {
      Card c = Card::cardFromIntValue(intValue, extended);
      return getPosition(c);
   }

   // Return the card at the specified offset in the deck.
   Card * getCard(int offset) { return _deck[offset]; }

   // Returns a string representation of the deck, suitable
   // for printing to the screen.
   std::string toString()
   {
      std::ostringstream os;
      for (int i = 0; i < deckSize; i++) {
         os << i << ": " << _deck[i]->stringValue() << std::endl;
      }

      return os.str();
   }

private:
   std::vector<Card *> _deck;
   bool extended;
   int deckSize;
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
      return std::string(
         "[1] B. Schneier, “The Solitaire Encryption Algorithm,”"
         " Schneier on Security, 26-May-1999."
         " https://www.schneier.com/academic/solitaire/.");
   }
   
   /**********************************************************
    * GET PSEUDOCODE
    * Returns the pseudocode as a string to be used by the caller.
    **********************************************************/
   virtual std::string getPseudocode()
   {
      std::string str("Solitaire\n\n");

      // Common pseudocode used in both encrypt/decrypt:
      str +=
         "Common:\n\n"
         "solitaire(keyedDeck) {\n"
         "    deck = moveCard(keyedDeck, jokerA, 1)\n"
         "    deck = moveCard(deck, jokerB, 2)\n"
         "    topJoker = findFirst(deck, [jokerA, jokerB])\n"
         "    bottomJoker = [jokerA, jokerB].remove(topJoker)[0]\n"
         "    deck = tripleCut(deck, topJoker, bottomJoker)\n"
         "    bottomCard = deck.last\n"
         "    cut(deck[0..-1], numberValueOf(bottomCard))\n"
         "}\n"
         "\n"
         "getKeyStreamLetters(count, passphrase) {\n"
         "    keyDeck(passphrase)\n"
         "    keystream = []\n"
         "    for offset in count {\n"
         "        keystream[offset] = getKeyStreamLetter(deck)\n"
         "    }\n"
         "}\n"
         "\n"
         "keyDeck(passphrase) {\n"
         "    for letter in passphrase {\n"
         "        num = numberValueOf(letter)\n"
         "        deck = solitaire(deck)\n"
         "        cut(deck[0..-1], num)\n"
         "    }\n"
         "}\n"
         "\n"
         "getKeyStreamLetter(deck) {\n"
         "    topCard = deck[0]\n"
         "    topNum = numberValueOf(topCard)\n"
         "    if [jokerA, jokerB].include?(topCard) {\n"
         "        solitaire(deck)\n"
         "        return getKeyStreamLetter(deck)\n"
         "    }\n"
         "    return numberValueOf(deck[topNum])\n"
         "}\n\n";

      // The encrypt pseudocode
      str +=
         "Encrypt:\n\n"
         "groupedTxt = groupLetters(plainTxt, groupLength)\n"
         "txtNumbers = convertToNumbers(groupedTxt)\n"
         "ksLetters = getKeyStreamLetters(groupedTxt.length)\n"
         "ksNumbers = getKeyStreamNumbers(ksLetters)\n"
         "cipherNums = []\n"
         "mixInKeyStream(txtNumbers, ksNumbers) {\n"
         "    for offset in txtNumbers.count {\n"
         "        cipherNums[offset] = (txtNumbers[offset] + ksNumbers[offset]) % 26\n"
         "    }\n"
         "}\n"
         "return convertNumsToLetters(cipherNums)\n\n";

      // The decrypt pseudocode
      str +=
         "Decrypt:\n\n"
         "ksLetters = getKeyStreamLetters(cipherTxt)\n"
         "ksNumbers = getKeyStreamNumbers(ksLetters)\n"
         "cipherNums = convertLettersToNums(cipherTxt)\n"
         "plainNums = []\n"
         "for offset in cipherNums.count {\n"
         "    plainNums[offset] = (cipherNums[offset] - ksNumbers[offset]) % 26\n"
         "}\n";

      return str;
   }

   /**********************************************************
    * ENCRYPT
    * TODO: ADD description
    **********************************************************/
   virtual std::string encrypt(const std::string & plainText,
                               const std::string & password)
   {
      return encrypt(plainText, password, true);
   }

   std::string encrypt(const std::string & plainText,
                       const std::string & password,
                       bool extended)
   {
      // We cast pretty much everything to int, so:
      assert(plainText.size() < INT_MAX);
      assert(password.size() < INT_MAX);

      Deck deck = Deck(extended);
      deck.key(password);

      std::string plainTextCp = PadText(plainText, kPaddingCharacter, kGroupSize);
      std::vector<int> keyStream = deck.keyStream(plainTextCp.size());
      std::ostringstream cipherText;
      for (int i = 0; i < plainTextCp.size(); i++) {
         cipherText << EncryptCharacter(keyStream[i], plainTextCp[i], extended);
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
      return decrypt(cipherText, password, true);
   }

   std::string decrypt(const std::string & cipherText,
                       const std::string & password,
                       bool extended)
   {
      // We cast pretty much everything to int, so:
      assert(cipherText.size() < INT_MAX);
      assert(password.size() < INT_MAX);

      Deck deck = Deck(extended);
      deck.key(password);

      std::vector<int> keyStream = deck.keyStream(cipherText.size());
      std::ostringstream plainText;
      for (int i = 0; i < cipherText.size(); i++) {
         plainText << DecryptCharacter(keyStream[i], cipherText[i], extended);
      }

      return plainText.str();
   }
};

#endif // CIPHER04_H
