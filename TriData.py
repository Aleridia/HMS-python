dictionnaire = dict()

with open("XSens1200.csv") as f:
    content = f.readlines()

    nbLine = 0
    for line in content:
        split = line.split(",")
        split.pop(0)

        nbParam = 0
        for str in split:
            if nbLine == 0:
                dictionnaire[nbParam] = dict()
                dictionnaire[nbParam]['min'] = 999999.
                dictionnaire[nbParam]['max'] = -999999.
            else:
                if float(str) < float(dictionnaire[nbParam]['min']):
                    dictionnaire[nbParam]['min'] = float(str)

                if float(str) > float(dictionnaire[nbParam]['max']):
                    dictionnaire[nbParam]['max'] = float(str)
            nbParam += 1

        nbLine += 1

    print(dictionnaire)