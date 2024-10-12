#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

class Borrowable {
public:
    virtual void borrow() = 0;
    virtual void returnItem() = 0;
    virtual bool isAvailable() const = 0;
    virtual ~Borrowable() = default;
};

class Book : public Borrowable {
private:
    std::string title;
    std::string author;
    std::string ISBN;
    bool available;

public:
    Book(std::string t, std::string a, std::string i)
        : title(std::move(t)), author(std::move(a)), ISBN(std::move(i)), available(true) {}

    void borrow() override {
        if (available) {
            available = false;
            std::cout << "Sách '" << title << "' đã được mượn." << std::endl;
        } else {
            std::cout << "Sách '" << title << "' không có sẵn để mượn." << std::endl;
        }
    }

    void returnItem() override {
        available = true;
        std::cout << "Sách '" << title << "' đã được trả lại." << std::endl;
    }

    bool isAvailable() const override {
        return available;
    }

    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getISBN() const { return ISBN; }
};

class DVD : public Borrowable {
private:
    std::string title;
    std::string director;
    int duration;
    bool available;

public:
    DVD(std::string t, std::string d, int dur)
        : title(std::move(t)), director(std::move(d)), duration(dur), available(true) {}

    void borrow() override {
        if (available) {
            available = false;
            std::cout << "DVD '" << title << "' đã được mượn." << std::endl;
        } else {
            std::cout << "DVD '" << title << "' không có sẵn để mượn." << std::endl;
        }
    }

    void returnItem() override {
        available = true;
        std::cout << "DVD '" << title << "' đã được trả lại." << std::endl;
    }

    bool isAvailable() const override {
        return available;
    }

    std::string getTitle() const { return title; }
    std::string getDirector() const { return director; }
    int getDuration() const { return duration; }
};

class Library {
private:
    std::vector<std::unique_ptr<Borrowable>> items;

public:
    void addItem(std::unique_ptr<Borrowable> item) {
        items.push_back(std::move(item));
    }

    void borrowItem(const std::string& title) {
        auto it = std::find_if(items.begin(), items.end(), [&title](const auto& item) {
            if (auto book = dynamic_cast<Book*>(item.get())) {
                return book->getTitle() == title;
            }
            if (auto dvd = dynamic_cast<DVD*>(item.get())) {
                return dvd->getTitle() == title;
            }
            return false;
        });

        if (it != items.end()) {
            (*it)->borrow();
        } else {
            std::cout << "Không tìm thấy mục '" << title << "' trong thư viện." << std::endl;
        }
    }
};

int main() {
    Library library;

    // Adding some books and DVDs to the library
    library.addItem(std::make_unique<Book>("The Great Gatsby", "F. Scott Fitzgerald", "9780743273565"));
    library.addItem(std::make_unique<Book>("1984", "George Orwell", "9780451524935"));
    library.addItem(std::make_unique<DVD>("Inception", "Christopher Nolan", 148));
    library.addItem(std::make_unique<DVD>("The Matrix", "The Wachowskis", 136));

    // Borrowing items
    library.borrowItem("The Great Gatsby");
    library.borrowItem("Inception");
    library.borrowItem("Unknown Title"); // Test for a non-existent item

    return 0;
}
