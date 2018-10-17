#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# pylint: disable=C0103,C0111

"""Definitions of class Pos."""
import attr

from snake.base.direc import Direc


@attr.s(hash=True)
class Pos:
    """Integer coordinate in 2D plane.

    The origin of the coordinate system is at the top-left corner,
    with x-axis extends downward and y-axis extends rightward.

    """
    x = attr.ib(default=0)
    y = attr.ib(default=0)

    def __pos__(self):
        return Pos(self.x, self.y)

    def __neg__(self):
        return Pos(-self.x, -self.y)

    def __add__(self, other):
        return Pos(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return self + (-other)

    @staticmethod
    def manhattan_dist(p1, p2):
        return abs(p1.x - p2.x) + abs(p1.y - p2.y)

    def direc_to(self, adj_pos):
        """Return the direction of an adjacent Pos relative to self."""
        real_diff = adj_pos - self
        return _ADJ_DIFF_TO_DIR.get(real_diff, Direc.NONE)

    def adj(self, direc):
        """Return the adjacent Pos in a given direction."""
        adj_diff = _DIR_TO_ADJ_DIFF.get(direc, None)
        if adj_diff is not None:
            return self + adj_diff

    def all_adj(self):
        """Return a list of all the adjacent Pos."""
        return list(map(self.adj, Direc.valid()))


_DIR_AND_ADJ_DIFF = (
    (Direc.LEFT, Pos(0, -1)),
    (Direc.RIGHT, Pos(0, 1)),
    (Direc.DOWN, Pos(1, 0)),
    (Direc.UP, Pos(-1, 0)),
)

_DIR_TO_ADJ_DIFF = dict(_DIR_AND_ADJ_DIFF)
_ADJ_DIFF_TO_DIR = {adj_diff: dir_ for dir_, adj_diff
                    in _DIR_AND_ADJ_DIFF}
