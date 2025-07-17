#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

void insert_row(int64_t id, const std::string& name, const std::string& email) {
    // Open column files
    std::ofstream id_file("id.data", std::ios::binary | std::ios::app);
    std::ofstream name_file("name.data", std::ios::binary | std::ios::app);
    std::ofstream email_file("email.data", std::ios::binary | std::ios::app);
    std::ofstream index_file("table.index", std::ios::binary | std::ios::app);

    // === 1. Get current file sizes (offsets)
    id_file.seekp(0, std::ios::end);
    int64_t id_start = id_file.tellp();

    name_file.seekp(0, std::ios::end);
    int64_t name_start = name_file.tellp();

    email_file.seekp(0, std::ios::end);
    int64_t email_start = email_file.tellp();

    // === 2. Write values
    id_file.write(reinterpret_cast<const char*>(&id), sizeof(int64_t));

    name_file.write(name.data(), name.size());
    email_file.write(email.data(), email.size());

    int64_t id_end = id_start + sizeof(int64_t);
    int64_t name_end = name_start + name.size();
    int64_t email_end = email_start + email.size();

    // === 3. Compute row start/end (optional, just for tracing)
    int64_t row_start = index_file.tellp(); // position in index
    int64_t row_end = row_start + 64;       // 8 fields × 8 bytes

    // === 4. Write index entry
    index_file.write(reinterpret_cast<const char*>(&row_start), sizeof(int64_t));
    index_file.write(reinterpret_cast<const char*>(&row_end), sizeof(int64_t));
    index_file.write(reinterpret_cast<const char*>(&id_start), sizeof(int64_t));
    index_file.write(reinterpret_cast<const char*>(&id_end), sizeof(int64_t));
    index_file.write(reinterpret_cast<const char*>(&name_start), sizeof(int64_t));
    index_file.write(reinterpret_cast<const char*>(&name_end), sizeof(int64_t));
    index_file.write(reinterpret_cast<const char*>(&email_start), sizeof(int64_t));
    index_file.write(reinterpret_cast<const char*>(&email_end), sizeof(int64_t));

    std::cout << "Inserted row: id=" << id << ", name=" << name << ", email=" << email << "\n";
}

int main() {
    insert_row(10, "Alice", "alice@example.com");
    insert_row(20, "Bob", "bob@shop.com");
    insert_row(30, "Carol", "carol@example.org");
    return 0;
}
