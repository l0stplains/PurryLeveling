#pragma once

#include <exception>
#include <string>

class OutOfBoundsException : public std::exception
{
public:
    const char* what() const noexcept override { return "Backpack coordinates out of bounds"; }
};

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

class InvalidEquipmentTypeException : public std::exception
{
private:
    std::string message;

public:
    InvalidEquipmentTypeException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class BackpackOvercapacityException : public std::exception
{
public:
    const char* what() const noexcept override { return "Backpack is at full capacity"; }
};

class InsufficientQuantityException : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return "Not enough items of this type in backpack";
    }
};

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
    const char* what() const noexcept override
    {
        return "Not enough money to buy the item";
    }
};