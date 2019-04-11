
#ifndef COLLISION__HANDLER_HPP
#define COLLISION__HANDLER_HPP

#include <assert.h>

#include "Box2D/Dynamics/b2WorldCallbacks.h"

class CollisionHandler : public b2ContactListener
{
    public:

    CollisionHandler();
    ~CollisionHandler();
    
    private:

    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;

};

#endif