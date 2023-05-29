import argparse

from snake.solver.dqn.history import History


def main():
    parser = argparse.ArgumentParser(description="Plot history data of dqn training.")
    parser.add_argument("beg_step", type=int, help="starting learning step of the history data")
    parser.add_argument("end_step", type=int, help="ending learning step of the history data")
    args = parser.parse_args()

    his = History(num_avg=0)
    his.load(args.beg_step, args.end_step)
    his.plot(args.beg_step)


if __name__ == "__main__":
    main()
