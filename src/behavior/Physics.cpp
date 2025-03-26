#include "Physics.h"

bool collidesWithWall(entt::registry &registry, const Position &position, const Collider &collider) {
	const auto view = registry.view<Position, Collider, LevelTileTag>();

	for (const auto entity : view) {
		const auto [pos, col] = view.get(entity);

		if (overlaps(position, collider, pos, col)) {
			return true;
		}
	}

	return false;
}

bool overlaps(const Position &pos1, const Collider &col1, const Position &pos2, const Collider &col2) {
	int xmin1 = pos1.x + col1.offsetX;
	int xmax1 = xmin1 + col1.width;
	int ymin1 = pos1.y + col1.offsetY;
	int ymax1 = ymin1 + col1.height;
	int xmin2 = pos2.x + col2.offsetX;
	int xmax2 = xmin2 + col2.width;
	int ymin2 = pos2.y + col2.offsetY;
	int ymax2 = ymin2 + col2.height;

	return xmax1 > xmin2 && xmax2 > xmin1 &&
		ymax1 > ymin2 && ymax2 > ymin1;
}