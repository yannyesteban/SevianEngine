#pragma once
#include "Event.h"
#include <memory>
#include <map>

namespace SEVIAN {

    class EventListener
    {
    public:
        virtual ~EventListener () = default;
        virtual bool handleEvent ( const Event& event ) = 0;
    };

    // Implementaci�n de EventListener basada en funci�n
    class EventListenerFunction : public EventListener
    {
    private:
        std::function<bool ( const Event& )> callback;
    public:
        EventListenerFunction ( std::function<bool ( const Event& )> callback ) : callback ( callback ) { }
        bool handleEvent ( const Event& event ) override {
            return callback ( event );
        }
    };

    class EventTarget
    {
    protected:
        struct ListenerInfo
        {
            std::shared_ptr<EventListener> listener;
            bool useCapture;
            bool once;
        };

        // Mapa de tipo de evento a lista de listeners
        std::map<EventType, std::vector<ListenerInfo>> listeners;

    public:
        virtual ~EventTarget () = default;

        // M�todo para a�adir un listener
        void addEventListener ( EventType type, std::shared_ptr<EventListener> listener, bool useCapture = false, bool once = false ) {
            // Verificar si ya existe este listener exacto para evitar duplicados
            auto& listenersList = listeners[type];
            auto it = std::find_if ( listenersList.begin (), listenersList.end (),
                [&]( const ListenerInfo& info ) {
                    return info.listener == listener && info.useCapture == useCapture;
                } );

            if (it == listenersList.end ()) {
                listenersList.push_back ( { listener, useCapture, once } );
            }
        }

        // Sobrecarga para funciones lambda o funciones normales
        void addEventListener ( EventType type, std::function<bool ( const Event& )> callback, bool useCapture = false, bool once = false ) {
            auto listener = std::make_shared<EventListenerFunction> ( callback );
            addEventListener ( type, listener, useCapture, once );
        }

        // Eliminar un listener
        void removeEventListener ( EventType type, std::shared_ptr<EventListener> listener, bool useCapture = false ) {
            auto it = listeners.find ( type );
            if (it != listeners.end ()) {
                auto& listenersList = it->second;
                listenersList.erase (
                    std::remove_if ( listenersList.begin (), listenersList.end (),
                        [&]( const ListenerInfo& info ) {
                            return info.listener == listener && info.useCapture == useCapture;
                        } ),
                    listenersList.end ()
                );
            }
        }

        // Sobrecarga para funciones
        void removeEventListener ( EventType type, std::function<bool ( const Event& )> callback, bool useCapture = false ) {
            // Esta implementaci�n es m�s compleja porque necesitar�amos comparar las funciones,
            // lo cual no es posible directamente. Una posible soluci�n es mantener un registro
            // de las funciones y sus listeners asociados.
        }

        // M�todo para disparar un evento
        virtual bool dispatchEvent ( Event& event ) {
            // M�todo virtual que debe ser implementado por las clases derivadas
            return false;
        }
    };
}


