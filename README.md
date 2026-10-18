# Computing Reachable Regions for Real-Time Decision Making in Simulated Robot Soccer
This is the source code for the paper "Computing Reachable Regions for Real-Time Decision Making in Simulated Robot Soccer".

A demonstration video is available at the following [link](https://zenodo.org/records/21747705)

## Setup
You need at least `build-essential` to build and run the `.cpp` files.

Install the requirements for Python plotting:
```bash
pip install -r requirements.txt
```

The following soccer simulator and HELIOS base team are used in this work:

- [rcssserver-19.0.0](https://github.com/rcsoccersim/rcssserver/releases/tag/rcssserver-19.0.0)
- [helios-base-#835e399](https://github.com/helios-base/helios-base/tree/835e3997bbe60eb76d7dc8828df373b175614fb0)

## Method Configuration
Each `.cpp` file has a `#define` that specifies which method will be used at runtime. Select the one you want to run.

The following three methods are available:

```c
/* Naive Method (N) */
#define METHOD NaiveEstimate

/* Convex Hull Method (C) */
#define METHOD ConvexHullEstimate

/* Adaptive Convex Hull Method (CA) */
#define METHOD ConvexHullAdaptiveRateEstimate
```

Note: The naive method may take too long to execute or may even crash the program due to OOM (Out of Memory), depending on the prediction window size.

## Reproducibility
To run the experiments from the paper, execute the following instructions.

### Execution Time

```bash
make build-execution-time
./sh/run_execution_time_suite.sh
python scripts/plot_execution_time.py
open execution_time.pdf
```

### Number of Computed States

```bash
make build-hull-sizes
./sh/run_hull_sizes_suite.sh
python scripts/plot_hull_sizes.py
open hull_sizes.pdf
```

### Accuracy and Area Ratio
Extract the `rcg` log files first:

```bash
mkdir logs/rcg && tar -xzf logs/rcgcsv.tar.gz -C logs/rcg
```

Repeat the following sequence for both Method C and Method CA.
```bash
make build-validation
./sh/run_validation_suite.sh logs/rcg 15 5
./sh/run_validation_suite.sh logs/rcg 15 10
./sh/run_validation_suite.sh logs/rcg 15 15
```

Compute and plot the metrics:
```bash
python scripts/compute_metrics.py --logs logs/rcg
python scripts/plot_metrics.py
open metrics.pdf
```
