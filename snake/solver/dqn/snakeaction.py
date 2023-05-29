from enum import Enum, unique

from snake.base.direc import Direc


@unique
class SnakeAction(Enum):
    """Snake actions for DQN."""
    LEFT = 0
    FORWARD = 1
    RIGHT = 2

    @staticmethod
    def to_direc(action, cur_direc):
        """Return the direction wrt the action."""
        if action == SnakeAction.FORWARD:
            return cur_direc
        if action == SnakeAction.LEFT:
            if cur_direc == Direc.LEFT:
                return Direc.DOWN
            if cur_direc == Direc.UP:
                return Direc.LEFT
            if cur_direc == Direc.RIGHT:
                return Direc.UP
            if cur_direc == Direc.DOWN:
                return Direc.RIGHT
        if action == SnakeAction.RIGHT:
            if cur_direc == Direc.LEFT:
                return Direc.UP
            if cur_direc == Direc.UP:
                return Direc.RIGHT
            if cur_direc == Direc.RIGHT:
                return Direc.DOWN
            if cur_direc == Direc.DOWN:
                return Direc.LEFT
        return Direc.NONE
