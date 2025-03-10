In order to facilitate their exam preparation, a group of computer science students decided to systematize the way they collect notes by using a binder. For this method to be practical and truly systematic, the students agreed that the binders they use must meet certain basic requirements.

The format of the note content should not be predetermined (after all, some students take handwritten notes during classes, while others prefer to use electronic devices), but it must be consistent within a single binder. Additionally, each note should be visibly marked with a (binder-unique) tab to facilitate its retrieval.

You are required to implement a binder class template available in the cxx namespace with the following declaration:

namespace cxx { template <typename K, typename V> class binder; }

Here, the key type (tab) K is expected to have value semantics, meaning that a default constructor, copy constructor, move constructor, assignment operators, and destructor are available. A linear order is defined on type K, allowing any comparisons between objects of this type. For type V (the note's content), the only assumption is that it has a public copy constructor and a public destructor.

Students are eager to share notes. Since they are generally frugal, they prefer to use the same binder rather than creating a new physical copy. However, the situation changes when one of the users of the shared binder decides to make modifications. In this case, so as not to hinder the learning of their peers, the student must go to the copy machine and create his or her own copy of the binder.

Formally, the container used should implement copy-on-write semantics.

Copy-on-write is an optimization technique widely applied, among others, in data structures from the Qt library and formerly in implementations of std::string. Its basic idea is that when an object is copied (in C++ via a copy constructor or assignment operator), it shares all of its internal resources (which might be stored in a separate dynamically allocated object) with the original object. This state persists until one of the copies needs to be modified. At that point, the object being modified creates its own copy of the resources that will be modified.

To facilitate efficient collection and modification of notes, the binder class should provide the following operations. Each operation's time complexity (assuming no copying is needed) is given below. The time complexity for copying the binder is O(n log n), where n denotes the number of stored notes. All operations must guarantee at least a strong exception safety guarantee, and wherever possible and desirable (for example, the move constructor and destructor), they must be marked noexcept.
Constructors: a parameterless constructor creating an empty binder, a copy constructor, and a move constructor. Complexity O(1).

  binder();
  binder(binder const &);
  binder(binder &&);

Assignment operator, which takes its argument by value. Complexity O(1)
plus the time for destroying the overwritten object.

  binder & operator=(binder);

The insert_front method inserts a note with the given tab at the beginning of the binder. To maintain tab uniqueness, it is not allowed to insert a note with a tab that is already present in the binder – in such a case, the method throws std::invalid_argument. Complexity O(log n).

  void insert_front(K const &k, V const &v);

The insert_after method allows you to place a note with the given tab k directly after the note with the tab prev_k. This method behaves similarly to insert_front, but it also throws std::invalid_argument when the tab prev_k cannot be found in the binder. Complexity O(log n).

  void insert_after(K const &prev_k, K const &k, V const &v);

The parameterless remove method deletes the first note in the binder. If the binder is empty, it throws std::invalid_argument. Complexity O(log n).

  void remove();

The single-parameter remove method deletes from the binder the note with the specified tab. If no such note exists, it throws std::invalid_argument. Complexity O(log n).

  void remove(K const &);

The single-parameter read methods return a reference to the note labeled with the provided tab. In the non-const version, the returned reference should allow modification of the note. A modifying operation on the binder may invalidate the returned reference. If a note with the specified tab does not exist in the binder, the method throws std::invalid_argument. Complexity O(log n).

  V & read(K const &);
  V const & read(K const &) const;

The size method returns the number of notes in the binder. Complexity O(1).

  size_t size() const;

The clear method empties the binder (preparing it for the next semester), meaning it removes all notes. Complexity O(n).

  void clear();
  Finally, in establishing a consistent format for binders, the students agreed that it need not provide any methods other than those listed above (for instance, methods for searching tabs without throwing exceptions – at most, a colleague unaware of them might experience difficulties in quickly searching through the diligently prepared binder). Your task is to assist these students by creating an implementation of a binder that meets all the specified requirements.

Where possible and justified, methods should be qualified with const and noexcept.

An instance of the binder class should store only a single copy of the inserted tabs and notes.

The binder class should be exception-transparent, meaning that it should propagate any exceptions thrown by the functions it calls and by operations on its member components, and the observable state of the object should remain unchanged. In particular, modifying operations that fail must not invalidate iterators.
