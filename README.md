# Differential Power Analysis Attack

During the labs, the DPA attack was implemented in Python language.
We thought the language's performance was low since it took up to one minute to calculate a single key.
We wanted to compare these performance results with the same algorithm but in C language.

## Usage

The proposed Makefile has several functionalities.
It can compile the C source files, extract the traces and texts or perform the whole DPA attack.

 - `make`: Compiles the C source files and generate the executable `./main`.
 - `make extract-numpy`: Calls the Python script to extracts the traces and texts into .csv files.
 - `make DPA`: Compiles source files, extracts the traces and texts and calls `./main`.

## Results

The traces are still extracted with Numpy and a small Python script.
However, we do not take the extraction into account for both performance analysis.
The following tests were realised on the same machine (Ubuntu 22.04, python3 3.10.6, gcc 11.2.0).

| Language  | Average time for one key (s) |
| --------- | ---------------------------- | 
| `Python`  | 57                           |
| `C`       | 11                           |

The implementation in C is five times faster compared to Python.

## To go further ...

We think that the performance obtained with the C implementation could be approved with multi-threading.
As of now, the sub-keys are calculated one after the other.
We could have sixteen threads running at the same time calculating the sub-keys.