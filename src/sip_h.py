import subprocess
import matplotlib.pyplot as plt
import numpy as np

SIP_H_NB_RUNS = 10000
NB_BINS = 100


#compile C source files
subprocess.call(
    ["gcc", "sip_h.c", "search_tree.c", "-o", "sip_h",
     "-O3", "-Ofast", "-Wall", "-Wextra", "-Wshadow"]
)


sip_h_seq = [
    int(out)
    for out
    in subprocess.check_output(["./sip_h", str(SIP_H_NB_RUNS)]).split()
]

plt.figure(1)
plt.hist(sip_h_seq, NB_BINS)
plt.xlabel("size of sequence to find a collision")
plt.ylabel("#occurences")
plt.title("Repartitions of sequence sizes after finding %s collisions" % SIP_H_NB_RUNS)
plt.legend()
plt.savefig("sip_h_collisions.png")


print("Average time to find a collision : %s calls" % np.mean(sip_h_seq))
print("Median time to find a collision : %s calls" % np.median(sip_h_seq))
print("Max value : %s, min value : %s" % (max(sip_h_seq), min(sip_h_seq)))
