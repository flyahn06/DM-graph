with open("k_100.txt", 'w') as f:
    f.write("100\n")

    for i in range(1, 100 + 1):
        f.write(str(i))
        for j in range(1, 100 + 1):
            if not i == j:
                f.write(" " + str(j))
        f.write("\n")

with open("k_100_biased.txt", 'w') as f:
    f.write("100\n")

    for i in range(1, 100+1):
        f.write(str(i))
        for j in range(1, 100+1):
            if not i == j:
                f.write(f" {j} 1")
        f.write("\n")
