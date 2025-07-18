#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

struct RowIndex {
    int64_t row_start, row_end;
    int64_t id_start, id_end;
    int64_t name_start, name_end;
    int64_t email_start, email_end;
};

class Database {
private:
    std::string dataFileName;
    std::string indexFileName;
    
    // Cache for recently accessed index entries
    mutable std::vector<RowIndex> indexCache;
    mutable int64_t cacheStartRow = -1;
    mutable int64_t cacheEndRow = -1;
    static const size_t INDEX_CACHE_SIZE = 100; // Cache 100 entries at a time

public:
    Database(const std::string& dataFile, const std::string& indexFile) 
        : dataFileName(dataFile), indexFileName(indexFile) {}

    // Get current file size
    int64_t getFileSize(const std::string& filename) const {
        std::ifstream file(filename, std::ios::binary | std::ios::ate); // seek to the end immediately after opening
        if (!file) return 0;
        return file.tellg();
    }

    // Get total number of rows without loading all indices
    int64_t getRowCount() const {
        int64_t indexFileSize = getFileSize(indexFileName);
        return indexFileSize / sizeof(RowIndex);
    }

    // Load specific index entry by row number (0-based)
    bool loadIndexEntry(int64_t rowNum, RowIndex& entry) const {
        // Check if it's in cache
        if (cacheStartRow != -1 && rowNum >= cacheStartRow && rowNum < cacheEndRow) {
            entry = indexCache[rowNum - cacheStartRow];
            return true;
        }

        // Calculate file position
        int64_t position = rowNum * sizeof(RowIndex);
        
        std::ifstream indexFile(indexFileName, std::ios::binary);
        if (!indexFile) {
            std::cerr << "❌ Failed to open index file.\n";
            return false;
        }

        // Seek to specific position
        indexFile.seekg(position);
        if (!indexFile.read(reinterpret_cast<char*>(&entry), sizeof(RowIndex))) {
            return false;
        }

        // Update cache with surrounding entries
        updateIndexCache(rowNum, indexFile);
        
        return true;
    }

    // Update cache with entries around the requested row
    void updateIndexCache(int64_t centerRow, std::ifstream& indexFile) const {
        int64_t totalRows = getRowCount();
        int64_t startRow = std::max(int64_t(0), int64_t(centerRow - INDEX_CACHE_SIZE / 2));
        int64_t endRow = std::min(int64_t(totalRows), int64_t(startRow + INDEX_CACHE_SIZE));

        indexCache.clear();
        indexCache.reserve(endRow - startRow);

        // Seek to start position
        indexFile.seekg(startRow * sizeof(RowIndex));
        
        for (int64_t i = startRow; i < endRow; ++i) {
            RowIndex entry;
            if (indexFile.read(reinterpret_cast<char*>(&entry), sizeof(RowIndex))) {
                indexCache.push_back(entry);
            } else {
                break;
            }
        }

        cacheStartRow = startRow;
        cacheEndRow = startRow + indexCache.size();
    }

    // Load a range of index entries (for pagination)
    std::vector<RowIndex> loadIndexRange(int64_t startRow, int64_t count) const {
        std::vector<RowIndex> entries;
        entries.reserve(count);

        std::ifstream indexFile(indexFileName, std::ios::binary);
        if (!indexFile) {
            std::cerr << "❌ Failed to open index file.\n";
            return entries;
        }

        // Seek to start position
        int64_t position = startRow * sizeof(RowIndex);
        indexFile.seekg(position);

        for (int64_t i = 0; i < count; ++i) {
            RowIndex entry;
            if (indexFile.read(reinterpret_cast<char*>(&entry), sizeof(RowIndex))) {
                entries.push_back(entry);
            } else {
                break; // End of file
            }
        }

        return entries;
    }

    // Insert a new row
    void insertRow(int64_t id, const std::string& name, const std::string& email) {
        // Get the current size of the data file
        int64_t row_start = getFileSize(dataFileName);
        
        // Open data file for appending
        std::ofstream dataFile(dataFileName, std::ios::binary | std::ios::app);
        if (!dataFile) {
            std::cerr << "❌ Failed to open data file for appending.\n";
            return;
        }

        // Calculate positions
        int64_t id_start = row_start;
        
        // Write ID as 8 bytes
        dataFile.write(reinterpret_cast<const char*>(&id), sizeof(int64_t));
        int64_t id_end = id_start + sizeof(int64_t);

        // Write Name
        int64_t name_start = id_end;
        dataFile.write(name.data(), name.size());
        int64_t name_end = name_start + name.size();

        // Write Email
        int64_t email_start = name_end;
        dataFile.write(email.data(), email.size());
        int64_t email_end = email_start + email.size();

        int64_t row_end = email_end;
        dataFile.close();

        // Write index entry
        std::ofstream indexFile(indexFileName, std::ios::binary | std::ios::app);
        if (!indexFile) {
            std::cerr << "❌ Failed to open index file.\n";
            return;
        }


        RowIndex entry{row_start, row_end, id_start, id_end, name_start, name_end, email_start, email_end};
        indexFile.write(reinterpret_cast<const char*>(&entry), sizeof(RowIndex));
        indexFile.close();
        
        // Invalidate cache since we added a new row
        cacheStartRow = -1;
        
        // std::cout << "✅ Inserted row: ID=" << id << ", Name=" << name << ", Email=" << email << "\n";
    }

