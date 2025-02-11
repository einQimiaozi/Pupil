#pragma once

#define PUPIL_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define PUPIL_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define PUPIL_PIN(a, min_value, max_value) PUPIL_MIN(max_value, PUPIL_MAX(a, min_value))

#define PUPIL_VALID_INDEX(idx, range) (((idx) >= 0) && ((idx) < (range)))
#define PUPIL_PIN_INDEX(idx, range) PUPIL_PIN(idx, 0, (range)-1)

#define PUPIL_SIGN(x) ((((x) > 0.0f) ? 1.0f : 0.0f) + (((x) < 0.0f) ? -1.0f : 0.0f))
