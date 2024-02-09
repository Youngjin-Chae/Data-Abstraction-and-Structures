// Name: Youngjin Chae
// CWID: 20521744
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class Student { // A VERY simple Student consisting only of the student's ID and
                // Name Both the interface and the code will be located here.

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

/*
1. Automatic Resizing when array limit reached
2. Constructor, Copy Constructor, Destructor

3. GetSize: Get number of Student ptrs currently in array
4. Get:     Return student ptr at supplied array offset
5. Append:  Put supplied student ptr at the back of array - auto resizing may
happen
6. Prepend: Put supplied student ptr at the front of array, move all subsequent
elements back one
7. Insert:  Put supplied student ptr at indicated offset, move all subsequent
elements back one
           - auto resizing may happen
8. Remove:  Remove student ptr at specified offset, move all subsequent elements
forward one
9. Set:     Replace ptr to student element at specified offset with a new one
*/
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
    cnum = 01;
    cmz = 2;
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
    // delete sap[index]; feedback -> do not delete Student obj....
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
    if (nsp == nullptr) {
      return -1;
    }
    int result = insert(nsp, cnum);
    return result;
  }

  // 7. PREPEND: Prepend the ptr to the supplied Student to the front of the
  // ESA, increment ESA size
  //    Return 0 on success, -1 on failure
  //    Note:  This may force a reallocation of the array.
  int prepend(Student *nsp) {
    if (nsp == nullptr) {
      return -1;
    }
    int result = insert(nsp, 0);
    return result;
  }
};

int main() {          // Redirect Input
  string infileName;  // Use if Input redirected
  string outfileName; // Use if output redirected
  // FILE **input = NULL; // Recovering Cin

  // Command File Record entries
  int ssize; // Size of extended array
  int nops;  // # operations to perform

  char command; // Command (I,A,R,S)
                // Not all of the following are present in each command (Default
                // given)
  int index;    // Array Index (-1 default)
  int num;      // Student ID Number (-1 default)
  string name;  // Student Name (XXXX default)

  StudentEsa *esa = NULL; // Pointer to Enhanced Student Array
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
  //  Append:  A / -1 / StudentID / Student Name
  //  Insert:  I / Index to Insert / StudentID / Student Name
  //  Remove:  R / Index to Remove / -1 / XXXX
  //  Store:   S / Index to Store (overwrite) /  new StudentID / new Student
  //  Name

  // Get Size of Extended Array and # of commands
  // ssize is size of extended array, nops is # commands

  inp >> ssize >> nops;
  cout << "Read Array size " << ssize << "  Read # commands " << nops << endl;

  Student *stud; // Array to hold pointer of created student.
  esa = new StudentEsa(
      ssize); // Small Student Enhanced Array.  May have to be resized.

  for (int i = 0; i < nops; i++) { // Process Commands
    //**************************************************************
    inp >> command;
    inp >> index;
    inp >> num;
    inp >> name;
    cout << "Command: " << command << "  " << index << "  " << num << "  "
         << name << endl;

    // Process each command
    switch (command) { // Convert to command for Extended Array
    case 'A':          // Append
      stud = new Student(num, name);
      esa->append(stud);
      break;
    case 'P': // prepend
      stud = new Student(num, name);
      esa->prepend(stud);
      break;
    case 'I': // Insert
      stud = new Student(num, name);
      esa->insert(stud, index);
      break;
    case 'R': // Remove (delete)
      esa->remove(index);
      break;
    case 'S': // Store over existing Student
      stud = new Student(num, name);
      esa->set(stud, index);
      break;
    default:
      cout << "Illegal Command:  " << command << endl;
    }
  }
  // Print out Current contents of extended array
  cout << "-------" << endl << endl;

  x = esa->getSize();           // Get size of array (number of active elements
  for (int i = 0; i < x; i++) { // Print each element
    stud = esa->get(i);         // Get ptr to ith student in array
    cout << i << "  Student: ID = " << stud->getId()
         << "  Name = " << stud->getName() << endl;
  }
  return (0);
}
