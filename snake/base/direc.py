from enum import Enum, unique


@unique
class Direc(Enum):
    NONE = 0
    LEFT = 1
    UP = 2
    RIGHT = 3
    DOWN = 4

    @staticmethod
    def opposite(direc):
        if direc == Direc.LEFT:
            return Direc.RIGHT
        if direc == Direc.RIGHT:
            return Direc.LEFT
        if direc == Direc.UP:
            return Direc.DOWN
        if direc == Direc.DOWN:
            return Direc.UP
        return Direc.NONE
