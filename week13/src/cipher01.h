/********************************************************************
* Header:
*     Cipher01
*    Date Shift Cipher
********************************************************************/
#ifndef CIPHER01_H
#define CIPHER01_H

//Include some libraries
#include <iostream>
#include <string>
#include <list>
using namespace std;//namespace

//BRIEF DESCRIPTION
//The Date Shift cipher is a much harder code to break than the simpler Shift Cipher. This is because the
//Date Shift number key varies from letter to letter, and also because it’s polyalphabetic (this means that a number or
//letter can represent multiple letters).
//This model is based 90% in date shift cipher and I adapted for this assignment an 10%.I added a new functionality 
// Convert letters (CAPS and lower) and some symbols in numbers too
// The original just allow numbers this version allow letter and symbols too.

// Why did I choose this cipher?
// I like it so much programming and it is a very fun challenge for me
// I never programming a cipher so I was excited about this opportunity
// Searching by a cipher I founded Date Shift Cipher and for me was a great opportunity to add a little bit original
// Converting a Date Shift Cipher stronger using letters and symbols too.
// I would like if this cipher has a name, because I would like to give it one: D

//Declare global variables
string alphabet = "0123456789abcdefghijklmnopqrstuvwxyz !.,\"@-_?=[]{}'/^&*+%~`#$|:<>ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/*********************************************************
 * CONVERT TEXT TO NUMBERS
 * This function work convertir text in numbers assign it
 * a number to each letter
 * *******************************************************/
list <int> convertTexttoNumbers(string & text){

//Declare a list 
list<int> numval;

//Using for to convert each letter in number using a double for 
for (int i = 0; i < text.size(); ++i)
{
   for (int a = 0; a < alphabet.size(); ++a)
   {  
      //If there are the same
      if(text[i] == alphabet[a]){
         numval.push_back(a);//add to list
      }
   }
}

//return a list with every letter converted in number
return numval;

}

/**************************************************************************
   * ENCRYPT CIPHER NUMBERS
   * This function take the numbers of each letter and sum it 
   * This obtain random number letter using a password
****************************************************************************/
list <int> encryptCipherNumbers(list<int> &textN, list<int> &passN){
   
   //Declare variables
   list<int> :: iterator x; //iterator
   list<int> :: iterator y;
   list<int> cipherNumber;//result of sum the two list

   //A for loop to sum the two list and obtain a cipherNumber
    for(x = textN.begin(), y = passN.begin(); x != textN.end(); x++,y++){
         
         //If the text if greater than password repeat the password to continue 
         if(y == passN.end()){
         y = passN.begin();//Reset password to begin   
         }

         cipherNumber.push_back(*x+*y);//Sum the two list and save in a list
    }

    return cipherNumber;//return the new list
}

/********************************************************
 * ENCRYPT TO TEXT
 * This function has two parts
 * First: Encrypt the number letter
 * Second: Using a for converts the number in cipher letter
*********************************************************/
string encryptToText(list <int> cipherNumber){

   //Declare variables
   list <int>:: iterator x;
   list <int>::iterator y;
   list <int> sum;
   string result;

   //A for loop to check if there not negative number and do the swift
   for (x = cipherNumber.begin(); x != cipherNumber.end(); x++)
   {
      //If the result is negative multipled by -1
      if(*x > (alphabet.size() - 1))
      sum.push_back(*x - (alphabet.size() - 1));
      else sum.push_back(*x);//just save
   
   }

   //A for loop to convert the number in letter again but cipher
   for (y= sum.begin(); y != sum.end(); y++)
   {
      for (int z = 0; z < alphabet.size(); ++z)
      {
         if(*y == z){
          result += alphabet[z];  
         }
      }
   }

   return result;//Return cipherText
}

/*********************************************************************
   * DECRYPT CIPHER NUMBERS
   * This function decrypt the numbers using a password
**********************************************************************/
list <int> decryptCipherNumbers(list<int> &textN, list<int> &passN){
   
   //Declare variables
   list<int> :: iterator x;
   list<int> :: iterator y;
   list<int> cipherNumber;
   int substract = 0;

   //A for loop to substact the two list and obtain a decrypt cipherNumber
    for(x = textN.begin(), y = passN.begin(); x != textN.end(); x++,y++){
         
         if(y == passN.end()){
         y = passN.begin();   
         }

         substract = *x - *y; //substract the two list plaintext - password

         cipherNumber.push_back(substract); //save in a list
    }

    return cipherNumber;//return a list with the decrypt cipher number
}

/***************************************************
 * DECRYPT TO TEXT
 * This function transform the numbers 
 * and assign it the correspond letter of alphabet
*****************************************************/
string decryptToText(list <int> cipherNumber){

   //Declare variables
   list <int>:: iterator x;
   list <int>::iterator y;
   list <int> sum;
   string result;
   

  //A for loop to check if there not negative number and do the swift
   for (x = cipherNumber.begin(); x != cipherNumber.end(); x++)
   {
      //If the result is negative multipled by -1
      if(*x < 0){
      sum.push_back((*x + (alphabet.size() - 1)));
      }

      else{ 
         sum.push_back(*x);//just save it
      
      }
      
   
   }
   //A for loop to convert the number in letter again but plainText(easy to read)
   for (y= sum.begin(); y != sum.end(); y++)
   {
      for (int z = 0; z < alphabet.size(); ++z)
      {
         if(*y == z){
          result += alphabet[z];//save in a string  
         }
      }
   }

   return result; //return PlainText
}

