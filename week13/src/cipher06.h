/********************************************************************
* Header:
*    Implement your cipher here. You can view 'example.h' to see the
*    completed Caesar Cipher example.
********************************************************************/
#ifndef CIPHER06_H
#define CIPHER06_H

#include "cipher.h"
#include <string>
#include <sstream>

// Background: The Rail Fence algoithm was developed by the Greeks 
// and has been used for centuries. [1] It's usually used with a piece 
// of paper and a pencil. The cipher is created by taking the plain
// text and creating a railed fence pattern. The letters are then 
// taken row by row. The height of the fence is determined by a secret
// key. The key is used to encrypt and decrypt the message.

// Example of the message 'I'll be home for christmas' with a secret
// key of `5`

// I . . .   . . . h . . . . . . .   . . . . . . . a . . . .
// . ' . . . . .   . o . . . . . r . c . . . . . m . s . . .
// . . l . . . e . . . m . . . o . . . h . . . t . . . x . .
// . . . l . b . . . . . e . f . . . . . r . s . . . . . x .
// . . . .   . . . . . . .   . . . . . . . i . . . . . . . x

// Cypher Text: "I h a' orcmslemohtxlbefrsx  ix"
// Limitations to the algorithm are the key, the key cannot be very
// big. The bigger the key the less the message is scrambled.

/********************************************************************
 * CLASS
 *******************************************************************/
class Cipher06 : public Cipher
{
public:
   virtual std::string getPseudoAuth()  { return "Porter McGary"; }
   virtual std::string getCipherName()  { return "Rail Fence"; }
   virtual std::string getEncryptAuth() { return "Porter McGary"; }
   virtual std::string getDecryptAuth() { return "Porter McGary"; }

   /***********************************************************
    * GET CIPHER CITATION
    * Returns the citation from which we learned about the cipher
    ***********************************************************/
   virtual std::string getCipherCitation()
   {
      return std::string(
         "[1] PRZESTAWIENIOWY, SZYFR. “Crypto-It: Rail Fence Cipher.”"
         " Rail Fence Cipher | Transposition Ciphers,"
         " http://www.crypto-it.net/eng/simple/rail-fence-cipher.html.\n"
         "[2] Rail Fence Cipher.” Crypto Corner,"
         " https://crypto.interactive-maths.com/rail-fence-cipher.html.\n"
         "[3] “Rail Fence Cipher.” Wikipedia, Wikimedia Foundation, 28 Sept. 2021,"
         " https://en.wikipedia.org/wiki/Rail_fence_cipher.");
   }
   
   /**********************************************************
    * GET PSEUDOCODE
    * Returns the pseudocode as a string to be used by the caller.
    **********************************************************/
   virtual std::string getPseudocode()
   {
      std::string str("Rail Fence\n\n");

      // The encrypt pseudocode
      str += 
         "encrypt(plainText, rowNumber)\n"
         "  fenceTable = Array[rowNumber][plainText.length]\n"
         "  direction = 1\n"
         "  for row,column = 0, column < plaintext.legnth,  column++\n"
         "    fenceTable[row][column] = plaintext[column]\n"
         "    if (row == rowNumber - 1 && direction == 1) or (row == 0 && direction == -1)\n"
         "      direction = -direction\n"
         "    row = row + direction\n"
         "  for row = 0, row < rowNumber, row++\n"
         "    for index = 0, index < fenceTable[row].length, index++\n"
         "      cypherText += fenceTable[row][index]\n"
         "  return cypherText\n\n";

      // The decrypt pseudocode
      str +=
        "decrypt(cypherText, rowNumber)\n"
        "  spacing = 2 + ((n - 2) * 2\n"
        "  length = cypherText.length\n"
        "  table = Array[rowNumber][length] = '.'\n"
        "  table[row] = cypherText[0 to k] with spaces between letters\n"
        "  first, second = spacing\n"
        "  isFirst = false\n"
        "  for count, position, row = 0, count < length, count++\n"
        "    table[row][position] = cypherText[count]\n"
        "    pos += first or second\n"
        "    first is flipped\n"
        "    if position >= length\n"
        "      increase second unless == the spacing then = 2\n"
        "      decrease first unless == 2 then = spacing\n"
        "      position = ++row\n"
        "      isFirst = true\n"
        "  plainText = ''\n"
        "  for column, row = 0, direction = 1, column < length, column++\n"
        "    for i = 0, i < n, i++\n"
        "      plainText += table[row][column]\n"
        "      decide when the direction changes between -1, 0, 1\n"
        "      row += direction\n"
        "  return plainText\n";

      return str;
   }

