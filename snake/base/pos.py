#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# pylint: disable=C0103,C0111

"""Definitions of class Pos."""

from snake.base.direc import Direc


class Pos:
    """Integer coordinate in 2D plane.

    The origin of the coordinate system is at the top-left corner,
    with x-axis extends downward and y-axis extends rightward.

    """

    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

    def __str__(self):
        return 'Pos(%d,%d)' % (self.x, self.y)
    __repr__ = __str__

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __pos__(self):
        return Pos(self.x, self.y)

    def __neg__(self):
        return Pos(-self.x, -self.y)

    def __add__(self, other):
        return Pos(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return self + (-other)

    def __hash__(self):
        return hash((self.x, self.y))

    @staticmethod
    def manhattan_dist(p1, p2):
        return abs(p1.x - p2.x) + abs(p1.y - p2.y)

    def direc_to(self, adj_pos):
        """Return the direction of an adjacent Pos relative to self."""
        adj_diff_to_dir = {adj_diff: dir_ for dir_, adj_diff
                           in _dir_and_adj_diff}
        real_diff = adj_pos - self
        return adj_diff_to_dir.get(real_diff, Direc.NONE)

    def adj(self, direc):
        """Return the adjacent Pos in a given direction."""
        dir_to_adj_diff = dict(_dir_and_adj_diff)
        adj_diff = dir_to_adj_diff.get(direc, None)
        if adj_diff is not None:
            return self + adj_diff

    def all_adj(self):
        """Return a list of all the adjacent Pos."""
        return list(map(self.adj, Direc.valid()))


_dir_and_adj_diff = (
    (Direc.LEFT, Pos(0, -1)),
    (Direc.RIGHT, Pos(0, 1)),
    (Direc.DOWN, Pos(1, 0)),
    (Direc.UP, Pos(-1, 0)),
)
