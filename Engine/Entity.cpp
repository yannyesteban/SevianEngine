#include "Entity.h"

namespace SEVIAN {

    /*
	template <typename T, typename... Args>
	void Entity::addComponent ( Args&&... args ) {
		components[std::type_index ( typeid(T) )] = std::make_unique<T> ( std::forward<Args> ( args )... );
	}
    
    template <typename T>
    T* Entity::getComponent () {
        auto it = components.find ( std::type_index ( typeid(T) ) );
        if (it != components.end ()) {
            return static_cast<T*>(it->second.get ());
        }
        return nullptr;
    }

    template <typename T>
    bool Entity::hasComponent () {
        return components.find ( std::type_index ( typeid(T) ) ) != components.end ();
    }
    */
}