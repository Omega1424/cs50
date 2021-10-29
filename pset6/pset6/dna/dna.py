from csv import reader, DictReader
from sys import argv, exit

def main():

    if len(argv) != 3:
        exit("Invalid CL arguments")

    ref_file = argv[1]
    new_dna = argv[2]

    with open (ref_file, "r") as rf:
        reader = DictReader(rf)
        dna_list = list(reader)

    with open(new_dna, "r") as nd:
        sequence = nd.read()

        max_counts = []

        for i in range(1, len(reader.fieldnames)):
            STR = reader.fieldnames[i]
            max_counts.append(0)

            for j in range(len(sequence)):
                STR_count = 0

                if sequence[j:(j+len(STR))] == STR:
                    k=0
                    while sequence[(j+k):(j+k+len(STR))] == STR:
                        STR_count+=1
                        k+=len(STR)
                    if STR_count > max_counts[i-1]:
                        max_counts[i-1] = STR_count

    for i in range(len(dna_list)):
        matches = 0
        for j in range(1, len(reader.fieldnames)):
            if int(max_counts[j - 1]) == int(dna_list[i]  [reader.fieldnames[j]]):
                matches += 1
            if matches == (len(reader.fieldnames) - 1):
                print(dna_list[i]['name'])
                exit(0)
    print("No match")

main()
