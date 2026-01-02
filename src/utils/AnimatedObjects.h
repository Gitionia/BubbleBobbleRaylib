#pragma once

#include <vector>

template <typename T>
struct AnimatedValueVectorObject {
    T Value;
    int Index;
};

template <typename T>
using AnimatedValueDefinition = std::vector<AnimatedValueVectorObject<T>>;

template <typename T>
class AnimatedValue {

  public:
    AnimatedValue(const AnimatedValueDefinition<T> &frames) : frames(frames) {
        length = 0;
        for (auto o : frames) {
            length += o.Index;
        }
    }

    void tick() {
        frame++;
    
        if (frame == frames.at(currentIndex).Index) {
            frame = 0;
            currentIndex++;
            if (currentIndex >= frames.size()) {
                currentIndex = frames.size() - 1;
            }
        }
        printf("index %i frame %i \n", currentIndex, frame);
    }

    T get() {
        return frames.at(currentIndex).Value;
    }

    void reset() {
        frame = 0;
        currentIndex = 0;
    }

    int getLength() {
        return length;
    }

  private:
    int frame = 0;
    int currentIndex = 0;
    int length;
    const AnimatedValueDefinition<T> &frames;
};
