#include "Components.h"


bool BubbleLeader::areGroupLeaderInitialized() {
        return groupLeaderInitialized;
    }
    void BubbleLeader::initGroupLeader(int initialLeader) {
        groupLeader[0] = initialLeader;
        groupLeader[1] = initialLeader;

        groupLeaderInitialized = true;
    }

    int BubbleLeader::getLeader() {
        return groupLeader[currentGroupLeaderIndex];
    }
    void BubbleLeader::setLeader(int leader) {
        groupLeader[currentGroupLeaderIndex] = leader;
    }
    bool BubbleLeader::updateLeaderSwitchCounterAndSwitch() {
        timeToSwitchGroupLeaderIndex--;
        if (timeToSwitchGroupLeaderIndex == 0) {
            timeToSwitchGroupLeaderIndex = FRAME_COUNT_TILL_SWITCHING_GROUP_LEADER_INDEX;
            currentGroupLeaderIndex = 1 - currentGroupLeaderIndex;

            return true;

        } else {
            return false;
        }
    }

    bool BubbleLeader::sharesLeaderWith(const BubbleLeader &other) {
        return std::min(groupLeader[0], groupLeader[1]) == std::min(other.groupLeader[0], other.groupLeader[1]);
    }