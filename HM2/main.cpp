#include <fstream>
#include <iostream>
using namespace std;

/*
  The following code file is divided into 4 sections

  1. The interface and code for Class Student  (supplied)
  2. The interface and code for class StudentEsa  (insert your HM1 code here)
  3. The interface and code for class StudentDQI  (The interface is supplied.
  HM2 is writing the code to implement this interface)
  4. The HM2 test program code (supplied)
*/

// 1.  ************************ class Student code included here
// ***************************

class Student { // A VERY simple Student consisting only of the student's ID and
                // Name Both the interface and the code will be located here.
  // test
private:
  int sid;      // Student ID
  string sname; // Full Name (Ex: KleinmanRon)

public:
  Student() { // Default constructor
    sid = 0;
    sname.clear();
  };
  Student(
      const Student &os) { // Copy constructor - overwrite internal variables
    sid = os.sid;
    sname = os.sname;
  };
  Student(int id,
          string name) { // Initializer Constructor - set internal variables
    sid = id;
    sname = name;
  };
  ~Student() { // Default Destructor - clear string
    sname.clear();
  };

  // Getters
  string getName() { return (sname); };
  int getId() { return (sid); };
};

//      ********************** End class Student
//      ***************************************
// 2.   ********************** Insert "class StudentEsa" code from your homework
// 1 assignment here

class StudentEsa {
private:         // Define whatever additional private variables you need
  Student **sap; // Underlying pointer to array of Student pointers, which you
                 // will allocate
  Student **tp;  // Optionally used in realloc
  int cnum;      // Number of students currently in array
  int cmz;       // Current Max size of the array (may need to be expanded)

public: // Publically supported methods YOU NEED TO IMPLEMENT.  Replace each
        // declaration with complete code
  // ****************  Constructors / Destructor

  //  1. Normal Constructor.  Create ESA of this size.  May need to be resized
  //  later
  //  *** The code for this might look like:
  //
  //  StudentEsa(int ms) { // Allocate Enhanced Student Array capable of holding
  //  this many Student Ptrs
  //       sap = new Student* [cmz = ms]; // sap points to an array of Student
  //       pointers of size cmz cnum = 0; tp = NULL; // Initialize with no
  //       pointers currently in array
  //  }
  StudentEsa(int ms) {
    sap = new Student *[cmz = ms];
    cnum = 0;
    tp = NULL;
  }

  StudentEsa() { // 2. Default Constructor <Comment out, or use some predefined
                 // size, and EXPLAIN YOUR CHOICE> 여기에는 아규먼트들
                 // 디폴트값으로 넣어두면 좋을 듯? need to figure out... what
                 // capacity is ideal
    cnum = 0;
    cmz = 4;
    sap = new Student *[cmz];
    tp = NULL;
    // My explaination:
  }
  StudentEsa(StudentEsa &sep) { // 3. Copy Constructor  (what gets copied and
                                // what does not?)
    cmz = sep.cmz;
    cnum = sep.cnum;
    sap = new Student *[sep.cmz];
    for (int i = 0; i < cnum; i++) {
      sap[i] = sep.sap[i];
    }
  }
  ~StudentEsa() { // 4. Default destructor  (what gets freed up and what does
                  // not?)
    delete[] sap;
    delete[] tp;
  }
  // ******************* Remaining public functions of the Extended Student
  // Array

  // getSize, get 함수들 적절히 사용할 것.
  // 1. Return the number of Students in the Collection
  int getSize() { return cnum; };

  // 2. GET:  Get and return the ptr to the Student at the specified Array Index
  //          Error if the supplied index >= number of entries.  Return a NULL
  //          ptr
  // *** The code for this might look like:
  //   Student* get (int idx) {
  //      if ((idx<0) || (idx>=cnum))  return (NULL);  // Bad index
  //      return (sap[idx]); // Otherwise return Student ptr at that index in
  //      the array
  //   }
  Student *get(int index) {
    // gotta put some conditon
    return (sap[index]);
  }

  // 3. SET: Replace whatever is at the supplied index with the new Student ptr.
  // Return index # on success Error if the supplied index >= number of entries.
  // Return -1 <DO NOT Destruct the Student whose ptr is at the supplied index>
  // (*)asterisk is indirection op, ptr is identifier
  int set(Student *ptr, int index) // overwriting..
  {

    if (index < 0 || index >= getSize() || ptr == nullptr) {
      return -1; // Error condition
    }
    delete sap[index];
    sap[index] = ptr;

    return index;
  }

