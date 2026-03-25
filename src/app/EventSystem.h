#pragma once

#include "entt/entity/fwd.hpp"
#include <memory>
#include <vector>
enum EventType {
    POINTS_GAINED,
    EventType_Count
};

struct Event {
    entt::entity sender;
    std::shared_ptr<void> data;
};

class EventSystem {

public:
    EventSystem();

    // Notifies system of an event that occured this frame
    void Notify(entt::entity sender, EventType type, const std::shared_ptr<void> data);

    // Gets all the events of a type that were raised this frame
    const std::vector<Event>& ReadEvent(EventType type);

    // Clears all the events (should happen every frame)
    void Clear();

private:
    std::array<std::vector<Event>, EventType_Count> eventLists;
    int EVENT_LIST_RESERVATION = 20;
};