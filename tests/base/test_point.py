from snake.base import Point, PointType


def test_init():
    p = Point()
    assert p.type == PointType.EMPTY
    p.type = PointType.FOOD
    assert p.type == PointType.FOOD
