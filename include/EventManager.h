#pragma once

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <functional>
#include <algorithm>

// EventManager: sistema central de eventos desacoplados
class EventManager {
public:
    using Callback = std::function<void(const void*)>;   // Acción que se ejecuta al emitir un evento
    using Listener = std::pair<void*, Callback>;         // Objeto + acción asociada

    // --------------------------------------------------
    // Acceso global (Singleton)
    // --------------------------------------------------
    static EventManager& instance() {
        static EventManager instance; // Instancia única
        return instance;
    }

    // --------------------------------------------------
    // Registrar un objeto para escuchar un evento
    // --------------------------------------------------
    template<typename Event, typename Object>
    void subscribe(Object* object, void (Object::* method)(const Event&)) {
        auto eventType = std::type_index(typeid(Event));     // Tipo del evento
		Callback callback = [object, method](const void* evt) {  //Callback=Apuntador a función miembro
            const Event* event = static_cast<const Event*>(evt); // Convertir evento genérico
            (object->*method)(*event);                           // Ejecutar método del objeto
            };
		// Acceso al hashmap por indice de tipo de evento que devuelve un vector de listeners
		// En el vector de listeners se agrega un par (objeto, callback)
		// Cada tipo de evento tiene su propia vector de listeners
        listeners[eventType].push_back({ object, callback });    // Guardar listener
    }

    // --------------------------------------------------
    // Cancelar la suscripción de un objeto a un evento
    // --------------------------------------------------
    template<typename Event>
    void unsubscribe(void* object) {
        auto eventType = std::type_index(typeid(Event));     // Tipo del evento
        auto it = listeners.find(eventType);                // Buscar lista de oyentes
		if (it == listeners.end()) return;                  // Si no hay listeners, salir

        auto& list = it->second;                             // Lista de listeners
        //Remueve y borra un listener si el objeto coincide
        list.erase(std::remove_if(list.begin(), list.end(),
			[object](const Listener& l) { return l.first == object; }), 
            list.end());
    }

    // --------------------------------------------------
    // Emitir un evento a todos los oyentes
    // --------------------------------------------------
    template<typename Event>
    void emit(const Event& event) {
        auto eventType = std::type_index(typeid(Event));    // Tipo del evento
        auto it = listeners.find(eventType);                // Buscar oyentes
		if (it == listeners.end()) return;                  // Si no hay listeners, salir

		for (const Listener& l : it->second)                // Recorre la lista de listeners
            l.second(&event);                               // Ejecutar callback
    }

private:
	// Mapa de eventos a sus listeners 
	// Un Listener es un par (objeto, apuntador a función)
    std::unordered_map<std::type_index, std::vector<Listener>> listeners;
};
