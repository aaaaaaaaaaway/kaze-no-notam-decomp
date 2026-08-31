


/**
 * kaze_approach_value  (library, 0x800225ec)
 *
 * Moves current toward target target by at most step step; returns the new
 * value.
 */

int kaze_approach_value(int current, int target, int step)
{
    int diff = target - current;

    if (diff < 0) {
        diff = -diff;
    }
    if (diff < step) {
        return target;
    }
    if (current < target) {
        return current + step;
    }
    return current - step;
}


/**
 * kaze_approach_angle  (library, 0x80022620)
 *
 * Angle-approach helper: wraps the target into the +-0x800 range of a
 * 0x1000-unit circle, then steps cur toward it by at most step.
 */

int kaze_approach_angle(int cur, int target, int step)
{
    int d;

    d = target - cur;
    if (d < 0) {
        d = -d;
    }
    if (d >= 0x800) {
        target -= 0x1000;
        if (target < -0xfff) {
            target = -target;
        }
    }
    d = target - cur;
    if (d < 0) {
        d = -d;
    }
    if (d < step) {
        return target;
    }
    if (cur < target) {
        return cur + step;
    }
    return cur - step;
}
