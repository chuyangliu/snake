import numpy as np
import pytest

from snake.util.sumtree import SumTree


def test_sumtree():
    tree = SumTree(8)
    data = (
        ("a", 3),
        ("b", 10),
        ("c", 12),
        ("d", 4),
        ("e", 1),
        ("f", 2),
        ("g", 8),
        ("h", 2),
    )
    for dat in data:
        tree.insert(dat[0], dat[1])

    assert tree.sum() == 42
    assert tree.max_leaf() == 12
    assert tree.min_leaf() == 1

    for i, val in enumerate([42, 29, 13, 13, 16, 3, 10, 3, 10, 12, 4, 1, 2, 8, 2]):
        assert tree.tree[i] == val
        if i > 6:
            assert tree.leaves()[i - 7] == val

    for i, val in enumerate(["a", "b", "c", "d", "e", "f", "g", "h"]):
        assert tree.data[i] == val

    tree_idx, priority, data = tree.retrieve(24)
    assert tree_idx == 9 and priority == 12 and data == "c"

    tree_idx, priority, data = tree.retrieve(33)
    assert tree_idx == 13 and priority == 8 and data == "g"

    tree.update(11, 20)

    for i, val in enumerate([61, 29, 32, 13, 16, 22, 10, 3, 10, 12, 4, 20, 2, 8, 2]):
        assert tree.tree[i] == val
        if i > 6:
            assert tree.leaves()[i - 7] == val

    for i, val in enumerate(["a", "b", "c", "d", "e", "f", "g", "h"]):
        assert tree.data[i] == val

    tree.insert("i", 18)
    tree.insert("j", 19)

    for i, val in enumerate([85, 53, 32, 37, 16, 22, 10, 18, 19, 12, 4, 20, 2, 8, 2]):
        assert tree.tree[i] == val
        if i > 6:
            assert tree.leaves()[i - 7] == val

    for i, val in enumerate(["i", "j", "c", "d", "e", "f", "g", "h"]):
        assert tree.data[i] == val

    assert tree.max_leaf() == 20
    assert tree.min_leaf() == 2


@pytest.mark.skip(reason="may be time-consuming")
def test_probability():
    tree = SumTree(8)
    pairs = (
        ("a", 3),
        ("b", 10),
        ("c", 12),
        ("d", 4),
        ("e", 1),
        ("f", 2),
        ("g", 8),
        ("h", 2),
    )
    for pair in pairs:
        tree.insert(pair[0], pair[1])

    n, counts = 10000000, {}

    for _ in range(n):
        val = np.random.uniform(0, tree.sum())
        _, _, data = tree.retrieve(val)
        if counts.get(data) is None:
            counts[data] = 1
        else:
            counts[data] += 1

    print("Expect    Actual")
    for pair in pairs:
        expect = pair[1] / tree.sum()
        actual = counts[pair[0]] / n
        print(f"{expect:.6f}  {actual:.6f}")

    # Force to show stdout
    assert False
