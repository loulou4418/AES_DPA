import numpy as np

# Load traces
print("Loading traces ... ", end = '')
traces = np.load(r"traces.npy")
texts = np.load(r"texts.npy")
print("Done")

print("Extracting the traces to csv ... ", end = '')
# export text, integer format
np.savetxt("texts.csv",texts,fmt='%i',delimiter=',',newline='\n')
np.savetxt("traces.csv",texts,fmt='%i',delimiter=',',newline='\n')

# export trace in float format
np.savetxt("traces_double.csv",traces,fmt='%10.10f',delimiter=',',newline='\n')
print("Done")