  // 4. INSERT: Insert Student ptr at the supplied index, by first "pushing
  // back" every subsequent element
  //    Error if the supplied index > number of entries.  Return -1
  //    OK if supplied index == number of entries (equivalent to an append)
  //    Note:  ESA size increases which may force a reallocation of the array.
  //    Return index # on success
  int insert(Student *nsp, int index) {
    // Check the input bound
    if (index < 0 || index > cnum || nsp == nullptr) {
      return -1;
    }

    if (cmz == cnum) {
      cmz *= 2;
      tp = new Student *[cmz];
      for (int i = 0; i < index; i++) {
        tp[i] = sap[i];
      }
      for (int i = index; i < cnum; i++) {
        tp[i + 1] = sap[i];
      }
      delete[] sap;
      sap = tp;
    } else {
      for (int i = cnum; i > index; i--) {
        sap[i] = sap[i - 1];
      }
    }
    sap[index] = nsp;
    cnum++;
    return index;
  }

  // 5. REMOVE: Delete specified array element by "pulling forward" every
  // subsequent element
  //    Error if the supplied index >= number of entries.  Return -1.  Else
  //    decrement ESA size and return it Note: <DO NOT Destruct the Student
  //    whose ptr is at the supplied index>
  int remove(int index) {
    if (index < 0 || index >= cnum) {
      return -1;
    }

    // Shift elements down to fill the gap
    for (int i = index; i < cnum - 1; i++) {
      sap[i] = sap[i + 1];
    }
    cnum--; // Decrease the count of elements

    return index;
  }

  // 6. APPEND:  Append the ptr to the supplied Student to back of the ESA, bump
  // ESA size
  //    Return index # of new entry on success, -1 on failure
  //    Note:  This may force a reallocation of the array.
  int append(Student *nsp) {
    int result = insert(nsp, cnum);
    return result;
  }

  // 7. PREPEND: Prepend the ptr to the supplied Student to the front of the
  // ESA, increment ESA size
  //    Return 0 on success, -1 on failure
  //    Note:  This may force a reallocation of the array.
  int prepend(Student *nsp) {
    int result = insert(nsp, 0);
    return result;
  }
};

//    ********************** End class StudentEsa ******************************

// 3. *************************** Homework 2.  insert the code for the
// StudentDQI into the StudentDQI interface below ***************

// class "StudentDQI   <Homework #2 - wraps (embeds) Extended Student Array
// (ESA) so it is invisible to external users of this class

class StudentDQI {
private:
  StudentEsa *soa; // Allocated Extended Student Array
                   // These might be useful if your solution is for extra credit
                   // and positions the DQ in the MIDDLE of the Extended Array
  unsigned int top; // Index value 1 above highest OA element used
  unsigned int btm; // Index value 1 below lowest OA element used

public:
  // Create a DQ with this initial size
  StudentDQI(unsigned int size) {
    soa = new StudentEsa(size);
    btm = 0;
    top = -1;
  }
  // Default Constructor //
  StudentDQI() {
    soa = new StudentEsa(8);
    btm = 0;
    top = -1;
  }
  // Equate this to an existing DQ (can use private parts directly)
  StudentDQI(StudentDQI &rhs) {
    soa =
        new StudentEsa(*rhs.soa); // use the copy constructor of StudentEsa obj
    btm = 0;                      // btm is 0
    top = rhs.soa->getSize() - 1; // copy the top of the rhs's
  }
  // Destruct the DQI (free OA space)
  ~StudentDQI() { delete soa; }

  // Gets # elements (Student *) in the DQ
  int getSize() { return soa->getSize(); }

  // True if no elements held (“pop” will fail)
  bool isEmpty() {
    bool result = false;
    if (soa->getSize() == 0) {
      result = true;
    }
    return result;
  }

  // Prepend a new element to the front
  int pushFront(Student *arg) {
    int result = soa->prepend(arg);
    if (isEmpty() || btm == 0) { // if array is empty or array at index 0 is
                                 // filled, then top should be incremented
      top++;
    } else { // else, btm should be decremented
      btm--;
    }
    return result;
  }

  // Remove the first element and return it
  Student *popFront() {
    Student *result;
    if (isEmpty()) { // if array is empty, return nullptr
      result = nullptr;
    } else { // else, remove the element at btm and return it
      result = soa->get(btm);
      soa->remove(btm);
      btm++;
    }
    return result;
  }

  // Return the first element but do not remove it
  Student *lookFront() { // return the element at btm
    Student *result = soa->get(btm);
    return result;
  }