   private: int convertPassword(const std::string & password, const int textLength) {
      // ensure that there is a valid password
      int secret = -1;
      std::stringstream stream(password);
      stream >> secret;
      
      // Password may not be a number that is greater than the max length 
      // and must be greater than 1
      assert(secret > 1);
      assert(secret < textLength);

      return secret;
   }

   /**********************************************************
    * ENCRYPT
    * Encrypts the user
    **********************************************************/
   virtual std::string encrypt(const std::string & plainText,
                               const std::string & password)
   {
      const int secret = convertPassword(password, plainText.length());

      return encrypt(plainText, secret);
   }

   std::string encrypt(const std::string & plainText, const int rowNumber) {
      // Table for encrypting
      const int length = plainText.length();
      std::string table[rowNumber][length];

      // filling the table
      for (int row = 0; row < rowNumber; row ++) {
         for (int column = 0; column < length; column++) {
            table[row][column] = ".";
         }
      }

      // direction to move up or down the columns
      int direction = 1;

      // Create the fence looking table
      for (int row = 0, column = 0; column < length; column++) {
         table[row][column] = plainText[column];
         if (((row == rowNumber - 1) && (direction == 1)) || 
             ((row == 0) && (direction == -1))) {
            direction = -direction;
         }
         row = row + direction;
      }

      // Cipher text
      std::string cypherText("");

      // read from the table row by row
      for (int row = 0; row < rowNumber; row++) {
         for (int index = 0; index < length; index++) {
            if (table[row][index] != ".") {
               cypherText += table[row][index];
            }
         }
      }

      return cypherText;
   }

   /**********************************************************
    * DECRYPT
    * Decrypt the cipher
    **********************************************************/
   virtual std::string decrypt(const std::string & cipherText,
                               const std::string & password)
   {
      const int secret = convertPassword(password, cipherText.length());
      
      return decrypt(cipherText, secret);
   }

   virtual std::string decrypt(const std::string & cypherText, const int n) {
      // Each has a unique initial spacing that changes by two, 
      // it starts from a base case of 2 from there each additional n needs to 
      // be multiplied by 2
      const int spacing = 2 + ((n - 2) * 2);
      const int length = cypherText.length();
      std::string table[n];

      // Set up the empty table or strings
      for (int i = 0; i < n; i++) {
         std::string str = "";
         for (int j = 0; j < length; j++) str.append(".");
         table[i] = str;
      }

      // initializing the table
      int first = spacing;
      int second = spacing;
      bool isFirst = true;
      // loop through and add the proper spacing to create the fence
      for (int counter = 0, pos = 0, row = 0; counter < length; counter++) {
         if (row < n) {
            table[row][pos] = cypherText[counter];
            
            pos += isFirst ? first : second;

            isFirst = !isFirst;
            
            if (pos >= length) {
               if (second == spacing) second = 2;
               else second += 2;
               if (first == 2) first = spacing;
               else first -= 2;
               pos = ++row;
               isFirst = true;
            }
         }
      }

      std::string plainText("");

      // Read from the fence along the fence
      for (int column = 0, row = 0, direction = 1; column < length; column++) {
         for (int i = 0; i < n; i++) {    
            if (table[row][column] != '.')        
               plainText += table[row][column];

            if ((row + 1 == n) && (direction == 1)) direction = 0;
            else if ((direction == 0) && (row == 0)) direction = 1;
            else if ((direction == 0) && (row + 1 == n)) direction = -1;
            else if ((row == 0) && (direction == -1)) direction = 0;

            row += direction;
         }
      }

      return plainText;
   }
};

#endif // CIPHER06_H