    // Read bytes from file between [start, end)
    std::string readBytes(int64_t start, int64_t end) const {
        int64_t len = end - start;
        if (len <= 0) return "";
        
        std::ifstream dataFile(dataFileName, std::ios::binary);
        if (!dataFile) return "";
        
        std::vector<char> buffer(len);
        dataFile.seekg(start);
        dataFile.read(buffer.data(), len);
        return std::string(buffer.begin(), buffer.end());
    }

    // Read a specific row by index
    bool readRow(int64_t rowNum, int64_t& id, std::string& name, std::string& email) const {
        RowIndex entry;
        if (!loadIndexEntry(rowNum, entry)) {
            return false;
        }

        std::ifstream dataFile(dataFileName, std::ios::binary);
        if (!dataFile) {
            std::cerr << "❌ Failed to open data file for reading.\n";
            return false;
        }

        // Read ID
        dataFile.seekg(entry.id_start);
        dataFile.read(reinterpret_cast<char*>(&id), sizeof(int64_t));

        // Read name and email
        name = readBytes(entry.name_start, entry.name_end);
        email = readBytes(entry.email_start, entry.email_end);

        return true;
    }

    // Read rows with pagination
    void readRowsPaginated(int64_t startRow, int64_t pageSize) const {
        int64_t totalRows = getRowCount();
        int64_t endRow = std::min(totalRows, startRow + pageSize);
        
        std::cout << "\n📄 Reading rows " << startRow << " to " << (endRow - 1) 
                  << " (Total: " << totalRows << " rows):\n";

        for (int64_t i = startRow; i < endRow; ++i) {
            int64_t id;
            std::string name, email;
            
            if (readRow(i, id, name, email)) {
                std::cout << "Row " << i << ": ID=" << id << ", Name=" << name << ", Email=" << email << "\n";
            }
        }
    }

    // Search for rows by ID (efficient with index)
    bool findRowById(int64_t searchId, int64_t& foundRowNum) const {
        int64_t totalRows = getRowCount();
        
        // Linear search through index (in real DB, you'd use B-tree or hash index)
        for (int64_t i = 0; i < totalRows; ++i) {
            RowIndex entry;
            if (loadIndexEntry(i, entry)) {
                std::ifstream dataFile(dataFileName, std::ios::binary);
                if (dataFile) {
                    dataFile.seekg(entry.id_start);
                    int64_t id;
                    dataFile.read(reinterpret_cast<char*>(&id), sizeof(int64_t));
                    
                    if (id == searchId) {
                        foundRowNum = i;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    // Check if database files exist
    bool databaseExists() const {
        std::ifstream dataFile(dataFileName);
        std::ifstream indexFile(indexFileName);
        return dataFile.good() && indexFile.good();
    }
};

int main() {
    Database db("table.data", "table.index");

    // Check if this is a fresh database or has existing data
    if (db.databaseExists()) {
        std::cout << "📂 Loading existing database...\n";
        int64_t existingRows = db.getRowCount();
        std::cout << "Found " << existingRows << " existing rows.\n";
    } else {
        std::cout << "🆕 Creating new database...\n";
    }

    // Insert new rows
    while(true){
            db.insertRow(1, "Alice", "alice@example.com");
    db.insertRow(2, "Bob", "bob@example.com");
    db.insertRow(3, "Charlie", "charlie@uni.edu");
    db.insertRow(4, "David", "david@tech.com");
    db.insertRow(5, "Eve", "eve123@gmail.com");
    db.insertRow(6, "Frank", "frankie@school.edu");
    db.insertRow(7, "Grace", "grace.work@corp.net");
    db.insertRow(8, "Heidi", "heidi_secure@mail.org");
    db.insertRow(9, "Ivan", "ivan.dev@startup.io");
    db.insertRow(10, "Judy", "judy.hr@company.com");

    }
    // Demonstrate pagination - read first 5 rows
    
    
        db.readRowsPaginated(0, 1);
    
    
    // Read next 5 rows
    // db.readRowsPaginated(5, 5);

    // Search for specific ID
    // int64_t foundRow;
    // if (db.findRowById(7, foundRow)) {
    //     std::cout << "\n🔍 Found ID 7 at row " << foundRow << "\n";
    //     int64_t id;
    //     std::string name, email;
    //     if (db.readRow(foundRow, id, name, email)) {
    //         std::cout << "Details: ID=" << id << ", Name=" << name << ", Email=" << email << "\n";
    //     }
    // }

    // // Show total row count
    // std::cout << "\n📊 Total rows in database: " << db.getRowCount() << "\n";

    return 0;
}