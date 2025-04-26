#include "Effect.hpp"

Effect::Effect(string name, string description, float duration) {
    this->name = name;
    this->description = description;
    this->duration = duration;
    this->remainingDuration = duration;
}

Effect::Effect(const Effect& other) {
    this->name = other.name;
    this->description = other.description;
    this->duration = other.duration;
    this->remainingDuration = other.remainingDuration;
}  

Effect::~Effect() {}

void Effect::apply() {
    // Tunggu Repki
}

void Effect::remove() {
    // Tunggu Repki
}