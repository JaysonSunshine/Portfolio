#include "Sequence.h"

#define CHECKTYPE(f, t) { auto p = (t)(f); (void)p; }

void thisFunctionWillNeverBeCalled()
{
    Sequence();
    (Sequence(Sequence()));
    CHECKTYPE(&Sequence::operator=,  Sequence& (Sequence::*)(const ItemType&));
    CHECKTYPE(&Sequence::empty,      bool (Sequence::*)() const);
    CHECKTYPE(&Sequence::size,       int  (Sequence::*)() const);
    CHECKTYPE(&Sequence::insert,     bool (Sequence::*)(int, const ItemType&));
    CHECKTYPE(&Sequence::insert,     bool (Sequence::*)(const ItemType&));
    CHECKTYPE(&Sequence::erase,      bool (Sequence::*)(int));
    CHECKTYPE(&Sequence::remove,     int  (Sequence::*)(const ItemType&));
    CHECKTYPE(&Sequence::get,        bool (Sequence::*)(int, ItemType&) const);
    CHECKTYPE(&Sequence::set,        bool (Sequence::*)(int, const ItemType&));
    CHECKTYPE(&Sequence::find,       int  (Sequence::*)(const ItemType&) const);
    CHECKTYPE(&Sequence::swap,       void (Sequence::*)(Sequence&));
    CHECKTYPE(subsequence, int  (*)(const Sequence&, const Sequence&));
    CHECKTYPE(interleave,  void (*)(const Sequence&, const Sequence&, Sequence&));
}

int main()
{
}