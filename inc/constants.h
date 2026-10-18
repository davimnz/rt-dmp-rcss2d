#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define VELOCITY_DECAY            (0.4)
#define VELOCITY_DECAY_DELTA_MAX  (0.1)
#define VELOCITY_DECAY_DELTA_MIN  (-0.1)
#define MAX_VELOCITY              (1.05)
#define MAX_VELOCITY_DECAY        (VELOCITY_DECAY + VELOCITY_DECAY_DELTA_MAX)
#define MIN_VELOCITY_DECAY        (VELOCITY_DECAY + VELOCITY_DECAY_DELTA_MIN)
#define DASH_POWER_RATE           (0.006)
#define DASH_POWER_RATE_DELTA_MAX (0.0008)
#define DASH_POWER_RATE_DELTA_MIN (-0.0012)
#define MAX_DASH_POWER_RATE       (DASH_POWER_RATE + DASH_POWER_RATE_DELTA_MAX)
#define MIN_DASH_POWER_RATE       (DASH_POWER_RATE + DASH_POWER_RATE_DELTA_MIN)
#define DASH_POWER_RATE_BACK      (0.7)
#define DASH_POWER_RATE_SIDE      (0.4)
#define MAX_STAMINA_EFFORT        (1.0)
#define MIN_STAMINA_EFFORT        (0.4)
#define MAX_DASH_POWER            (100)
#define MAX_ACCELERATION          (MAX_DASH_POWER * \
                                   MAX_STAMINA_EFFORT * \
                                   MAX_DASH_POWER_RATE * \
                                   1.0)
#define FIELD_HALF_WIDTH          (52.5)
#define FIELD_HALF_LENGTH         (34.0)
#define REDUCED_MAX_ACCELERATION  (1.0 * MAX_ACCELERATION)
#define MAX_ALLOWED_ANGLE         (180.0)
#define MIN_ALLOWED_ANGLE         (-180.0)

#endif
