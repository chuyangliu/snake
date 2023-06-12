import argparse
import os

from tensorflow.python.tools import inspect_checkpoint as chkp


def main():
    parser = argparse.ArgumentParser(description="Print data in tensorflow checkpoint file.")
    parser.add_argument("global_step", help="global step of the checkpoint file to print")
    parser.add_argument(
        "-n",
        metavar="tensor_name",
        default="",
        help="name of the tensor in the checkpoint file to print " "or print all tensors if not provided.",
    )
    args = parser.parse_args()

    file_name = os.path.join("logs", f"solver-net-{args.global_step}")
    tensor_name = args.n
    all_tensors = args.n == ""

    chkp.print_tensors_in_checkpoint_file(file_name, tensor_name=tensor_name, all_tensors=all_tensors)


if __name__ == "__main__":
    main()
