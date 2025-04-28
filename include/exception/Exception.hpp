#pragma once

#include <exception>
#include <string>

class OutOfBoundsException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Backpack coordinates out of bounds";
    }
};

class EmptyCellException : public std::exception {
public:
    const char* what() const noexcept override {
        return "No item at the specified position";
    }
};

class InvalidQuantityException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid item quantity";
    }
};

class QuantityRequiresNullItemException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Quantity 0 requires a null item";
    }
};

class QuantityRequiresValidItemException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Quantity > 0 requires a valid item";
    }
};

class InvalidEquipmentTypeException : public std::exception {
private:
    std::string message;
    
public:
    InvalidEquipmentTypeException(const std::string& msg) : message(msg) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class BackpackOvercapacityException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Backpack is at full capacity";
    }
};

class InsufficientQuantityException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Not enough items of this type in backpack";
    }
};

class ItemNotFoundException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Item not found in backpack";
    }
};

class ItemSlotOccupiedException : public std::exception {
private:
    std::string message;
    
public:
    ItemSlotOccupiedException(const std::string& msg) : message(msg) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};