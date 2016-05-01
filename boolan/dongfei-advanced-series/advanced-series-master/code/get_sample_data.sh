# Streaming data coming in, to get sample data with probability of 1/K
# Now, let's assume that this method works for files of ≤N lines. To prove that this means it works for N+1, in the (N+1)th iteration of the loop, there is a probability of 1/(N+1) that the last line will be selected (random(0, N+1) < 1 has that probability). Thus, the last line has 1/(N+1) probability of being selected. The probability of all other lines being selected is still going to be equal to each other, let's call this x. Then, N*x + 1/(N+1) == 1, which means that x = 1/(N+1)
i = 1
chosen_line = ""
for line in lines:
    if random() < 1/i:    # random returns a uniform random number in [0,1)
        chosen_line = line
    i += 1
return chosen_line
