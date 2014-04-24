from sys import argv

freq = float(argv[1])     # Frequency to convert, taken in from the command line

period = 1 / freq

half_period = period / 2

clock_speed = 7373000     # Simon2 Board's Clock Speed

num_ticks = (half_period * clock_speed) / 2   # Number of clock ticks given the
                                              # frequency input by user

print num_ticks
