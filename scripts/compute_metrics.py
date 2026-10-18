import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import pickle

import argparse
import os

from scipy.spatial import ConvexHull
from pathlib import Path

PROGRESS_FILENAME = "metrics.pkl"

ANGLE_DIVISOR     = 15
PREDICTION_WINDOW = [5, 10, 15]
ALGORITHMS        = ["convexhull", "convexhulladaptive"]

FIELD_WIDTH  = 105
FIELD_HEIGHT = 68
FIELD_MAX_X  = (FIELD_WIDTH / 2)
FIELD_MIN_X  = -(FIELD_WIDTH / 2)
FIELD_MAX_Y  = (FIELD_HEIGHT / 2)
FIELD_MIN_Y  = -(FIELD_HEIGHT / 2)
FIELD_AREA   = FIELD_WIDTH * FIELD_HEIGHT


def make_dict():
    return {
        i: { "accuracy": [], "ratio": [] }
        for i in range(1, 12)
    }


def points_in_hull(target_points, hull):

    a = hull.equations[:, :-1]
    b = hull.equations[:, -1]

    return np.all((a @ target_points.T + b[:, None]) <= 1e-12, axis=0)


def clip_polygon_halfplane(polygon, A, B, C):
    """
    Clips polygon with half-plane: Ax + By + C >= 0
    polygon: Nx2 vertices in CCW order
    """

    def inside(p):
        return A*p[0] + B*p[1] + C >= 0

    def intersection(p1, p2):
        x1, y1 = p1
        x2, y2 = p2

        d1 = A*x1 + B*y1 + C
        d2 = A*x2 + B*y2 + C

        t = d1 / (d1 - d2)
        return np.array([
            x1 + t*(x2-x1),
            y1 + t*(y2-y1)
        ])

    output = []
    n = len(polygon)

    for i in range(n):
        curr = polygon[i]
        prev = polygon[i-1]

        curr_in = inside(curr)
        prev_in = inside(prev)

        if curr_in:
            if not prev_in:
                output.append(intersection(prev, curr))
            output.append(curr)

        elif prev_in:
            output.append(intersection(prev, curr))

    return np.array(output)


def clip_to_field(polygon):
    # field left
    polygon = clip_polygon_halfplane(polygon, 1, 0, FIELD_MAX_X)

    # field right
    polygon = clip_polygon_halfplane(polygon, -1, 0, FIELD_MAX_X)

    # field bottom
    polygon = clip_polygon_halfplane(polygon, 0, 1, FIELD_MAX_Y)

    # field top
    polygon = clip_polygon_halfplane(polygon, 0, -1, FIELD_MAX_Y)

    return polygon


parser = argparse.ArgumentParser()

parser.add_argument("--logs", type=str, required=True, help="Path to the input logs")

args = parser.parse_args()

metrics = {
    "metadata": {
        "directory": 0
    }
}
for j in PREDICTION_WINDOW:
    metrics[j] = {
        "convexhull": make_dict(),
        "convexhulladaptive": make_dict()
    }

logs_directories = os.listdir(args.logs)

file = Path(PROGRESS_FILENAME)
if file.exists():
    with open(PROGRESS_FILENAME, 'rb') as f:
        metrics = pickle.load(f)
    print("Found %s. Finished loading previous progress directories=(%d/%d) ..." % (PROGRESS_FILENAME, metrics["metadata"]["directory"], len(logs_directories)))
else:
    print("No progress %s found. Starting from scratch..." % PROGRESS_FILENAME)

begin_directory = metrics["metadata"]["directory"]

for j in range(begin_directory, len(logs_directories)):
    directory = logs_directories[j]
    for i in range(0, len(ALGORITHMS)):
        algorithm_type = ALGORITHMS[i]
        for n in PREDICTION_WINDOW:
            basepath = f"{args.logs}/{directory}"
            rcg_csv = [f for f in os.listdir(basepath) if "HELIOS_base" in f][0]

            gtruth_df = pd.read_csv(f"{basepath}/{rcg_csv}")
            validate_df = pd.read_csv(f"{basepath}/validate/{algorithm_type}_{ANGLE_DIVISOR}_{n}.csv")

            for player in range(1, 12):
                df = validate_df[validate_df["player"] == player]

                for key, group in df.groupby("cycle"):
                    points = group[["x", "y"]].to_numpy()
                    assert len(points) > 2

                    clipped_points = clip_to_field(points)
                    if len(clipped_points) == 0:
                        accuracy = 0
                        hull = ConvexHull(points)
                        ratio = hull.volume / FIELD_AREA
                        metrics[n][algorithm_type][player]["ratio"].append(ratio)
                        metrics[n][algorithm_type][player]["accuracy"].append(accuracy)
                        continue

                    assert len(clipped_points) > 2

                    hull = ConvexHull(clipped_points)

                    truth = gtruth_df[[f" l{player}_x", f" l{player}_y", " cycle"]]
                    truth = truth[(truth[" cycle"] > key) & (truth[" cycle"] <= key + n)]
                    truth = truth[[f" l{player}_x", f" l{player}_y"]]

                    if truth.shape[0] != n:
                        continue

                    in_hull = points_in_hull(truth, hull)
                    accuracy = np.sum(in_hull) / len(in_hull)

                    ratio = hull.volume / FIELD_AREA

                    metrics[n][algorithm_type][player]["ratio"].append(ratio)
                    metrics[n][algorithm_type][player]["accuracy"].append(accuracy)

    with open(PROGRESS_FILENAME, 'wb') as f:
        metrics["metadata"]["directory"] = j + 1
        pickle.dump(metrics, f)
        print("Dumped progress directories=(%d/%d)" % (metrics["metadata"]["directory"], len(logs_directories)))

