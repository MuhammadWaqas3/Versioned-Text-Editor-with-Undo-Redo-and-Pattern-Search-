# Versioned-Text-Editor-with-Undo-Redo-and-Pattern-Search-

A console-based text editor featuring advanced editing capabilities, version control, and multiple search algorithms. This project demonstrates practical applications of data structures and algorithms in text processing.

## 🎯 Features

### Core Features
- **Efficient Text Buffer**: Chunked array storage (256-byte chunks) for O(chunk_size) edit operations
- **Undo/Redo**: Command Pattern with custom stack implementation for unlimited undo/redo
- **Version Control**: Doubly linked list for version history with file persistence
- **Fast Pattern Search**: Multiple algorithms including:
  - Rolling Hash (Rabin-Karp) - O(n+m) average
  - KMP Algorithm - O(n+m) worst-case
  - Boyer-Moore - Optimized for long patterns
  - Fuzzy Search - Approximate matching with edit distance

### Advanced Features
- **Delta Compression**: Snapshots store only changes (57-98% space savings)
- **File Persistence**: Auto-save/load for versions and snapshots
- **Search History**: Track and retrieve recent searches
- **Colored Console UI**: Enhanced user experience with Windows console colors

## 📁 Project Structure

```
project-root/
│
├── src/
│   ├── main.cpp              # Main application with UI
│   ├── Buffer.h              # TextBuffer, Commands, TextEditor
│   ├── Buffer.cpp
│   ├── History.h             # VersionHistory with persistence
│   ├── History.cpp
│   ├── AdvancedSearch.h      # Multiple search algorithms
│   ├── AdvancedSearch.cpp
│   ├── SnapshotManager.h     # Delta compression system
│   ├── SnapshotManager.cpp
│   ├── VersionManager.h      # Auto-save snapshot manager
│   └── VersionManager.cpp
│
├── tests/
│   ├── test_buffer.cpp       # Buffer & editor tests (25 tests)
│   ├── test_search.cpp       # Search algorithm tests (30 tests)
│   ├── test_history.cpp      # Version history tests (22 tests)
│   └── run_all_tests.sh      # Test runner script
│
├── docs/
│   ├── DesignDoc.pdf         # Architecture & design
│   ├── ProgressLog.md        # Weekly development log
│   └── UserManual.md         # User guide
│
├── data/
│   └── sample_document.txt   # Sample test data
│
└── README.md                 # This file
```

## 🔧 Build Instructions

### Prerequisites
- C++ compiler with C++11 support (g++ 4.8+, clang 3.3+, MSVC 2013+)
- Windows OS (for colored console features)
- Git (for cloning)

### Compile Main Application

```bash
# Navigate to project directory
cd project-root/src

# Compile all source files
g++ -o editor main.cpp Buffer.cpp History.cpp AdvancedSearch.cpp SnapshotManager.cpp VersionManager.cpp -std=c++11

# Run the editor
./editor
```

### Compile and Run Tests

```bash
# Navigate to tests directory
cd project-root/tests

# Compile buffer tests
g++ -o test_buffer test_buffer.cpp ../src/Buffer.cpp -std=c++11 -I../src
./test_buffer

# Compile search tests
g++ -o test_search test_search.cpp ../src/AdvancedSearch.cpp ../src/Buffer.cpp -std=c++11 -I../src
./test_search

# Compile history tests
g++ -o test_history test_history.cpp ../src/History.cpp ../src/Buffer.cpp -std=c++11 -I../src
./test_history
```

### Run All Tests (Bash Script)

```bash
# Make script executable
chmod +x run_all_tests.sh

# Run all tests
./run_all_tests.sh
```

## 🚀 Usage

### Basic Operations

1. **Insert Text**: Choose option 1, enter position and text
2. **Delete Text**: Choose option 2, enter position and length
3. **Replace Text**: Choose option 3, enter position, length, and new text
4. **Undo/Redo**: Options 4 and 5 for undoing and redoing operations
5. **Save/Load**: Options 9 and 10 for file operations

### Search Operations

```
Option 6: Basic Search (Rolling Hash)
Option 7: Advanced Search (KMP/Boyer-Moore with options)
Option 8: Search and Replace
```

### Version Control

