#include "EventSystem.h"
#include <memory>
#include <vector>

EventSystem::EventSystem() {
    for (int i = 0; i < EventType_Count; i++) {
        eventLists.at(i).reserve(EVENT_LIST_RESERVATION);
    }
}

void EventSystem::Notify(entt::entity sender, EventType type, std::shared_ptr<void> data) {
    eventLists.at(type).push_back({sender, data});
}

const std::vector<Event> &EventSystem::ReadEvent(EventType type) {
    return eventLists.at(type);
}

void EventSystem::Clear() {
    for (auto& eventList : eventLists) {
        eventList.clear();
    }
}