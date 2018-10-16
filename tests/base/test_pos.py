#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# pylint: disable=C0103,C0111

"""Unit tests for class Pos."""
import pytest
from snake.base import Direc, Pos


def test_init():
    p = Pos(-5, 5)
    assert p == Pos(-5, 5)
    p.x = -10
    p.y = 10
    assert p != Pos(-5, 5)
    assert p == Pos(-10, 10)


def test_arithmetic():
    p1 = Pos(-5, 10)
    p2 = Pos(5, -10)
    p3 = p1 + p2
    p4 = p1 - p2
    p5 = p2 - p1
    assert p3 == Pos(0, 0)
    assert p3 - p1 == p2
    assert p3 - p2 == p1
    assert p4 == Pos(-10, 20)
    assert p5 == -Pos(-10, 20)
    assert p4 + p2 == p1
    assert p5 + p1 == p2


def test_dist():
    p1 = Pos(-5, 20)
    p2 = Pos(10, 8)
    assert Pos.manhattan_dist(p1, p2) == 27


def test_all_adj():
    p = Pos(0, 0)
    directions = set(map(p.direc_to, p.all_adj()))
    assert directions == set(Direc.valid())


@pytest.mark.parametrize("direc", Direc.valid())
def test_bijection_adj_and_direc_to(direc):
    pos = Pos(10, 10)
    adj_pos = pos.adj(direc)
    direc_adj_to_pos = pos.direc_to(adj_pos)
    assert direc == direc_adj_to_pos
