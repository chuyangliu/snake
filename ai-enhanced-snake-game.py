import argparse
import logging

from snake.game import Game, GameConf, GameMode

def main():
    # Mapping of solvers and modes
    dict_solver = {
        "greedy": "GreedySolver",
        "hamilton": "HamiltonSolver",
        "dqn": "DQNSolver",
    }

    dict_mode = {
        "normal": GameMode.NORMAL,
        "bcmk": GameMode.BENCHMARK,
        "train_dqn": GameMode.TRAIN_DQN,
        "train_dqn_gui": GameMode.TRAIN_DQN_GUI,
    }

    # Setup command-line argument parser
    parser = argparse.ArgumentParser(description="Run snake game agent with various configurations.")
    parser.add_argument(
        "-s",
        default="hamilton",
        choices=dict_solver.keys(),
        help="Name of the solver to direct the snake (default: hamilton)."
    )
    parser.add_argument(
        "-m",
        default="normal",
        choices=dict_mode.keys(),
        help="Game mode (default: normal)."
    )
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="Enable verbose output for debugging purposes."
    )
    parser.add_argument(
        "--custom_config",
        type=str,
        default=None,
        help="Path to a custom configuration file for the game."
    )
    args = parser.parse_args()

    # Setup logging
    logging_level = logging.DEBUG if args.verbose else logging.INFO
    logging.basicConfig(level=logging_level, format='%(asctime)s - %(levelname)s - %(message)s')

    try:
        # Load and apply custom configuration if provided
        conf = GameConf()
        if args.custom_config:
            # Placeholder for custom configuration loading logic
            logging.info(f"Loading custom configuration from {args.custom_config}.")
            # Implement loading logic based on the format of custom_config file
            # Example: conf.load_from_file(args.custom_config)
        else:
            logging.info("Using default configuration.")

        conf.solver_name = dict_solver[args.s]
        conf.mode = dict_mode[args.m]
        logging.info(f"Solver: {conf.solver_name}   Mode: {conf.mode}")

        # Run the game
        Game(conf).run()

    except Exception as e:
        logging.error(f"An error occurred: {e}")
        raise

if __name__ == "__main__":
    main()
