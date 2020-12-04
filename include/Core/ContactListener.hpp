#pragma once

#include <box2d/b2_contact.h>
#include <box2d/b2_world_callbacks.h>

class ContactListener : public b2ContactListener {
private:
    void BeginContact(b2Contact *contact) final;
};