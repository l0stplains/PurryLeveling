#pragma once

#include <exception>
#include <string>

/* Inventory Exceptions */

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

/* Shop Exceptions */

class InsufficientGoldException : public std::exception
{
public:
    const char* what() const noexcept override { return "Not enough money to buy the item"; }
};

class ItemNotFoundException : public std::exception
{
private:
    std::string itemID;

public:
    ItemNotFoundException(const std::string& id) : itemID(id) {}
    const char* what() const noexcept override
    {
        static std::string message;
        message = "Item not found: " + itemID;
        return message.c_str();
    }

    const std::string& getItemID() const { return itemID; }
};
/* File Processing Exceptions */

class FileNotFoundException : public std::exception
{
private:
    std::string filename;
    std::string errorMessage;

public:
    FileNotFoundException(const std::string& file, const std::string& error)
        : filename(file), errorMessage(error)
    {}

    const char* what() const noexcept override
    {
        static std::string message;
        message = "File not found: " + filename + "\nError: " + errorMessage;
        return message.c_str();
    }

    const std::string& getFilename() const { return filename; }
    const std::string& getErrorMessage() const { return errorMessage; }
};

class LineTooShortException : public std::exception
{
private:
    std::string filename;
    std::string errorMessage;

public:
    LineTooShortException(const std::string& file, const std::string& error)
        : filename(file), errorMessage(error)
    {}

    const char* what() const noexcept override
    {
        static std::string message;
        message = "Line too short in file: " + filename + "\nError: " + errorMessage;
        return message.c_str();
    }

    const std::string& getFilename() const { return filename; }
    const std::string& getErrorMessage() const { return errorMessage; }
};

class InvalidFormatException : public std::exception
{
private:
    std::string filename;
    std::string errorMessage;

public:
    InvalidFormatException(const std::string& file, const std::string& error)
        : filename(file), errorMessage(error)
    {}

    const char* what() const noexcept override
    {
        static std::string message;
        message = "Invalid format in file: " + filename + "\nError: " + errorMessage;
        return message.c_str();
    }

    const std::string& getFilename() const { return filename; }
    const std::string& getErrorMessage() const { return errorMessage; }
};

class ResourceNotFoundException : public std::exception
{
private:
    std::string filename;
    std::string errorMessage;

public:
    ResourceNotFoundException(const std::string& file, const std::string& error)
        : filename(file), errorMessage(error)
    {}

    const char* what() const noexcept override
    {
        static std::string message;
        message = "Resource not found in file: " + filename + "\nError: " + errorMessage;
        return message.c_str();
    }

    const std::string& getFilename() const { return filename; }
    const std::string& getErrorMessage() const { return errorMessage; }
};

class MissingFileException : public std::exception
{
private:
    std::string filename;
    std::string errorMessage;

public:
    MissingFileException(const std::string& file, const std::string& error)
        : filename(file), errorMessage(error)
    {}

    const char* what() const noexcept override
    {
        static std::string message;
        message = "Missing file: " + filename + "\nError: " + errorMessage;
        return message.c_str();
    }

    const std::string& getFilename() const { return filename; }
    const std::string& getErrorMessage() const { return errorMessage; }
};

class InvalidCharacterTypeException : public std::exception
{
private:
    std::string m_type;

public:
    InvalidCharacterTypeException(const std::string& type) : m_type(type) {}
    const char* what() const noexcept override
    {
        return ("Invalid character type: " + m_type).c_str();
    }
};

class InvalidEquipmentDataException : public std::exception
{
public:
    const char* what() const noexcept override { return "Invalid equipment data"; }
};