import random
from collections import deque

from snake.base.direc import Direc
from snake.base.point import PointType
from snake.base.pos import Pos


class Snake:
    """Snake of the game."""

    def __init__(self, game_map, init_direc=None, init_bodies=None, init_types=None):
        """Initialize a Snake object.

        Args:
            game_map (base.map.Map): The map that the snake moves on.
            init_direc (base.direc.Direc): Initial direction.
            init_bodies (list of base.pos.Pos): Initial snake bodies positions.
            init_types (list of base.point.PointType): Types of each position in init_bodies.

        """
        self._map = game_map
        self._init_direc = init_direc
        self._init_bodies = init_bodies
        self._init_types = init_types
        self.reset(False)

    def reset(self, reset_map=True):
        rand_init = False
        if self._init_direc is None:  # Randomly initialize
            rand_init = True
            head_row = random.randrange(2, self._map.num_rows - 2)
            head_col = random.randrange(2, self._map.num_cols - 2)
            head = Pos(head_row, head_col)

            self._init_direc = random.choice(
                [Direc.LEFT, Direc.UP, Direc.RIGHT, Direc.DOWN]
            )
            self._init_bodies = [head, head.adj(Direc.opposite(self._init_direc))]

            self._init_types = []
            if self._init_direc == Direc.LEFT:
                self._init_types.append(PointType.HEAD_L)
            elif self._init_direc == Direc.UP:
                self._init_types.append(PointType.HEAD_U)
            elif self._init_direc == Direc.RIGHT:
                self._init_types.append(PointType.HEAD_R)
            elif self._init_direc == Direc.DOWN:
                self._init_types.append(PointType.HEAD_D)
            if self._init_direc == Direc.LEFT or self._init_direc == Direc.RIGHT:
                self._init_types.append(PointType.BODY_HOR)
            elif self._init_direc == Direc.UP or self._init_direc == Direc.DOWN:
                self._init_types.append(PointType.BODY_VER)

        self._steps = 0
        self._dead = False
        self._direc = self._init_direc
        self._direc_next = Direc.NONE
        self._bodies = deque(self._init_bodies)

        if reset_map:
            self._map.reset()
        for i, pos in enumerate(self._init_bodies):
            self._map.point(pos).type = self._init_types[i]

        if rand_init:
            self._init_direc = self._init_bodies = self._init_types = None

    def copy(self):
        m_copy = self._map.copy()
        s_copy = Snake(m_copy, Direc.NONE, [], [])
        s_copy._steps = self._steps
        s_copy._dead = self._dead
        s_copy._direc = self._direc
        s_copy._direc_next = self._direc_next
        s_copy._bodies = deque(self._bodies)
        return s_copy, m_copy

    @property
    def map(self):
        return self._map

    @property
    def steps(self):
        return self._steps

    @property
    def dead(self):
        return self._dead

    @dead.setter
    def dead(self, val):
        self._dead = val

    @property
    def direc(self):
        return self._direc

    @property
    def direc_next(self):
        return self._direc_next

    @direc_next.setter
    def direc_next(self, val):
        self._direc_next = val

    @property
    def bodies(self):
        return self._bodies

    def len(self):
        return len(self._bodies)

    def head(self):
        if not self._bodies:
            return None
        return self._bodies[0]

    def tail(self):
        if not self._bodies:
            return None
        return self._bodies[-1]

    def move_path(self, path):
        for p in path:
            self.move(p)

    def move(self, new_direc=None):
        if new_direc is not None:
            self._direc_next = new_direc

        if (
            self._dead
            or self._direc_next == Direc.NONE
            or self._map.is_full()
            or self._direc_next == Direc.opposite(self._direc)
        ):
            return

        old_head_type, new_head_type = self._new_types()
        self._map.point(self.head()).type = old_head_type
        new_head = self.head().adj(self._direc_next)
        self._bodies.appendleft(new_head)

        if not self._map.is_safe(new_head):
            self._dead = True
        if self._map.point(new_head).type == PointType.FOOD:
            self._map.rm_food()
        else:
            self._rm_tail()

        self._map.point(new_head).type = new_head_type
        self._direc = self._direc_next
        self._steps += 1

    def _rm_tail(self):
        self._map.point(self.tail()).type = PointType.EMPTY
        self._bodies.pop()

    def _new_types(self):
        old_head_type, new_head_type = None, None
        # new_head_type
        if self._direc_next == Direc.LEFT:
            new_head_type = PointType.HEAD_L
        elif self._direc_next == Direc.UP:
            new_head_type = PointType.HEAD_U
        elif self._direc_next == Direc.RIGHT:
            new_head_type = PointType.HEAD_R
        elif self._direc_next == Direc.DOWN:
            new_head_type = PointType.HEAD_D
        # old_head_type
        if (self._direc == Direc.LEFT and self._direc_next == Direc.LEFT) or (
            self._direc == Direc.RIGHT and self._direc_next == Direc.RIGHT
        ):
            old_head_type = PointType.BODY_HOR
        elif (self._direc == Direc.UP and self._direc_next == Direc.UP) or (
            self._direc == Direc.DOWN and self._direc_next == Direc.DOWN
        ):
            old_head_type = PointType.BODY_VER
        elif (self._direc == Direc.RIGHT and self._direc_next == Direc.UP) or (
            self._direc == Direc.DOWN and self._direc_next == Direc.LEFT
        ):
            old_head_type = PointType.BODY_LU
        elif (self._direc == Direc.LEFT and self._direc_next == Direc.UP) or (
            self._direc == Direc.DOWN and self._direc_next == Direc.RIGHT
        ):
            old_head_type = PointType.BODY_UR
        elif (self._direc == Direc.LEFT and self._direc_next == Direc.DOWN) or (
            self._direc == Direc.UP and self._direc_next == Direc.RIGHT
        ):
            old_head_type = PointType.BODY_RD
        elif (self._direc == Direc.RIGHT and self._direc_next == Direc.DOWN) or (
            self._direc == Direc.UP and self._direc_next == Direc.LEFT
        ):
            old_head_type = PointType.BODY_DL
        return old_head_type, new_head_type


