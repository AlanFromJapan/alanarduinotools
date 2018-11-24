
import datetime
import time

"""
int moon_phase(int y, int m, int d)
{
    /*
      calculates the moon phase (0-7), accurate to 1 segment.
      0 = > new moon.
      4 => full moon.
      */

    int c,e;
    double jd;
    int b;

    if (m < 3) {
        y--;
        m += 12;
    }
    ++m;
    c = 365.25*y;
    e = 30.6*m;
    jd = c+e+d-694039.09;  /* jd is total days elapsed */
    jd /= 29.53;           /* divide by the moon cycle (29.53 days) */
b = jd;   /* int(jd) -> b, take integer part of jd */
jd -= b;   /* subtract integer part to leave fractional part of original jd */
b = jd*8 + 0.5;   /* scale fraction from 0-8 and round by adding 0.5 */
b = b & 7;   /* 0 and 8 are the same so turn 8 into 0 */
    return b;
}
"""
def moonPhase1(d):
    #http://www.voidware.com/moon_phase.htm
    y = d.year
    m = d.month
    d = d.day

    if m < 3:
        y = y -1
        m = m + 12
    m = m + 1
    c = 365.25 * y
    e = 30.6 * m
    jd  = c + e + d - 694039.09
    jd = jd / 29.53

    jd = jd - float(int (jd))
    b = jd * 8 + 0.5
    b = b % 8
    return b
    

d = datetime.datetime.today()
print ("As of date %s he moon phase is %d" % (d, moonPhase1(d)))
