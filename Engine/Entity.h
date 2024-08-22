#pragma once
#include <iostream>
#include <unordered_map>
#include <typeindex>
#include "Component.h"


namespace SEVIAN {

	using EntityID = std::size_t;
	using ComponentID = std::type_index;



	class Entity
	{
	private:
		EntityID id;
		std::unordered_map<ComponentID, std::unique_ptr<Component>> components;

	public:
		Entity ( EntityID id ) : id ( id ) { }
		EntityID getID () const { return id; }

		template <typename T, typename... Args>
		void addComponent ( Args&&... args ) {
			components[std::type_index ( typeid(T) )] = std::make_unique<T> ( std::forward<Args> ( args )... );
		}


		template <typename T>
		T* getComponent () {
			auto it = components.find ( std::type_index ( typeid(T) ) );
			if (it != components.end ()) {
				return static_cast<T*>(it->second.get ());
			}
			return nullptr;
		}

		template <typename T>
		bool hasComponent () {
			return components.find ( std::type_index ( typeid(T) ) ) != components.end ();
		}
	};


};