/********************************************************************
 * CLASS
 *******************************************************************/
class Cipher01 : public Cipher
{
public:
   virtual std::string getPseudoAuth()  { return "Marvin H. Zavala"; }
   virtual std::string getCipherName()  { return "Date Shift Cipher"; }
   virtual std::string getEncryptAuth() { return "Marvin H. Zavala"; }
   virtual std::string getDecryptAuth() { return "Marvin H. Zavala"; }


   /***********************************************************
    * GET CIPHER CITATION
    * Returns the citation from which we learned about the cipher
    ***********************************************************/
   virtual std::string getCipherCitation()
   {
      std::string s;
      s+="ScienceLearningSpace2.com\n";
      s+="https://www.sciencelearningspace2.com/math/documents/math-date-shift-cipher.pdf\n";
      s+="WikiHow.com\n";
      s+="https://www.wikihow.com/Create-Secret-Codes-and-Ciphers\n";
      return s;
   }
   
   /**********************************************************
    * GET PSEUDOCODE
    * Returns the pseudocode as a string to be used by the caller.
    **********************************************************/
   virtual std::string getPseudocode()
   {
      std::string str;

      // The encrypt pseudocode
      str +=  "convertTexttoNumbers(text)\n";
      str += "FOR i in text.size\n";
      str += " FOR a in alphabet\n";
      str += "    IF Text == alphabet\n";
      str += "       List.push_back(a)\n";
      str += "RETURN List\n";

      str += "\nencryptCipherNumbers(textN,passN)\n"

      "FOR x in textN AND y in passN\n"
      "  IF y == passN.end\n"
      "     y = passN.begin\n"
      "  List.push_back(x+y)\n"
      "RETURN cipherNumber\n";

      str += "\nencryptToText(cipherNumber)\n"

      "FOR x in cipherNumber\n"
      "  IF x > alphabet.size\n"
      "      list.push_back(x - alphabet * -1)\n"
      "  ELSE\n"
      "     list.push_back(x)\n\n"

      "FOR y in sum\n"
      "  FOR z in alphabet.size\n"
      "     IF y == z\n"
      "        cipherText += alphabet\n"
      "RETURN cipherText\n";

      str += "\nencrypt(plainText,password)\n"
      "textN <- convertTexttoNumbers(plainText)\n"
      "passN <- convertTexttoNumbers(password)\n\n"
      "cipherNumbers <- encryptCipherNumbers(textN,passN)\n"
      "cipherText <- encryptToText(cipherNumbers)\n"
      "RETURN cipherText\n";

      // The decrypt pseudocode

      str+="\ndecryptCipherNumbers(textN, passN)\n"

      "FOR x in textN AND y in passN\n"
      "  IF y == passN.end\n"
      "     y = passN.begin\n"
      "  substract <- x - y\n"
      "  cipherNumber.push_back(substract)\n"
      "RETURN cipherNumber\n";

      str+="\ndecryptToText(cipherNumber)\n"

      "FOR x in cipherNumber\n"
      "  IF x < 0\n"
      "     sum.push_back(x - alphabet.size)\n"
      "  ELSE\n"
      "     sum.push_back(x)\n\n"
      "FOR y in sum\n"
      "  FOR z in alphabet\n"
      "     IF y == z\n"
      "        plainText <- alphabet\n"
      "RETURN plainText\n";

      return str;
   }

   /**********************************************************
    * ENCRYPT
    * TODO: ADD description
    **********************************************************/
   virtual std::string encrypt(const std::string & plainText, 
                               const std::string & password)
   {
      //Declare variables
      string myText = plainText; //copy the plain text
      string pass = password; //copy password
      string cipherText;//cipherText Varibles
      list <int> textN;//list
      list <int> passN;//list
      list <int> cipherNumbers;//list

      //call function and obtain text in number
      textN = convertTexttoNumbers(myText);
      passN = convertTexttoNumbers(pass);

      //Obtain the sum of the two list
      cipherNumbers = encryptCipherNumbers(textN,passN);
      //Obtain the letters in cipher using the swift
      cipherText = encryptToText(cipherNumbers);

      return cipherText;//return cipherText
   }

   /**********************************************************
    * DECRYPT
    * TODO: ADD description
    **********************************************************/
   virtual std::string decrypt(const std::string & cipherText, 
                               const std::string & password)
   {
      //Declare variables
      std::string plainText;
      string myText = cipherText;//make a copy of cipherText to send as parameter
      string pass = password; // make a cope of password to send as parameter
      list <int> textN;//list
      list <int> passN;//list
      list <int> cipherNumbers;//list

      //Call functions and obtain the text in number
      textN = convertTexttoNumbers(myText);
      passN = convertTexttoNumbers(pass);

      //Obtain the substract of the two list
      cipherNumbers = decryptCipherNumbers(textN,passN);
      //Obtain the result of converts number in text
      plainText = decryptToText(cipherNumbers);
      
      return plainText; //PlainText result
   }
};

#endif // CIPHER01_H