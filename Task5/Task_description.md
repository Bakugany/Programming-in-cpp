# Binder

## Introduction

Students are expected to learn:

- Exception safety levels,
- Schemes that ensure at least strong exception safety,
- Memory management using smart pointers,
- Techniques for implementing copy-on-write semantics when modifying objects.

## Task

To facilitate study for exams, a group of computer science students decided to systematize the way they collect lecture notes using a binder. For this method to be practical and truly organized, the binders they use must meet some basic requirements.

The format of the notes' content should not be predetermined (after all, some students take handwritten notes during classes while others prefer using electronic devices), but it must be consistent within a single binder. Additionally, each note must be visibly marked with a tab (unique within a binder) to aid in retrieval.

You are required to implement a class template `binder` available in the namespace `cxx` with the following declaration:

```cpp
namespace cxx {
  template <typename K, typename V> class binder;
}
```

The key type (the tab) `K` must have value semantics, which means that a default (no-argument) constructor, copy constructor, move constructor, copy and move assignment operators, and destructor are available. A linear order is defined on type `K` and all standard comparisons can be performed on its objects. For the value type `V` (the note content), it is only assumed that it has a public copy constructor and a public destructor.

Students are eager to share their notes. However, since they are generally frugal, they prefer to share the same binder rather than creating a new physical copy. The situation changes when one of the users of a shared binder decides to make modifications. In that case, to avoid hindering their classmates’ studies, the student must go to the copier and create their own copy of the binder.

Formally, the container used should implement copy-on-write semantics. Copy-on-write is a widely used optimization technique, for example in data structures from the Qt library and previously in std::string implementations. The basic idea is that when an object is copied (using the copy constructor or assignment operator), the copies share all internal resources (which may be stored in a separate heap object) with the original. This state persists until one of the copies needs to be modified. At that point, the modifying object makes its own copy of the resources before performing the modification.

## Binder Operations

The class `binder` should provide the following operations. For each operation, the indicated time complexity applies assuming that no copy is required. The time complexity for copying the entire binder is O(n log n), where *n* is the number of stored notes. All operations must ensure at least strong exception safety, and where possible and appropriate (for example, the move constructor and destructor) they must not throw exceptions.

### Constructors

- **Default Constructor:**  
  ```cpp
  binder();
  ```  
  Creates an empty binder. Complexity: O(1)

- **Copy Constructor and Move Constructor:**  
  ```cpp
  binder(binder const &);
  binder(binder &&);
  ```  
  Complexity: O(1)

### Assignment Operator

- **Assignment Operator** (taking the argument by value):  
  ```cpp
  binder & operator=(binder);
  ```  
  Complexity: O(1) plus the time for destroying the overwritten object.

### Insertion Methods

- **insert_front:**  
  ```cpp
  void insert_front(K const &k, V const &v);
  ```  
  Inserts a note with tab `k` at the front of the binder. To maintain uniqueness of tabs, it is not allowed to insert a note with a tab that is already in use in the binder – in that case, the method throws a `std::invalid_argument` exception. Complexity: O(log n)

- **insert_after:**  
  ```cpp
  void insert_after(K const &prev_k, K const &k, V const &v);
  ```  
  Inserts a note with tab `k` directly after the note with tab `prev_k`. This method behaves similarly to `insert_front` but also throws a `std::invalid_argument` exception if the tab `prev_k` cannot be found in the binder. Complexity: O(log n)

### Removal Methods

- **remove (without parameters):**  
  ```cpp
  void remove();
  ```  
  Removes the first note from the binder. Throws `std::invalid_argument` if the binder is empty. Complexity: O(log n)

- **remove (with a key parameter):**  
  ```cpp
  void remove(K const &);
  ```  
  Removes the note with the given tab. Throws `std::invalid_argument` if such a note does not exist in the binder. Complexity: O(log n)

### Read Methods

- **read:**  
  ```cpp
  V & read(K const &);
  V const & read(K const &) const;
  ```  
  Returns a reference to the note marked with the given tab. In the non-const version, the returned reference should allow modification of the note. A modifying operation on the binder may invalidate the returned reference. Throws `std::invalid_argument` if a note with the given tab does not exist. Complexity: O(log n)

### Size Method

- **size:**  
  ```cpp
  size_t size() const;
  ```  
  Returns the number of notes in the binder. Complexity: O(1)

### Clear Method

- **clear:**  
  ```cpp
  void clear();
  ```  
  Empties the binder (preparing it for the next semester) by removing all notes. Complexity: O(n)

### Iterators

To allow quick reviewing of all notes (for example, when studying for an exam), the binder should support efficient paging through its contents by providing:

- A `const_iterator` type along with the methods `cbegin` and `cend`.
- Iterator operations that include:
  - Assignment (`=`),
  - Comparisons (`==` and `!=`),
  - Both prefix and postfix increment (`++`),
  - Dereference (`*`) and member access (`->`).
- The iterators should allow browsing the notes in the order they appear in the binder. If no copy of the binder has occurred, the iterator remains valid.
- The iterator must satisfy the requirements of a `std::forward_iterator`. All iterator operations must have O(1) time complexity.
- The iterator is meant solely for reading notes and cannot be used to modify the binder; hence, it behaves like a standard library `const_iterator`.

## Additional Requirements

- Wherever possible and justified, methods should be qualified with `const` and `noexcept`.
- Each `binder` object must store only one copy of the inserted tabs and notes.
- The `binder` class should be transparent to exceptions; that is, it should let exceptions thrown by called functions or by operations on its members propagate, and the observable state of the object must not change. In particular, modifying operations that fail must not invalidate iterators.

This concludes the specification for the Binder module.