  // Append a new element to the back
  int pushBack(Student *arg) { // pushing back always append elements, so top is
                               // incremented
    int result = soa->append(arg);
    top++;
    return result;
  }
  // Remove the last element and return it
  Student *popBack() {
    Student *result;
    if (isEmpty()) { // if the array is empty, return nullptr, and set top as -1
      result = nullptr;
      top = -1;
    } else { // else, remove the element at top and return it
      result = soa->get(top);
      soa->remove(top);
      top--;
    }
    return result;
  }
  // Return the last element but do not remove it
  Student *lookBack() {
    Student *result = soa->get(top);
    return result;
  }
};

// **************************** End class StudentDQI
// ******************************

// 4.  ***********************  Start of StudentDQI Test Program Code
// ************

int main() {          // Redirect Input
  string infileName;  // Use if Input redirected
  string outfileName; // Use if output redirected
  // FILE** input = NULL;  // Recovering Cin

  // Command File Record entries
  int ssize; // Size of extended array
  int nops;  // # operations to perform

  char command; // Command (G,Z   F,G,H   B,C,D)
                // Not all of the following are present in each command (Default
                // given)
  int num;      // Student ID Number (-1 default)
  string name;  // Student Name (XXXX default)

  StudentDQI *dqi = NULL; // Pointer to DeQue
  Student sc;             // A class to collect Students is generated

  int x = 0; // Useful variables

  cout << "Use input file:  ";
  cin >> infileName; // Get name of file containing input data
  cout << "Using input file " << infileName << endl << endl;

  // Create an input file stream to read supplied file
  std::ifstream inp;
  inp.open(infileName.c_str());
  if (!inp) {
    cerr << "Error: file " << infileName.c_str() << "  could not be opened"
         << endl;
    exit(1);
  }

  // First line is array size and # commands to add, every subsequent line is
  // one of:
  //
  // GetSize:      S / -1 / -1
  // isEmpty:      Z / -1 / -1
  //
  //  PushFront:   F / Student ID / Student Name
  //  PopFront:    G / -1 / -1
  //  LookFront :  H / -1 / -1
  //
  //  PushBack:    B / Student ID / Student Name
  //  PopBack:     C / -1 / -1
  //  LookBack :   D / -1 / -1

  // Get Size of Extended Array and # of commands
  // ssize is size of extended array, nops is # commands

  inp >> ssize >> nops;
  cout << "Read Array size " << ssize << "  Read # commands " << nops << endl;

  Student *stud;               // Array to hold pointer of created student.
  dqi = new StudentDQI(ssize); // Create Dequeue of specified size.

  for (int i = 0; i < nops; i++) { // Process Commands
    //**************************************************************
    inp >> command;
    inp >> num;
    inp >> name;
    cout << "Command: " << command << "  " << num << "  " << name << endl;

    // Process each command
    switch (command) { // Convert to command for Extended Array
    case 'S':          // Get Size
      cout << "Size:  " << dqi->getSize() << endl;
      break;

    case 'Z': // Check if Empty
      cout << boolalpha << "Empty : " << dqi->isEmpty() << endl;
      break;

    case 'F': // Push Front
      stud = new Student(num, name);
      (void)dqi->pushFront(stud);
      cout << "Pushed Front " << num << "  " << name << endl;
      break;

    case 'G': // Pop Front
      stud = dqi->popFront();
      if (stud) { // Success
        cout << "Pop Front " << stud->getId() << "  " << stud->getName()
             << endl;
        delete (stud);
      }
      break;

    case 'H': // Look Front
      stud = dqi->lookFront();
      if (stud) { // Success ... don't delete Student
        cout << "Look Front " << stud->getId() << "  " << stud->getName()
             << endl;
      }
      break;

    case 'B': // Push Back
      stud = new Student(num, name);
      (void)dqi->pushBack(stud);
      cout << "Pushed Back " << num << "  " << name << endl;
      break;

    case 'C': // Pop Back
      stud = dqi->popBack();
      if (stud) { // Success
        cout << "Pop Back " << stud->getId() << "  " << stud->getName() << endl;
        delete (stud);
      }
      break;

    case 'D': // Look Back
      stud = dqi->lookBack();
      if (stud) { // Success ... don't delete Student
        cout << "Look Back " << stud->getId() << "  " << stud->getName()
             << endl;
      }
      break;

    default:
      cout << "Illegal Command:  " << command << endl;
    }
  }
  // Print out Current contents of extended array
  x = dqi->getSize(); // Get number of array elements
  cout << "------------------" << endl
       << "Start Popping from bottom  " << x << "  Elements queued" << endl
       << endl;

  while ((stud = dqi->popBack())) {
    cout << "Student: ID = " << stud->getId() << "  Name = " << stud->getName()
         << endl;
  }
  return (0);
};

// **********************************  End of test code
// *************************************