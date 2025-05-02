#pragma once

#include <exception>
#include <string>

class OutOfBoundsException : public std::exception
{
public:
    const char* what() const noexcept override { return "Backpack coordinates out of bounds"; }
};

// Exception ketika tidak ada item pada slot yang diminta
class EmptyCellException : public std::exception
{
public:
    const char* what() const noexcept override { return "No item at the specified position"; }
};

class InvalidQuantityException : public std::exception
{
public:
    const char* what() const noexcept override { return "Invalid item quantity"; }
};

class QuantityRequiresValidItemException : public std::exception
{
public:
    const char* what() const noexcept override { return "Quantity > 0 requires a valid item"; }
};

// Exception ketika item tidak sesuai dengan tipe slot yang diminta
class InvalidEquipmentTypeException : public std::exception
{
private:
    std::string message;

public:
    InvalidEquipmentTypeException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

// Exception ketika backpack penuh dan tidak bisa menampung item baru
class BackpackOvercapacityException : public std::exception
{
public:
    const char* what() const noexcept override { return "Backpack is at full capacity"; }
};

// Exception ketika kuantitas item yang diminta lebih banyak dari total yang ada di backpack
class InsufficientQuantityException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Not enough items of this type in backpack";
    }
};

// Exception ketika slot yang diminta sudah terisi item lain
class ItemSlotOccupiedException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Cannot add a different item to an occupied slot";
    }
};

class InsufficientGoldException : public std::exception
{
public:
    const char* what() const noexcept override { return "Not enough money to buy the item"; }
};

class InsufficientGoldException : public std::exception
{
public:
    const char* what() const noexcept override { return "Not enough money to buy the item"; }
};

class FileNotFoundException : public std::exception
{
public:
    const char* what() const noexcept override { return "File not found"; }
};

class LineTooShortException : public std::exception
{
public:
    const char* what() const noexcept override { return "Line has too few columns"; }
};