import pygame
import random

# Initialize pygame
pygame.init()

# Constants
WIDTH, HEIGHT = 500, 500
CELL_SIZE = 20
WHITE, GREEN, RED, BLACK = (255, 255, 255), (0, 255, 0), (255, 0, 0), (0, 0, 0)

# Set up display
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Snake Game AI")

# Snake and Food Initialization
snake = [(100, 100)]
direction = (CELL_SIZE, 0)
food = (random.randrange(0, WIDTH, CELL_SIZE), random.randrange(0, HEIGHT, CELL_SIZE))

# AI Logic: Moves towards food
def move_towards_food():
    global direction
    head_x, head_y = snake[0]
    food_x, food_y = food

    if head_x < food_x:
        direction = (CELL_SIZE, 0)  # Move Right
    elif head_x > food_x:
        direction = (-CELL_SIZE, 0)  # Move Left
    elif head_y < food_y:
        direction = (0, CELL_SIZE)  # Move Down
    elif head_y > food_y:
        direction = (0, -CELL_SIZE)  # Move Up

# Game Loop
running = True
clock = pygame.time.Clock()
while running:
    pygame.time.delay(100)  # Control speed
    screen.fill(BLACK)

    # Move Snake
    move_towards_food()
    new_head = (snake[0][0] + direction[0], snake[0][1] + direction[1])
    snake.insert(0, new_head)

    # Check Collision with Food
    if new_head == food:
        food = (random.randrange(0, WIDTH, CELL_SIZE), random.randrange(0, HEIGHT, CELL_SIZE))
    else:
        snake.pop()  # Remove tail if no food eaten

    # Draw Snake and Food
    for part in snake:
        pygame.draw.rect(screen, GREEN, (*part, CELL_SIZE, CELL_SIZE))
    pygame.draw.rect(screen, RED, (*food, CELL_SIZE, CELL_SIZE))

    pygame.display.update()

    # Check for collisions (wall or self)
    if (new_head[0] < 0 or new_head[0] >= WIDTH or
        new_head[1] < 0 or new_head[1] >= HEIGHT or
        new_head in snake[1:]):
        running = False  # End game if snake crashes

pygame.quit()
