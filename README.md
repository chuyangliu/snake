# Snake

[![][badge-travis]][build-travis] [![][badge-appveyor]][build-appveyor] ![][badge-python]

The project focuses on the artificial intelligence of the [Snake][wiki-snake] game. The snake's goal is to eat the food continuously and fill the map with its bodies as soon as possible. Originally, the project was [written in C++][snake-proj-old]. It has now been rewritten in Python for a user-friendly GUI and the simplicity in algorithm implementations.

***[Algorithms >][doc-algorithms]***

## Experiments

We have used two metrics to evaluate the performance of an AI:

1. **Average Length:** Average length the snake has grown to (*max:* 64).
2. **Average Steps:** Average steps the snake has moved.

Test results (averaged over 1000 episodes):

| Solver | Demo (optimal) | Average Length | Average Steps |
| :----: | :------------: | :------------: | :-----------: |
|[Hamilton][doc-hamilton]|![][demo-hamilton]|63.93|717.83|
|[Greedy][doc-greedy]|![][demo-greedy]|60.15|904.56|
|[DQN][doc-dqn]<br>(experimental)|![][demo-dqn]|24.44|131.69|

## Installation

Requirements: Python 3.5+ (64-bit) with [Tkinter][doc-tkinter] installed.

```
$ pip3 install -r requirements.txt

# Use -h for more details
$ python3 run.py [-h]
```

Run unit tests:

```
$ python3 -m pytest -v
```

## License

See the [LICENSE](./LICENSE.md) file for license rights and limitations.


[snake-proj-old]: https://github.com/chuyangliu/Snake/tree/7227f5e0f3185b07e9e3de1ac5c19a17b9de3e3c

[build-travis]: https://travis-ci.org/chuyangliu/snake
[build-appveyor]: https://ci.appveyor.com/project/chuyangliu/snake/branch/master

[badge-travis]: https://travis-ci.org/chuyangliu/snake.svg?branch=master
[badge-appveyor]: https://ci.appveyor.com/api/projects/status/ew63pr1vb7ee1yyi/branch/master?svg=true
[badge-python]: https://img.shields.io/badge/python-3.5+-blue.svg

[wiki-snake]: https://en.wikipedia.org/wiki/Snake_(video_game)
[doc-tkinter]: https://docs.python.org/3.6/library/tkinter.html
[doc-algorithms]: ./docs/algorithms.md
[doc-greedy]: ./docs/algorithms.md#greedy-solver
[doc-hamilton]: ./docs/algorithms.md#hamilton-solver
[doc-dqn]: ./docs/algorithms.md#dqn-solver

[demo-hamilton]: ./docs/images/solver_hamilton.gif
[demo-greedy]: ./docs/images/solver_greedy.gif
[demo-dqn]: ./docs/images/solver_dqn.gif
