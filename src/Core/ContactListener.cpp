#include <Core/ContactListener.hpp>

void ContactListener::BeginContact(b2Contact *contact) {
    Collider *c1 = contact->GetFixtureA()->GetBody()->GetUserData();
    Collider *c2 = contact->GetFixtureB()->GetBody()->GetUserData();
    if (c1 != nullptr && c2 != nullptr) {
        c1->hit(c2);
        c2->hit(c1);
    }
}
