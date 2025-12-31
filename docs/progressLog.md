

# Project Progress Log

## Project: Versioned Text Editor with Undo/Redo and Pattern Search

**Team Members:** Ahsan, Waqas
**Project Duration:** Week 9 – Week 13

---

## Week 9 — Project Charter & Initial Planning

**Date:** 30 November 2025
**Author:** Ahsan

### Tasks Completed

* ✅ Team formation and role assignment
* ✅ Project requirements analysis from specification document
* ✅ Domain study: Research on text editors (VS Code, Sublime Text architecture)
* ✅ Initial data structure brainstorming
* ✅ Created project charter document

### Key Decisions

#### Text Storage

* **Decision:** Chunked array (rope-like) instead of single string
* **Reasoning:**

  * O(chunk_size) edit operations vs O(n) for single string
* **Trade-off:**

  * Slightly more complex implementation for better performance

#### Undo/Redo

* **Decision:** Command Pattern with custom Stack implementation
* **Reasoning:**

  * Clean separation of concerns
  * Industry-standard pattern
* **Bonus:** Custom stack implementation for extra marks

#### Search Algorithm

* **Primary:** Rolling Hash (Rabin-Karp)
* **Secondary:** KMP
* **Reasoning:**

  * Rolling hash required by specification
  * KMP added for bonus marks

### Challenges & Risks

* **Challenge:** Understanding rope data structure complexity

  * **Solution:** Simplified to chunked array with 256-byte chunks
* **Risk:** Time constraint for multiple search algorithms

  * **Mitigation:** Prioritize rolling hash first, add KMP if time permits

### Next Week Goals

* Design detailed class architecture
* Create UML diagrams for presentation
* Start implementing `TextBuffer` class
* Prepare Week 10 presentation

---

## Week 10 — Architecture Design & Presentation 1

**Date:** 4 December 2025
**Author:** Ahsan

### Tasks Completed

* ✅ Created comprehensive class diagrams
* ✅ Designed module structure (Buffer, History, Search, UI)
* ✅ Implemented core `TextBuffer` class with chunked storage
* ✅ Delivered Presentation 1 (Architecture & Plan)
* ✅ Started custom `Stack<T>` implementation

### Technical Implementation

#### TextBuffer Class (`Buffer.h/.cpp`)

* **Private:** `vector<string> chunks`, `CHUNK_SIZE = 256`
* **Methods:** `insert()`, `deleteText()`, `replace()`, `rebalance()`
* **Complexity:**

  * O(chunk_size) for edits
  * O(n) worst case

#### Custom Stack (`Buffer.h`)

* Template class with linked list nodes
* Methods: `push()`, `pop()`, `top()`, `empty()`
* Memory: Dynamic allocation with proper destructor

### Presentation 1 Feedback

* ✅ Strengths: Clear architecture, good data structure choices
* ⚠️ Suggestions: Add more edge case handling, consider file persistence
* 📝 Action Items: Implement error checking, add file I/O module

### Challenges Encountered

* **Issue:** Rebalancing algorithm not preserving order
* **Solution:** Accumulate all chunks, then redistribute
* **Code Location:** `TextBuffer::rebalance()` — `Buffer.cpp` line 90

### Performance Testing

* **Test:** Insert 10,000 characters at different positions
* **Result:** Average 0.05 ms per operation
* **Bottleneck:** Large insertions causing rebalancing

### Next Week Goals

* Implement Command Pattern (`InsertCommand`, `DeleteCommand`, `ReplaceCommand`)
* Complete undo/redo functionality
* Start `SearchEngine` with rolling hash
* Begin `VersionHistory` module

---

## Week 11 — Core Implementation

**Date:** 18 December 2025
**Author:** Ahsan

### Tasks Completed

* ✅ Implemented Command Pattern for undo/redo
* ✅ Created `SearchEngine` with Rabin-Karp algorithm
* ✅ Developed `VersionHistory` with doubly linked list
* ✅ Added file I/O operations (save/load)
* ✅ Started `AdvancedSearchEngine` with KMP

### Module Details

#### 1. Command Pattern (`Buffer.h/.cpp`)

* Abstract `Command` class with `execute()` and `undo()`
* Concrete classes:

  * `InsertCommand`
  * `DeleteCommand`
  * `ReplaceCommand`

#### 2. SearchEngine (`Buffer.cpp`)

* Rolling Hash implementation
* `calculateHash()`
* `recalculateHash()` — O(1) sliding window
* `search()` — O(n+m) average
* Verification step to avoid false positives

#### 3. VersionHistory (`History.h/.cpp`)

* Doubly linked list
* `VersionNode { version, next, prev }`
* Navigation: previous/next version
* Persistence: `serialize()` / `deserialize()`

### Algorithm Complexity Analysis

| Operation   | Data Structure | Time          | Space          |
| ----------- | -------------- | ------------- | -------------- |
| Insert      | Chunked array  | O(chunk_size) | O(1) amortized |
| Delete      | Chunked array  | O(k)          | O(1)           |
| Undo/Redo   | Stack          | O(1)          | O(n)           |
| Search      | Rolling hash   | O(n+m)        | O(1)           |
| Version nav | Linked list    | O(1)          | O(v)           |

### Testing Results

* `test_buffer.cpp`: 15 tests (all passing)
* `test_search.cpp`: 8 tests (all passing)
* Manual integration testing in `main.cpp`

### Known Issues

* Search on very large text (>100KB) slower than expected
* Needs optimization (indexing)

### Next Week Goals

* Complete AdvancedSearchEngine (KMP, Boyer-Moore)
* Add SnapshotManager with delta compression
* Implement VersionManager with auto-save
* Create colored console UI
* Prepare demo for Presentation 2

---

## Week 12 — Advanced Features & Presentation 2

**Date:** 30 December 2025
**Author:** Ahsan

### Tasks Completed

* ✅ AdvancedSearchEngine (KMP, Boyer-Moore, Fuzzy)
* ✅ SnapshotManager with delta compression
* ✅ VersionManager with persistence
* ✅ SearchHistory tracking
* ✅ Colored console UI
* ✅ Presentation 2 demo

### Delta Compression Highlights

* Store only changes, not full text
* Space savings: **57–98%**
* Reconstruction via delta application

### Performance Benchmarks

| Text Size | Operation        | Time   | Algorithm    |
| --------- | ---------------- | ------ | ------------ |
| 10KB      | Search "hello"   | 2ms    | Rolling Hash |
| 10KB      | Search "hello"   | 1.5ms  | KMP          |
| 100KB     | Pattern 20 chars | 15ms   | Boyer-Moore  |
| 1000 ops  | Undo             | <0.1ms | Stack        |
| 50KB      | Snapshot         | 8ms    | Delta        |

---

## Week 13 — Final Testing & Documentation

**Date:** 31 December 2025
**Author:** Ahsan

### Tasks Completed

* ✅ 87+ unit & integration tests
* ✅ README.md
* ✅ DesignDoc.pdf
* ✅ UserManual.md
* ✅ Final bug fixes
* ✅ Release tagged v1.0

### Final Statistics

* **Lines of Code:** 2,847
* **Classes:** 18
* **Functions:** 120+
* **Test Coverage:** 100% pass rate

---

## Individual Contributions

* **Waqas:** TextBuffer, SearchEngine, File I/O
* **Ahsan:** VersionHistory, SnapshotManager, AdvancedSearch, UI, Testing & Docs

---

## Lessons Learned

* Chunked storage dramatically improves editor performance
* Command Pattern simplifies undo/redo
* Rolling hash requires verification
* Delta compression saves massive storage
* Early testing saves time later

---

## End of Progress Log