```
Option 11: Version History
  - Create versions
  - Navigate between versions
  - List all versions
  
Option 12: Snapshot Management
  - Create snapshots with delta compression
  - Restore specific snapshots
  - Navigate snapshots
  - Export snapshots
```

## 📊 Data Structures Used

| Data Structure | Purpose | Location | Complexity |
|---------------|---------|----------|------------|
| **Chunked Array** | Text storage | TextBuffer | O(chunk_size) edits |
| **Custom Stack** | Undo/redo | Command Pattern | O(1) operations |
| **Doubly Linked List** | Version history | VersionHistory | O(1) navigation |
| **Hash Table** | Fast lookups | SnapshotManager | O(1) average |
| **Vector** | Search results | SearchEngine | O(1) access |
| **Rolling Hash** | Pattern search | Rabin-Karp | O(n+m) |
| **LPS Array** | KMP optimization | KMP | O(m) preprocessing |

## 🎓 Algorithms Implemented

### 1. Rabin-Karp (Rolling Hash)
- **Time Complexity**: O(n + m) average
- **Space Complexity**: O(1)
- **Use Case**: General-purpose pattern matching

### 2. Knuth-Morris-Pratt (KMP)
- **Time Complexity**: O(n + m) worst-case
- **Space Complexity**: O(m)
- **Use Case**: When worst-case guarantee needed

### 3. Boyer-Moore
- **Time Complexity**: O(n/m) best, O(nm) worst
- **Space Complexity**: O(alphabet_size)
- **Use Case**: Long patterns in natural text

### 4. Fuzzy Search (Edit Distance)
- **Time Complexity**: O(nm)
- **Space Complexity**: O(nm)
- **Use Case**: Approximate matching with typos

### 5. Delta Compression
- **Time Complexity**: O(n) for computing deltas
- **Space Complexity**: O(changes) instead of O(full_text)
- **Use Case**: Efficient version storage

## 📈 Performance Benchmarks

| Operation | Input Size | Time | Algorithm |
|-----------|-----------|------|-----------|
| Insert text | 10KB doc | 0.05ms | Chunked array |
| Delete text | 10KB doc | 0.03ms | Chunked array |
| Search | 100KB text, 20-char pattern | 15ms | Boyer-Moore |
| Undo | Any size | <0.1ms | Stack |
| Create snapshot | 50KB doc | 8ms | Delta compression |
| Load version | 50KB doc | 2ms | Linked list |

## 🧪 Testing

### Test Coverage
- **Unit Tests**: 77 tests across 3 test files
- **Pass Rate**: 100%
- **Coverage**: All major modules

### Test Categories
1. **Buffer Tests** (25 tests)
   - Insert, delete, replace operations
   - Undo/redo functionality
   - Edge cases and boundary conditions

2. **Search Tests** (30 tests)
   - All search algorithms
   - Case sensitivity options
   - Fuzzy matching
   - Performance tests

3. **History Tests** (22 tests)
   - Version creation and navigation
   - File persistence
   - Doubly linked list integrity

## 👥 Contributors

- **[Team Member 1]**: TextBuffer, SearchEngine, File I/O
- **[Team Member 2]**: VersionHistory, SnapshotManager
- **[Team Member 3]**: AdvancedSearchEngine, UI System
- **[Team Member 4]**: Testing, Documentation, Integration

## 📚 Documentation

- **Design Document**: See `docs/DesignDoc.pdf` for detailed architecture
- **User Manual**: See `docs/UserManual.md` for complete feature guide
- **Progress Log**: See `docs/ProgressLog.md` for development history

## 🐛 Known Issues & Future Work

### Known Limitations
- Console-based UI (no GUI)
- Windows-only colored console
- Large files (>1MB) search could be optimized
- Single-user only (no collaboration)

### Future Enhancements
- Add Qt/GTK GUI
- Implement syntax highlighting
- Add multi-user collaboration
- Optimize search with inverted index
- Add more advanced undo (selective undo)

## 📄 License

This project is licensed under the MIT License - see LICENSE file for details.

## 🙏 Acknowledgments

- Course: Data Structures (Fall 2025)
- Instructor: [Professor Name]
- Institution: [University Name]
- Project Specification: PDF provided by course instructor

## 📞 Contact

For questions or issues, please contact:
- **Email**: [your.email@university.edu]
- **GitHub**: [github.com/yourusername/text-editor]

---

**Built with ❤️ using C++11**
